// D3D9RenderFactory.hpp
// KlayGE D3D9��Ⱦ������󹤳� ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
//
// 2.4.0
// ������resource_pool_��Ա (2005.3.3)
//
// 2.3.0
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// 2.0.3
// ��Ϊtemplateʵ�� (2004.3.4)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERFACTORY_HPP
#define _D3D9RENDERFACTORY_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_D3D9_RE_SOURCE				// Build dll
		#define KLAYGE_D3D9_RE_API __declspec(dllexport)
	#else										// Use dll
		#define KLAYGE_D3D9_RE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_D3D9_RE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_D3D9_RE_API void MakeRenderFactory(KlayGE::RenderFactoryPtr& ptr);
}

#endif			// _D3D9RENDERFACTORY_HPP
