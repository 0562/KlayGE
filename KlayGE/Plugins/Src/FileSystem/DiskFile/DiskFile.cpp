// DiskFile.cpp
// KlayGE �����ļ��� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/CommFuncs.hpp>

#include <vector>
#include <cassert>

#include <KlayGE/DiskFile/DiskFile.hpp>

using namespace std;

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DiskFile::DiskFile()
			: openMode_(OM_Unknown)
	{
	}

	DiskFile::DiskFile(const WString& fileName, OpenMode openMode)
			: openMode_(OM_Unknown)
	{
		this->Open(fileName, openMode);
	}

	DiskFile::DiskFile(const DiskFile& rhs)
			: openMode_(OM_Unknown)
	{
		this->Open(rhs.fileName_, rhs.openMode_);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DiskFile::~DiskFile()
	{
		this->Close();
	}

	// ���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Open(const WString& fileName, OpenMode openMode)
	{
		this->Close();

		ios_base::openmode mode(ios_base::binary);

		switch (openMode)
		{
		case OM_Read:
			mode |= ios_base::in;
			break;

		case OM_Write:
			mode |= ios_base::out;
			break;

		case OM_ReadWrite:
			mode |= ios_base::in | ios_base::out;
			break;
			
		case OM_Create:
			mode |= ios_base::out | ios_base::trunc;
			break;
		}

		String fn;
		Convert(fn, fileName);
		file_ = SharePtr<fstream>(new fstream(fn.c_str(), mode));

		fileName_ = fileName;
		openMode_ = openMode;
	}

	// �ر��ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Close()
	{
		if ((file_.Get() != NULL) && file_->is_open())
		{
			file_->close();
			file_ = SharePtr<fstream>();
		}
	}

	// ��¡һ���������ͬ��ָ��
	/////////////////////////////////////////////////////////////////////////////////
	VFilePtr DiskFile::Clone() const
	{
		return VFilePtr(new DiskFile(*this));
	}

	// ����=
	/////////////////////////////////////////////////////////////////////////////////
	DiskFile& DiskFile::operator=(const DiskFile& rhs)
	{
		DiskFile(rhs).Swap(*this);
		return *this;
	}

	// ��ȡ�ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Length()
	{
		assert((file_.Get() != NULL) && file_->is_open());

		size_t curPos(this->Tell());
		size_t len(this->Seek(0, SM_End));
		this->Seek(curPos, SM_Begin);

		return len;
	}

	// �����ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Length(size_t newLen)
	{
		assert((file_.Get() != NULL) && file_->is_open());

		this->Seek(newLen, SM_Begin);

		short eof(EOF);
		this->Write(&eof, sizeof(eof));
	}

	// ������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Write(const void* data, size_t count)
	{
		assert((file_.Get() != NULL) && file_->is_open());
		assert(data != NULL);

		file_->write(static_cast<const char*>(data), count);

		return count;
	}

	// ���ļ���������
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Read(void* data, size_t count)
	{
		assert((file_.Get() != NULL) && file_->is_open());
		assert(data != NULL);

		if (this->Tell() + count > this->Length())
		{
			count = this->Length() - this->Tell();
		}

		file_->read(static_cast<char*>(data), count);

		return count;
	}

	// ���ļ��������ݵ���ǰ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::CopyFrom(VFile& src, size_t size)
	{
		std::vector<U8> data(size);
		return this->Write(&data[0], src.Read(&data[0], data.size()));
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Seek(size_t offset, SeekMode from)
	{
		assert((file_.Get() != NULL) && file_->is_open());

		ios_base::seekdir seekFrom(ios_base::beg);
		switch (from)
		{
		case SM_Begin:
			seekFrom = ios_base::beg;
			break;

		case SM_End:
			seekFrom = ios_base::end;
			break;

		case SM_Current:
			seekFrom = ios_base::cur;
			break;
		}

		if (openMode_ & ios_base::in)
		{
			file_->seekg(offset, seekFrom);
		}
		else
		{
			file_->seekp(offset, seekFrom);
		}

		return this->Tell();
	}

	// ��ȥ�ļ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Tell()
	{
		assert((file_.Get() != NULL) && file_->is_open());

		if (openMode_ & ios_base::in)
		{
			return file_->tellg();
		}
		else
		{
			return file_->tellp();
		}
	}

	// ����������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Flush()
	{
		assert((file_.Get() != NULL) && file_->is_open());

		file_->flush();
	}

	// ���������ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::Swap(DiskFile& rhs)
	{
		std::swap(file_, rhs.file_);
		std::swap(fileName_, rhs.fileName_);
		std::swap(openMode_, rhs.openMode_);
	}
}
