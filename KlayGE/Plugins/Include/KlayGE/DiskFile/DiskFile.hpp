// DiskFile.hpp
// KlayGE �����ļ��� ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// �Ѵ���������󵽻��� (2004.10.21)
//
// 2.1.0
// �ļ�������string������ (2004.4.14)
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
		DiskFile(std::string const & fileName, OpenMode openMode);

		size_t Write(void const * data, size_t count);
		size_t Read(void* data, size_t count);

		void OnSeek(size_t offset, std::ios_base::seekdir from);

	private:
		std::string		fileName_;
	};

	class DiskFileResIdentifier : public ResIdentifier
	{
	public:
		DiskFileResIdentifier(std::string const & fileName);

		VFilePtr Load();

	private:
		std::string fileName_;
	};
}

#endif			// _DISKFILE_HPP
