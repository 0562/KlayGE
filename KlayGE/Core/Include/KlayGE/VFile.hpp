// VFile.hpp
// KlayGE �����ļ��� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _VFILE_HPP
#define _VFILE_HPP

#include <KlayGE/SharedPtr.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	class VFile;
	typedef SharedPtr<VFile> VFilePtr;

	// �����ļ�����
	/////////////////////////////////////////////////////////////////////////////////
	class VFile
	{
	public:
		enum OpenMode
		{
			OM_Read,
			OM_Write,
			OM_ReadWrite,
			OM_Create,
			OM_Unknown,
		};

		enum SeekMode
		{
			SM_Begin,
			SM_End,
			SM_Current,
		};

	public:
		virtual ~VFile()
			{ }

		virtual void Close() = 0;

		virtual size_t Length() = 0;
		virtual void Length(size_t newLen) = 0;

		virtual size_t Write(const void* data, size_t count) = 0;
		virtual size_t Read(void* data, size_t count) = 0;
		virtual size_t CopyFrom(VFile& src, size_t size) = 0;

		virtual size_t Seek(size_t offset, SeekMode from) = 0;
		virtual size_t Tell() = 0;
		virtual void Rewind()
			{ this->Seek(0, SM_Begin); }
	};
}

#endif		// _VFILE_HPP
