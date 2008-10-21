// OGLRenderFactory.h
// KlayGE OpenGL��Ⱦ������ ͷ�ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// ֧��vertex_buffer_object (2005.6.19)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERFACTORY_HPP
#define _OGLRENDERFACTORY_HPP

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4251 4275 4512 4702)
#endif
#include <boost/program_options.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_OGL_RE_SOURCE					// Build dll
		#define KLAYGE_OGL_RE_API __declspec(dllexport)
	#else										// Use dll
		#define KLAYGE_OGL_RE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_OGL_RE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_OGL_RE_API void MakeRenderFactory(KlayGE::RenderFactoryPtr& ptr, boost::program_options::variables_map const & vm);
	KLAYGE_OGL_RE_API bool Match(char const * name, char const * compiler);
}

#endif			// _OGLRENDERFACTORY_HPP
