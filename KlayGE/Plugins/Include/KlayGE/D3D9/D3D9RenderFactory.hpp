// D3D9RenderFactory.hpp
// KlayGE D3D9��Ⱦ������󹤳� ͷ�ļ�
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

#ifndef _D3D9RENDERFACTORY_HPP
#define _D3D9RENDERFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderBuffer.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	RenderFactory& D3D9RenderFactoryInstance();
}

#endif			// _D3D9RENDERFACTORY_HPP
