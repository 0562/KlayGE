// DiskFile.hpp
// KlayGE �����ļ��� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DISKFILE_HPP
#define _DISKFILE_HPP

#include <KlayGE/VFile.hpp>
#include <KlayGE/SharedPtr.hpp>
#include <fstream>

#pragma comment(lib, "KlayGE_FileSystem_DiskFile.lib")

namespace KlayGE
{
	// ��������ļ���д
	/////////////////////////////////////////////////////////////////////////////////
	class DiskFile : public VFile
	{
	public:
		DiskFile();
		DiskFile(const WString& fileName, OpenMode openMode);
		DiskFile(const DiskFile& rhs);
		~DiskFile();

		DiskFile& operator=(const DiskFile& rhs);

		void Open(const WString& fileName, OpenMode openMode);
		void Close();

		VFilePtr Clone() const;

		size_t Length();
		void Length(size_t newLen);

		size_t Write(const void* data, size_t count);
		size_t Read(void* data, size_t count);
		size_t CopyFrom(VFile& src, size_t size);

		size_t Seek(size_t offset, SeekMode from);
		size_t Tell();

		void Flush();

		void Swap(DiskFile& rhs);

	private:
		SharedPtr<std::fstream> file_;

		WString		fileName_;
		OpenMode	openMode_;
	};
}

#endif			// _DISKFILE_HPP