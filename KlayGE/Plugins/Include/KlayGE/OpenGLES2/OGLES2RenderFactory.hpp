// OGLES2RenderFactory.h
// KlayGE OpenGL ES 2��Ⱦ������ ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// ���ν��� (2010.1.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLES2RENDERFACTORY_HPP
#define _OGLES2RENDERFACTORY_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_OGL_RE_SOURCE					// Build dll
		#define KLAYGE_OGLES2_RE_API __declspec(dllexport)
	#else										// Use dll
		#define KLAYGE_OGLES2_RE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_OGLES2_RE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_OGLES2_RE_API void MakeRenderFactory(KlayGE::RenderFactoryPtr& ptr, KlayGE::XMLNodePtr const & extra_param);
}

#endif			// _OGLRENDERFACTORY_HPP
