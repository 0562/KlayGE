// PackedFile.hpp
// KlayGE ����ļ��� ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// �Ѵ���������󵽻��� (2004.10.21)
//
// 2.0.0
// ���ν��� (2003.8.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _PACKEDFILE_HPP
#define _PACKEDFILE_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/VFile.hpp>
#include <KlayGE/PackedFile/Pkt.hpp>
#include <KlayGE/ResLocator.hpp>

#include <boost/utility.hpp>

#pragma comment(lib, "KlayGE_FileSystem_PackedFile.lib")

namespace KlayGE
{
	// �Ѵ���ļ�����ͨ�ļ��ж�ȡ��������
	//		ֻ�ܶ�ȡ
	/////////////////////////////////////////////////////////////////////////////////
 	class PackedFile : boost::noncopyable, public VFile
	{
	public:
		PackedFile(std::string const & pathName);

		size_t Write(void const * /*data*/, size_t /*count*/)
			{ return 0; }
		size_t Read(void* data, size_t count);

		void OnSeek(size_t offset, std::ios_base::seekdir from);

	private:
		UnPkt		unPkt_;
		VFilePtr	pktFile_;
	};

	class PackedFileResIdentifier : public ResIdentifier
	{
	public:
		PackedFileResIdentifier(std::string const & fileName);

		VFilePtr Load();

	private:
		std::string fileName_;
	};
}

#endif			// _PACKEDFILE_HPP
