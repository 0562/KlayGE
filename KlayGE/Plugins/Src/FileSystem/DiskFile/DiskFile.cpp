// DiskFile.cpp
// KlayGE �����ļ��� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ������Seek��bug (2004.8.11)
//
// 2.0.4
// ������Read��bug (2004.3.22)
//
// 2.0.0
// ���ν��� (2003.8.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <vector>
#include <cassert>

#include <KlayGE/DiskFile/DiskFile.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DiskFile::DiskFile()
			: openMode_(OM_Unknown)
	{
	}

	DiskFile::DiskFile(std::string const & fileName, OpenMode openMode)
			: openMode_(OM_Unknown)
	{
		this->Open(fileName, openMode);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DiskFile::~DiskFile()
	{
		this->Close();
	}

	// ���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	bool DiskFile::Open(std::string const & fileName, OpenMode openMode)
	{
		this->Close();

		std::ios_base::openmode mode(std::ios_base::binary);

		switch (openMode)
		{
		case OM_Read:
			mode |= std::ios_base::in;
			break;

		case OM_Write:
			mode |= std::ios_base::out;
			break;

		case OM_ReadWrite:
			mode |= std::ios_base::in | std::ios_base::out;
			break;
			
		case OM_Create:
			mode |= std::ios_base::out | std::ios_base::trunc;
			break;
		}

		fileName_ = fileName;
		openMode_ = openMode;

		file_.open(fileName.c_str(), mode);

		return !file_.fail();
	}

	// �ر��ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Close()
	{
		if (file_.is_open())
		{
			file_.close();
		}
		file_.clear();
	}

	// ��ȡ�ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Length()
	{
		assert(file_.is_open());

		size_t curPos(this->Tell());
		size_t len(this->Seek(0, SM_End));
		this->Seek(curPos, SM_Begin);

		return len;
	}

	// �����ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Length(size_t newLen)
	{
		assert(file_.is_open());

		this->Seek(newLen, SM_Begin);

		short eof(EOF);
		this->Write(&eof, sizeof(eof));
	}

	// ������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Write(void const * data, size_t count)
	{
		assert(file_.is_open());
		assert(data != NULL);

		file_.write(static_cast<char const *>(data),
			static_cast<std::streamsize>(count));

		return count;
	}

	// ���ļ���������
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Read(void* data, size_t count)
	{
		assert(file_.is_open());
		assert(data != NULL);

		if (this->Tell() + count > this->Length())
		{
			count = this->Length() - this->Tell();
		}

		file_.read(static_cast<char*>(data),
			static_cast<std::streamsize>(count));

		return count;
	}

	// ���ļ��������ݵ���ǰ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::CopyFrom(VFile& src, size_t size)
	{
		std::vector<U8> data(size);
		size = src.Read(&data[0], data.size());
		return this->Write(&data[0], size);
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Seek(size_t offset, SeekMode from)
	{
		assert(file_.is_open());

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

		if (OM_Read == openMode_)
		{
			file_.seekg(static_cast<std::istream::off_type>(offset), seekFrom);
		}
		else
		{
			file_.seekp(static_cast<std::ostream::off_type>(offset), seekFrom);
		}

		return this->Tell();
	}

	// ��ȥ�ļ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Tell()
	{
		assert(file_.is_open());
		assert(!file_.fail());

		if (OM_Read == openMode_)
		{
			return file_.tellg();
		}
		else
		{
			return file_.tellp();
		}
	}

	// ����������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Flush()
	{
		assert(file_.is_open());

		file_.flush();
	}


	DiskFileResIdentifier::DiskFileResIdentifier(std::string const & fileName)
		: fileName_(fileName)
	{
	}

	VFilePtr DiskFileResIdentifier::Load()
	{
		return VFilePtr(new DiskFile(fileName_, VFile::OM_Read));
	};
}
