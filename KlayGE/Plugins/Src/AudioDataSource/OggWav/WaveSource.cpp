// WaveSource.cpp
// KlayGE wave����Դ�� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <vector>
#include <cassert>

#include <KlayGE/OggWav/WaveSource.hpp>

namespace KlayGE
{
	enum
	{
		WaveFmt_PCM			= 0x01,
		//WaveFmt_IMAADPCM	= 0x11,
		//WaveFmt_MP3			= 0x55,
	};

	enum
	{
		frRIFF	= MakeFourCC<'R', 'I', 'F', 'F'>::value,
		frWAVE	= MakeFourCC<'W', 'A', 'V', 'E'>::value,
		frFmt	= MakeFourCC<'f', 'm', 't', ' '>::value,
		frData	= MakeFourCC<'d', 'a', 't', 'a'>::value,
	};

	#ifdef _MSC_VER
		#pragma pack (push, 1)						// �رն���
	#endif

	struct WAVChunkHdr
	{
		FourCC		id;
		U32			size;
	};

	struct WAVFileHdr
	{
		WAVChunkHdr chuck;
		FourCC		type;
	};

	struct WaveFmt
	{
		U16			formatTag;
		U16			channels;
		U32			samplesPerSec;
		U32			avgBytesPerSec;
		U16			blockAlign;
	};

	struct PCMWaveFmt
	{
		WaveFmt		wf;
		U16			bitsPerSample;
	};

	struct WaveFmtEx
	{
		WaveFmt		wf;
		U16			bitsPerSample;
		U16			extraSize;
	};

	#ifdef _MSC_VER
		#pragma pack (pop)
	#endif

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	WaveSource::WaveSource(const VFile& file)
	{
		wavFile_ = file.Clone();

		this->ReadMMIO();
		this->Reset();
	}

	// ��ȡ�ļ�ͷ
	/////////////////////////////////////////////////////////////////////////////////
	void WaveSource::ReadMMIO()
	{
		wavFile_->Seek(0, VFile::SM_Begin);

		WAVFileHdr fileHdr;
		wavFile_->Read(&fileHdr, sizeof(fileHdr));

		// ����Ƿ���һ����Ч�� Wave �ļ�
		assert(frRIFF == fileHdr.chuck.id);
		assert(frWAVE == fileHdr.type);

		PCMWaveFmt	pcmWaveFmt;
		WAVChunkHdr chunkHdr;
		while (wavFile_->Tell() != wavFile_->Length())
		{
			wavFile_->Read(&chunkHdr, sizeof(chunkHdr));

			switch (chunkHdr.id)
			{
			case frFmt:
				wavFile_->Read(&pcmWaveFmt, sizeof(pcmWaveFmt));
				assert(WaveFmt_PCM == pcmWaveFmt.wf.formatTag);
				wavFile_->Seek(chunkHdr.size - sizeof(pcmWaveFmt), VFile::SM_Current);

				freq_ = pcmWaveFmt.wf.samplesPerSec;
				if (1 == pcmWaveFmt.wf.formatTag)
				{
					if (1 == pcmWaveFmt.wf.channels)
					{
						if (8 == pcmWaveFmt.bitsPerSample)
						{
							format_ = AF_Mono8;
						}
						else
						{
							format_ = AF_Mono16;
						}
					}
					else
					{
						if (8 == pcmWaveFmt.bitsPerSample)
						{
							format_ = AF_Stereo8;
						}
						else
						{
							format_ = AF_Stereo16;
						}
					}
				}
				break;

			case frData:
				dataOffset_ = wavFile_->Tell();
				size_ = chunkHdr.size;
				wavFile_->Seek(chunkHdr.size, VFile::SM_Current);
				break;

			default:
				wavFile_->Seek(chunkHdr.size, VFile::SM_Current);
				break;
			}

			wavFile_->Seek(chunkHdr.size & 1, VFile::SM_Current);
		}
	}

	// ��ȡWav����
	/////////////////////////////////////////////////////////////////////////////////
	size_t WaveSource::Read(U8* data, size_t size)
	{
		assert(data != NULL);

		return wavFile_->Read(data, size);
	}

	// ����Դ��λ
	/////////////////////////////////////////////////////////////////////////////////
	void WaveSource::Reset()
	{
		wavFile_->Seek(dataOffset_, VFile::SM_Begin);
	}

	// ��������Դ��С
	/////////////////////////////////////////////////////////////////////////////////
	size_t WaveSource::Size()
	{
		return size_;
	}
}
