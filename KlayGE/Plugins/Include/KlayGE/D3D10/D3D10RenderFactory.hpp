// D3D10RenderFactory.hpp
// KlayGE D3D10��Ⱦ������󹤳� ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2008.9.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D10RENDERFACTORY_HPP
#define _D3D10RENDERFACTORY_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_D3D10_RE_SOURCE				// Build dll
		#define KLAYGE_D3D10_RE_API __declspec(dllexport)
	#else										// Use dll
		#define KLAYGE_D3D10_RE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_D3D10_RE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_D3D10_RE_API void MakeRenderFactory(KlayGE::RenderFactoryPtr& ptr, KlayGE::XMLNodePtr const & extra_param);
}

#endif			// _D3D10RENDERFACTORY_HPP
