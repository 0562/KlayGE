// D3D9RenderFactory.cpp
// KlayGE D3D9��Ⱦ������󹤳� ʵ���ļ�
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

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/D3D9/D3D9RenderEngine.hpp>
#include <KlayGE/D3D9/D3D9Texture.hpp>
#include <KlayGE/D3D9/D3D9RenderTexture.hpp>
#include <KlayGE/D3D9/D3D9RenderEffect.hpp>
#include <KlayGE/D3D9/D3D9Font.hpp>

#include <KlayGE/D3D9/D3D9RenderFactory.hpp>

namespace KlayGE
{
	RenderFactory& D3D9RenderFactoryInstance()
	{
		static ConcreteRenderFactory<D3D9RenderEngine, D3D9Texture, D3D9RenderTexture,
			D3D9Font, D3D9RenderEffect> renderFactory(L"Direct3D9 Render Factory");
		return renderFactory;
	}
}
