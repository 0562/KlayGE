// VFile.hpp
// KlayGE �����ļ��� ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ������������������� (2004.10.21)
//
// 2.0.0
// ���ν��� (2003.8.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _VFILE_HPP
#define _VFILE_HPP

#include <KlayGE/PreDeclare.hpp>

#include <ios>
#include <boost/shared_ptr.hpp>

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
		VFile(OpenMode openMode);
		virtual ~VFile();

		static VFilePtr NullObject();

		bool Fail();

		void Close();

		size_t Length();

		virtual size_t Write(void const * data, size_t count) = 0;
		virtual size_t Read(void* data, size_t count) = 0;
		size_t CopyFrom(VFile& src, size_t size);

		size_t Seek(size_t offset, SeekMode from);
		size_t Tell();
		void Rewind();

	protected:
		virtual void OnSeek(size_t offset, std::ios_base::seekdir from) = 0;

		boost::shared_ptr<std::iostream> stream_;
		OpenMode		openMode_;
	};
}

#endif		// _VFILE_HPP
