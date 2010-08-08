// DInputFactory.hpp
// KlayGE DirectInput����������󹤳� ͷ�ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://www.klayge.org
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

#pragma once

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_DINPUT_IE_SOURCE			// Build dll
		#define KLAYGE_DINPUT_IE_API __declspec(dllexport)
	#else									// Use dll
		#define KLAYGE_DINPUT_IE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_DINPUT_IE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_DINPUT_IE_API void MakeInputFactory(KlayGE::InputFactoryPtr& ptr);
}

#endif			// _DINPUTFACTORY_HPP
