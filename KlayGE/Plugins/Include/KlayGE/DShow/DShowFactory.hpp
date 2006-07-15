// DShowFactory.hpp
// KlayGE DirectShow����������󹤳� ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ���ν��� (2006.7.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DSHOWFACTORY_HPP
#define _DSHOWFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ShowFactory.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_ShowEngine_DShow_d.lib")
#else
	#pragma comment(lib, "KlayGE_ShowEngine_DShow.lib")
#endif

namespace KlayGE
{
	ShowFactory& DShowFactoryInstance();
}

#endif			// _DSHOWFACTORY_HPP
