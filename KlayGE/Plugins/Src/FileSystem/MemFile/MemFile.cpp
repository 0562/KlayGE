// MemFile.cpp
// KlayGE �ڴ��ļ��� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/Engine.hpp>

#include <cassert>

#include <KlayGE/MemFile/MemFile.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	MemFile::MemFile()
				: curPos_(0)
	{
	}

	MemFile::MemFile(const void* data, size_t length)
				: curPos_(0)
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

		chunkData_.resize(length);
		if (data != NULL)
		{
			Engine::MemoryInstance().Cpy(&chunkData_[0], data, length);
		}

		this->Length(length);
		curPos_ = 0;
	}

	// �ر��ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void MemFile::Close()
	{
		curPos_ = 0;

		chunkData_.clear();
		std::vector<U8>().swap(chunkData_);
	}

	// ��ȡ�ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Length()
	{
		return chunkData_.size();
	}

	// �����ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	void MemFile::Length(size_t newLen)
	{
		chunkData_.resize(newLen);
	}

	// ������д���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Write(const void* data, size_t count)
	{
		assert(data != NULL);

		if (count > (this->Length() - curPos_))
		{
			this->Length(count + curPos_);
		}

		Engine::MemoryInstance().Cpy(&chunkData_[curPos_], data, count);
		curPos_ += count;

		return count;
	}

	// ���ļ���������
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Read(void* data, size_t count)
	{
		assert(data != NULL);

		if (count > (this->Length() - curPos_))
		{
			count = this->Length() - curPos_;
		}

		Engine::MemoryInstance().Cpy(data, &chunkData_[curPos_], count);
		curPos_ += count;

		return count;
	}

	// ���ļ��������ݵ���ǰ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::CopyFrom(VFile& src, size_t size)
	{
		std::vector<U8> data(size);
		size = src.Read(&data[0], data.size());
		return this->Write(&data[0], size);
	}

	// ���ļ�ָ���Ƶ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Seek(size_t offset, SeekMode from)
	{
		switch (from)
		{
		case SM_Begin:
			curPos_ = offset;
			break;

		case SM_End:
			curPos_ = this->Length() - 1 - offset;
			break;

		case SM_Current:
			curPos_ += offset;
			break;
		}

		return curPos_;
	}

	// ��ȥ�ļ�ָ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t MemFile::Tell()
	{
		return curPos_;
	}
}
