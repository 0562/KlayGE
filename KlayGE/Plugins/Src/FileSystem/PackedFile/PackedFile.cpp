// PackedFile.cpp
// KlayGE ����ļ������������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.8)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <cassert>

#include <KlayGE/DiskFile/DiskFile.hpp>
#include <KlayGE/MemFile/MemFile.hpp>
#include <KlayGE/PackedFile/PackedFile.hpp>

namespace KlayGE
{
	PackedFile::PackedFile()
	{
	}

	PackedFile::PackedFile(const std::string& pathName)
	{
		this->Open(pathName);
	}

	bool PackedFile::Open(const std::string& pathName)
	{
		const std::string::size_type offset(pathName.rfind(".pkt/"));
		const std::string pktName(pathName.substr(0, offset + 4));
		const std::string fileName(pathName.substr(offset + 5));

		SharedPtr<DiskFile> pktFile(new DiskFile);
		if (!pktFile->Open(pktName, VFile::OM_Read))
		{
			return false;
		}
		else
		{
			pktFile_ = pktFile;
		}

		try
		{
			unPkt_.Open(pktFile_);

			this->Close();

			unPkt_.LocateFile(fileName);

			file_ = VFilePtr(new MemFile);

			std::vector<U8> data(unPkt_.CurFileSize());
			unPkt_.ReadCurFile(&data[0]);
			file_->Write(&data[0], data.size());
			file_->Rewind();
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	void PackedFile::Close()
	{
		if (file_.Get() != NULL)
		{
			file_->Close();
		}
	}

	size_t PackedFile::Length()
	{
		return file_->Length();
	}

	size_t PackedFile::Read(void* data, size_t count)
	{
		assert(data != NULL);

		return file_->Read(data, count);
	}

	size_t PackedFile::Seek(size_t off, SeekMode from)
	{
		return file_->Seek(off, from);
	}

	size_t PackedFile::Tell()
	{
		return file_->Tell();
	}


	PackedFileResIdentifier::PackedFileResIdentifier(const std::string& fileName)
		: fileName_(fileName)
	{
	}

	VFilePtr PackedFileResIdentifier::Load()
	{
		return VFilePtr(new PackedFile(fileName_));
	};
}
