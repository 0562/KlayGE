// MemFile.cpp
// KlayGE �ڴ��ļ��� ʵ���ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// �Ѵ���������󵽻��� (2004.10.21)
//
// 2.0.5
// ��Ϊstringstreamʵ�� (2004.4.9)
//
// 2.0.0
// ���ν��� (2003.8.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <cassert>
#include <string>
#include <vector>

#include <KlayGE/MemFile/MemFile.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	MemFile::MemFile()
				: VFile(OM_ReadWrite)
	{
		stream_ = boost::shared_ptr<std::iostream>(new std::stringstream);
	}

	MemFile::MemFile(void const * data, size_t length)
				: VFile(OM_ReadWrite)
	{
		stream_ = boost::shared_ptr<std::iostream>(
				new std::stringstream(std::string(static_cast<char const *>(data))));
	}

	// ������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Write(void const * data, size_t count)
	{
		assert(data != NULL);

		stream_->write(static_cast<char const *>(data),
			static_cast<std::streamsize>(count));

		stream_->seekg(static_cast<std::istream::off_type>(count), std::ios_base::cur);

		return count;
	}

	// ���ļ���������
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Read(void* data, size_t count)
	{
		assert(data != NULL);

		if (this->Tell() + count > this->Length())
		{
			count = this->Length() - this->Tell();
		}

		stream_->read(static_cast<char*>(data),
			static_cast<std::streamsize>(count));

		stream_->seekp(static_cast<std::istream::off_type>(count), std::ios_base::cur);

		return count;
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	void MemFile::OnSeek(size_t offset, std::ios_base::seekdir from)
	{
		stream_->seekp(static_cast<std::iostream::off_type>(offset), from);
		stream_->seekg(static_cast<std::iostream::off_type>(offset), from);
	}
}
