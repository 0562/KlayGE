// Pkt.hpp
// KlayGE ����ļ���ȡ�� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
// LZSSѹ���㷨�������� Haruhiko Okumura
//
// 2.0.0
// ���ν��� (2003.9.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _PKT_HPP
#define _PKT_HPP

#include <KlayGE/VFile.hpp>
#include <KlayGE/tree.hpp>
#include <vector>

#pragma comment(lib, "KlayGE_FileSystem_PackedFile.lib")

namespace KlayGE
{
	#ifdef _MSC_VER
		#pragma pack(push, 1)
	#endif

	struct FileDes
	{
		WString		fileName;
		U32			start;
		U32			length;
		U32			DeComLength;
		U32			crc32;
		U8			attr;
	};

	struct PktHeader
	{
		U32		magic;
		U32		ver;
		U32		DTStart;
		U32		DTLength;
		U32		DTDeComLength;
		U32		FIStart;
	};

	#ifdef _MSC_VER
		#pragma pack(pop)
	#endif

	typedef tree<FileDes> DirTable;

	enum FileAttrib
	{
		FA_IsDir			= 1,
		FA_UnCompressed		= 2,
	};

	enum DirItemTag
	{
		DIT_Dir		= 1UL << 0,
		DIT_UnDir	= 1UL << 1,
		DIT_File	= 1UL << 2,
	};

	// ����·����
	/////////////////////////////////////////////////////////////////////////////////
	WString& TransPathName(WString& out, const WString& in);

	// �ļ����
	/////////////////////////////////////////////////////////////////////////////////
	class Pkt
	{
	public:
		typedef DirTable::ChildIterator FileIterator;

		static void Encode(VFile& Out, VFile& In);

		void Pack(const WString& dirName, VFile& pktFile);

		Pkt();
	};


	// ����ļ���ȡ
	/////////////////////////////////////////////////////////////////////////////////
	class UnPkt
	{
	public:
		typedef DirTable::ChildIterator FileIterator;

		static void Decode(VFile& Out, VFile& In);

		void Open(const VFile& pktFile);
		void Close();
		
		void Dir(const WString& dirName);
		void LocateFile(const WString& pathName);
		void LocateFile(FileIterator iter);
		
		size_t CurFileSize() const;
		size_t CurFileCompressedSize() const;

		bool ReadCurFile(void* data);
		void ReadCurFileCompressed(void* data);

		FileIterator BeginFile();
		FileIterator EndFile();

		UnPkt();
		~UnPkt();

	private:
		VFilePtr	file_;

		DirTable	dirTable_;
		DirTable*	curDir_;
		FileDes*	curFile_;

		PktHeader	mag_;

		UnPkt(const UnPkt& rhs);
		UnPkt& operator=(const UnPkt& rhs);
	};
}

#endif		// _UNPKT_HPP