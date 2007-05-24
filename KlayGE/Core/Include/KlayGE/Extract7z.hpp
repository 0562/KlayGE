// Extract7z.hpp
// KlayGE ���ϵͳ7z��ȡ�� ͷ�ļ� ����7zip
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _EXTRACT7Z_HPP
#define _EXTRACT7Z_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>

#include <string>

namespace KlayGE
{
	void Extract7z(boost::shared_ptr<std::istream> const & archive_is,
		std::string const & password,
		std::string const & extractFilePath,
		boost::shared_ptr<std::ostream> const & os);
}

#endif		// _EXTRACT7Z_HPP
