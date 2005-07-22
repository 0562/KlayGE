// D3D9RenderTexture.hpp
// KlayGE D3D9��Ⱦ������ ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ȥ����OnLostDevice��OnResetDevice������texture���� (2005.3.3)
//
// 2.3.0
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERTEXTURE_HPP
#define _D3D9RENDERTEXTURE_HPP

#include <KlayGE/RenderTexture.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9RenderTexture : public RenderTexture, public D3D9Resource
	{
	public:
		D3D9RenderTexture();

		void AttachTexture2D(TexturePtr texture2D);
		void AttachTextureCube(TexturePtr textureCube, Texture::CubeFaces face);
		void DetachTexture();

		boost::shared_ptr<IDirect3DSurface9> D3DRenderSurface() const;
		boost::shared_ptr<IDirect3DSurface9> D3DRenderZBuffer() const;

		void CustomAttribute(std::string const & name, void* pData);

		bool RequiresTextureFlipping() const
			{ return true; }

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		boost::shared_ptr<IDirect3DSurface9> renderSurface_;
		boost::shared_ptr<IDirect3DSurface9> depthStencilSurface_;
	};

	typedef boost::shared_ptr<D3D9RenderTexture> D3D9RenderTexturePtr;
}

#endif			// _D3D9RENDERTEXTURE_HPP
