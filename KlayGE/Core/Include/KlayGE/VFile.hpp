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

#include <KlayGE/PreDeclare.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
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

		static VFilePtr NullObject();

		virtual void Close() = 0;

		virtual size_t Length() = 0;
		virtual void Length(size_t newLen) = 0;

		virtual size_t Write(void const * data, size_t count) = 0;
		virtual size_t Read(void* data, size_t count) = 0;
		virtual size_t CopyFrom(VFile& src, size_t size) = 0;

		virtual size_t Seek(size_t offset, SeekMode from) = 0;
		virtual size_t Tell() = 0;
		virtual void Rewind()
			{ this->Seek(0, SM_Begin); }
	};
}

#endif		// _VFILE_HPP
