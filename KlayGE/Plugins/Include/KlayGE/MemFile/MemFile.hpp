// MemFile.hpp
// KlayGE �ڴ��ļ��� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _MEMFILE_HPP
#define _MEMFILE_HPP

#include <sstream>
#include <KlayGE/VFile.hpp>

#include <boost/utility.hpp>

#pragma comment(lib, "KlayGE_FileSystem_MemFile.lib")

namespace KlayGE
{
	class MemFile : boost::noncopyable, public VFile
	{
	public:
		MemFile();
		MemFile(void const * data, size_t length);
		~MemFile();

		void Open(void const * data, size_t length);
		void Close();

		size_t Length();
		void Length(size_t newLen);

		size_t Write(void const * data, size_t count);
		size_t Read(void* data, size_t count);
		size_t CopyFrom(VFile& src, size_t size);

		size_t Seek(size_t offset, SeekMode from);
		size_t Tell();

	private:
		std::stringstream	chunkData_;
	};
}

#endif			// _MEMFILE_HPP
