// OggSource.cpp
// KlayGE Ogg Vorbis����Դ�� ʵ���ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2003-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ��vorbisfile��д�����Է���ԭʼ��С (2007.7.12)
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <vector>
#include <cstring>
#include <boost/assert.hpp>

#include <KlayGE/OggWav/OggSource.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#ifdef KLAYGE_DEBUG
#pragma comment(lib, "ogg_static_d.lib")
#pragma comment(lib, "vorbis_static_d.lib")
#pragma comment(lib, "vorbisfile_static_d.lib")
#else
#pragma comment(lib, "ogg_static.lib")
#pragma comment(lib, "vorbis_static.lib")
#pragma comment(lib, "vorbisfile_static.lib")
#endif
#endif

size_t const READSIZE = 4096;

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	OggSource::OggSource(ResIdentifierPtr const & file)
				: oggFile_(file)
	{
		oggFile_->seekg(0, std::ios_base::end);
		length_ = oggFile_->tellg();
		oggFile_->seekg(0, std::ios_base::beg);

		ov_callbacks vorbis_callbacks;
		vorbis_callbacks.read_func = OggSource::VorbisRead;
		vorbis_callbacks.close_func = OggSource::VorbisClose;
		vorbis_callbacks.seek_func = OggSource::VorbisSeek;
		vorbis_callbacks.tell_func = OggSource::VorbisTell;

		Verify(0 == ov_open_callbacks(this, &vf_, NULL, 0, vorbis_callbacks));

		vorbis_info* vorbis_info = ov_info(&vf_, -1);
		format_ = (1 == vorbis_info->channels) ? AF_Mono16 : AF_Stereo16;
		freq_ = vorbis_info->rate;

		this->Reset();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	OggSource::~OggSource()
	{
		ov_clear(&vf_);
	}

	// ��ȡOgg����
	/////////////////////////////////////////////////////////////////////////////////
	size_t OggSource::Read(void* data, size_t size)
	{
		BOOST_ASSERT(data != NULL);

		char* pcm = static_cast<char*>(data);

		size_t cur_size = 0;
		int section;
		while (cur_size < size)
		{
			int result = ov_read(&vf_, pcm + cur_size, static_cast<int>(size - cur_size), 0, 2, 1, &section);
	    
			if (result > 0)
			{
				cur_size += result;
			}
			else
			{
				if (result <= 0)
				{
					break;
				}
			}
		}

		return cur_size;
	}

	// ��������Դ��С
	/////////////////////////////////////////////////////////////////////////////////
	size_t OggSource::Size()
	{
		vorbis_info* vorbis_info = ov_info(&vf_, -1);
		return static_cast<size_t>(ov_pcm_total(&vf_, -1) * vorbis_info->channels * sizeof(ogg_int16_t));
	}

	// ����Դ��λ
	/////////////////////////////////////////////////////////////////////////////////
	void OggSource::Reset()
	{
		oggFile_->clear();
		ov_pcm_seek(&vf_, 0);
	}

	size_t OggSource::VorbisRead(void* ptr, size_t byte_size, size_t size_to_read, void* datasource)
	{
		// Get the data in the right format
		OggSource* vorbis_data = static_cast<OggSource*>(datasource);

		size_t actual_size_to_read;	// How much data we are actually going to read from memory
		// Calculate how much we need to read.  This can be sizeToRead*byteSize or less depending on how near the EOF marker we are
		size_t space_to_eof = vorbis_data->length_ - vorbis_data->oggFile_->tellg();
		if (size_to_read * byte_size < space_to_eof)
		{
			actual_size_to_read = size_to_read * byte_size;
		}
		else
		{
			actual_size_to_read = space_to_eof;
		}
		
		// A simple copy of the data from memory to the datastruct that the vorbis libs will use
		if (actual_size_to_read > 0)
		{
			vorbis_data->oggFile_->read(static_cast<char*>(ptr), static_cast<std::streamsize>(actual_size_to_read));
		}

		// Return how much we read (in the same way fread would)
		return actual_size_to_read;
	}

	int OggSource::VorbisSeek(void* datasource, ogg_int64_t offset, int whence)
	{
		OggSource* vorbis_data = static_cast<OggSource*>(datasource);

		std::ios_base::seek_dir dir;
		switch (whence)
		{
		case SEEK_SET: // Seek to the start of the data file
			dir = std::ios_base::beg;
			break;

		case SEEK_CUR: // Seek from where we are
			dir = std::ios_base::cur;
			break;

		case SEEK_END: // Seek from the end of the file
			dir = std::ios_base::end;
			break;

		default:
			BOOST_ASSERT(false);
			dir = std::ios_base::beg;
			break;
		};

		vorbis_data->oggFile_->seekg(static_cast<long>(offset), dir);

		return 0;
	}

	int OggSource::VorbisClose(void* datasource)
	{
		OggSource* vorbis_data = static_cast<OggSource*>(datasource);
		vorbis_data->oggFile_.reset();
		return 1;
	}

	long OggSource::VorbisTell(void* datasource)
	{
		OggSource* vorbis_data = static_cast<OggSource*>(datasource);
		return static_cast<long>(vorbis_data->oggFile_->tellg());
	}
}
