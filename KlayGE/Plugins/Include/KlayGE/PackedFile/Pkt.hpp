// Pkt.hpp
// KlayGE ����ļ���ȡ�� ͷ�ļ�
// Ver 2.1.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
// LZSSѹ���㷨�������� Haruhiko Okumura
//
// 2.1.0
// ����Ŀ¼��ı�ʾ�� (2004.4.14)
//
// 2.0.0
// ���ν��� (2003.9.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _PKT_HPP
#define _PKT_HPP

#include <KlayGE/VFile.hpp>
#include <KlayGE/MapVector.hpp>

#include <boost/utility.hpp>

#pragma comment(lib, "KlayGE_FileSystem_PackedFile.lib")

namespace KlayGE
{
	#ifdef _MSC_VER
		#pragma pack(push, 1)
	#endif

	struct FileDes
	{
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

	typedef MapVector<std::string, FileDes> DirTable;

	enum FileAttrib
	{
		FA_UnCompressed = 1,
	};

	// ����·����
	/////////////////////////////////////////////////////////////////////////////////
	std::string& TransPathName(std::string& out, const std::string& in);

	// �ļ����
	/////////////////////////////////////////////////////////////////////////////////
	class Pkt : boost::noncopyable
	{
	public:
		static void Encode(VFile& Out, VFile& In);

		void Pack(const std::string& dirName, VFile& pktFile);

		Pkt();
	};


	// ����ļ���ȡ
	/////////////////////////////////////////////////////////////////////////////////
	class UnPkt : boost::noncopyable
	{
	public:
		static void Decode(VFile& Out, VFile& In);

		void Open(const VFilePtr& pktFile);
		void Close();
		
		void LocateFile(const std::string& pathName);

		size_t CurFileSize() const;
		size_t CurFileCompressedSize() const;

		bool ReadCurFile(void* data);
		void ReadCurFileCompressed(void* data);

		UnPkt();
		~UnPkt();

	private:
		VFilePtr	file_;

		DirTable	dirTable_;
		DirTable::iterator	curFile_;

		PktHeader	mag_;
	};
}

#endif		// _UNPKT_HPP
