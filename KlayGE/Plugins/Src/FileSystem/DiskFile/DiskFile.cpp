// DiskFile.cpp
// KlayGE �����ļ��� ʵ���ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// �Ѵ���������󵽻��� (2004.10.21)
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
	DiskFile::DiskFile(std::string const & fileName, OpenMode openMode)
				: VFile(openMode)
	{
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
		stream_ = boost::shared_ptr<std::iostream>(new std::fstream(fileName_.c_str(), mode));
	}

	// ������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Write(void const * data, size_t count)
	{
		assert(stream_);
		assert(data != NULL);

		stream_->write(static_cast<char const *>(data),
			static_cast<std::streamsize>(count));

		return count;
	}

	// ���ļ���������
	/////////////////////////////////////////////////////////////////////////////////
	size_t DiskFile::Read(void* data, size_t count)
	{
		assert(stream_);
		assert(data != NULL);

		if (this->Tell() + count > this->Length())
		{
			count = this->Length() - this->Tell();
		}

		stream_->read(static_cast<char*>(data),
			static_cast<std::streamsize>(count));

		return count;
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	void DiskFile::OnSeek(size_t offset, std::ios_base::seekdir from)
	{
		assert(stream_);

		if (std::ios_base::in == openMode_)
		{
			stream_->seekg(static_cast<std::iostream::off_type>(offset), from);
		}
		else
		{
			stream_->seekp(static_cast<std::iostream::off_type>(offset), from);
		}
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
