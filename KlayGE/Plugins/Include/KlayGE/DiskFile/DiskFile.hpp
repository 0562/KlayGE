// DiskFile.hpp
// KlayGE �����ļ��� ͷ�ļ�
// Ver 2.1.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.0
// �ļ�������String������ (2004.4.14)
//
// 2.0.0
// ���ν��� (2003.8.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DISKFILE_HPP
#define _DISKFILE_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/VFile.hpp>
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/ResLocator.hpp>

#include <fstream>

#include <boost/utility.hpp>

#pragma comment(lib, "KlayGE_FileSystem_DiskFile.lib")

namespace KlayGE
{
	// ��������ļ���д
	/////////////////////////////////////////////////////////////////////////////////
	class DiskFile : boost::noncopyable, public VFile
	{
	public:
		DiskFile();
		DiskFile(const std::string& fileName, OpenMode openMode);
		~DiskFile();

		bool Open(const std::string& fileName, OpenMode openMode);
		void Close();

		size_t Length();
		void Length(size_t newLen);

		size_t Write(const void* data, size_t count);
		size_t Read(void* data, size_t count);
		size_t CopyFrom(VFile& src, size_t size);

		size_t Seek(size_t offset, SeekMode from);
		size_t Tell();

		void Flush();

	private:
		std::fstream	file_;

		std::string		fileName_;
		OpenMode		openMode_;
	};

	class DiskFileResIdentifier : public ResIdentifier
	{
	public:
		DiskFileResIdentifier(const std::string& fileName);

		VFilePtr Load();

	private:
		std::string fileName_;
	};
}

#endif			// _DISKFILE_HPP
