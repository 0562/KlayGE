// DInputFactory.hpp
// KlayGE DirectInput����������󹤳� ͷ�ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// ��Ϊtemplateʵ�� (2004.3.4)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DINPUTFACTORY_HPP
#define _DINPUTFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/InputFactory.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_InputEngine_DInput_d.lib")
#else
	#pragma comment(lib, "KlayGE_InputEngine_DInput.lib")
#endif

namespace KlayGE
{
	InputFactory& DInputFactoryInstance();
}

#endif			// _DINPUTFACTORY_HPP
