// VFile.cpp
// KlayGE �����ļ��� ʵ���ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ������������������� (2004.10.21)
//
// 2.0.0
// ���ν��� (2003.8.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <istream>
#include <vector>

#include <KlayGE/VFile.hpp>

namespace KlayGE
{
	// ���ļ�������
	/////////////////////////////////////////////////////////////////////////////////
	class NullVFile : public VFile
	{
	public:
		NullVFile()
			: VFile(OM_Unknown)
			{ }

		size_t Write(void const * data, size_t count)
			{ return 0; }
		size_t Read(void* data, size_t count)
			{ return 0; }

		void OnSeek(size_t offset, std::ios_base::seekdir from)
			{ }
	};

	// ���ؿն���
	/////////////////////////////////////////////////////////////////////////////////
	VFilePtr VFile::NullObject()
	{
		static VFilePtr obj(new NullVFile);
		return obj;
	}

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	VFile::VFile(OpenMode openMode)
			: openMode_(openMode)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	VFile::~VFile()
	{
		this->Close();
	}

	// �����Ƿ�ʧ��
	/////////////////////////////////////////////////////////////////////////////////
	bool VFile::Fail()
	{
		return stream_->fail();
	}

	// �ر��ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void VFile::Close()
	{
		stream_.reset();
	}

	// ��ȡ�ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	size_t VFile::Length()
	{
		size_t curPos(this->Tell());
		size_t len(this->Seek(0, SM_End));
		this->Seek(curPos, SM_Begin);

		return len;
	}

	// ���ļ��������ݵ���ǰ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t VFile::CopyFrom(VFile& src, size_t size)
	{
		std::vector<U8> data(size);
		size = src.Read(&data[0], data.size());
		return this->Write(&data[0], size);
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t VFile::Seek(size_t offset, SeekMode from)
	{
		assert(stream_);

		std::ios_base::seekdir seekFrom(std::ios_base::beg);
		switch (from)
		{
		case SM_Begin:
			seekFrom = std::ios_base::beg;
			break;

		case SM_End:
			seekFrom = std::ios_base::end;
			break;

		case SM_Current:
			seekFrom = std::ios_base::cur;
			break;
		}

		this->OnSeek(static_cast<std::iostream::off_type>(offset), seekFrom);

		return this->Tell();
	}

	// ��ȡ�ļ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t VFile::Tell()
	{
		assert(stream_);
		assert(!this->Fail());

		if (OM_Read == openMode_)
		{
			return stream_->tellg();
		}
		else
		{
			return stream_->tellp();
		}
	}

	// �ļ�ָ�����
	/////////////////////////////////////////////////////////////////////////////////
	void VFile::Rewind()
	{
		this->Seek(0, SM_Begin);
	}
}
