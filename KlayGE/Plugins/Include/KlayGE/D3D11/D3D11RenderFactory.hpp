// D3D11RenderFactory.hpp
// KlayGE D3D11��Ⱦ������󹤳� ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2009
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2009.1.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11RENDERFACTORY_HPP
#define _D3D11RENDERFACTORY_HPP

#pragma once

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4251 4275 4512 4702)
#endif
#include <boost/program_options.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_D3D11_RE_SOURCE				// Build dll
		#define KLAYGE_D3D11_RE_API __declspec(dllexport)
	#else										// Use dll
		#define KLAYGE_D3D11_RE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_D3D11_RE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_D3D11_RE_API void MakeRenderFactory(KlayGE::RenderFactoryPtr& ptr, boost::program_options::variables_map const & vm);
}

#endif			// _D3D11RENDERFACTORY_HPP
