// MemFile.cpp
// KlayGE �ڴ��ļ��� ʵ���ļ�
// Ver 2.0.5
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/Memory.hpp>

#include <cassert>
#include <string>
#include <vector>

#include <KlayGE/MemFile/MemFile.hpp>

using namespace std;

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	MemFile::MemFile()
	{
	}

	MemFile::MemFile(const void* data, size_t length)
	{
		this->Open(data, length);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	MemFile::~MemFile()
	{
		this->Close();
	}

	// ���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void MemFile::Open(const void* data, size_t length)
	{
		this->Close();

		chunkData_.str(string(static_cast<const char*>(data)));
	}

	// �ر��ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void MemFile::Close()
	{
		chunkData_.clear();
	}

	// ��ȡ�ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Length()
	{
		size_t curPos(this->Tell());
		size_t len(this->Seek(0, SM_End));
		this->Seek(curPos, SM_Begin);

		return len;
	}

	// �����ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	void MemFile::Length(size_t newLen)
	{
		this->Seek(newLen, SM_Begin);

		short eof(EOF);
		this->Write(&eof, sizeof(eof));
	}

	// ������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Write(const void* data, size_t count)
	{
		assert(data != NULL);

		chunkData_.write(static_cast<const char*>(data),
			static_cast<std::streamsize>(count));

		chunkData_.seekg(static_cast<istream::off_type>(count), ios_base::cur);

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

		chunkData_.read(static_cast<char*>(data),
			static_cast<std::streamsize>(count));

		chunkData_.seekp(static_cast<istream::off_type>(count), ios_base::cur);

		return count;
	}

	// ���ļ��������ݵ���ǰ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::CopyFrom(VFile& src, size_t size)
	{
		vector<U8, alloc<U8> > data(size);
		size = src.Read(&data[0], data.size());
		return this->Write(&data[0], size);
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Seek(size_t offset, SeekMode from)
	{
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

		chunkData_.seekg(static_cast<istream::off_type>(offset), seekFrom);
		chunkData_.seekp(static_cast<ostream::off_type>(offset), seekFrom);

		return this->Tell();
	}

	// ��ȥ�ļ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Tell()
	{
		return chunkData_.tellg();
	}
}
