// Pkt.hpp
// KlayGE ����ļ���ȡ�� ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
// LZSSѹ���㷨�������� Haruhiko Okumura
//
// 2.2.0
// ͳһʹ��istream��Ϊ��Դ��ʾ�� (2004.10.26)
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

#include <KlayGE/MapVector.hpp>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

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
	std::string& TransPathName(std::string& out, std::string const & in);

	// �ļ����
	/////////////////////////////////////////////////////////////////////////////////
	class Pkt : boost::noncopyable
	{
	public:
		Pkt();

		static void Encode(std::ostream& out, std::istream& in);

		void Pack(std::string const & dirName, std::ostream& pktFile);
	};


	// ����ļ���ȡ
	/////////////////////////////////////////////////////////////////////////////////
	class UnPkt : boost::noncopyable
	{
	public:
		UnPkt();

		static void UnPkt::Decode(std::ostream& out, std::istream& in);

		void Open(boost::shared_ptr<std::istream> const & pktFile);
		
		void LocateFile(std::string const & pathName);

		size_t CurFileSize() const;
		size_t CurFileCompressedSize() const;

		bool ReadCurFile(void* data);
		void ReadCurFileCompressed(void* data);

	private:
		boost::shared_ptr<std::istream>	file_;

		DirTable	dirTable_;
		DirTable::iterator	curFile_;

		PktHeader	mag_;
	};
}

#endif		// _UNPKT_HPP
