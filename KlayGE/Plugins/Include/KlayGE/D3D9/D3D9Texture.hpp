// D3D9Texture.hpp
// KlayGE D3D9������ ͷ�ļ�
// Ver 2.3.1
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.3.1
// ��Ϊ������D3D9Resource (2005.3.3)
//
// 2.3.0
// ������CopyToMemory (2005.2.6)
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9TEXTURE_HPP
#define _D3D9TEXTURE_HPP

#include <boost/smart_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <KlayGE/D3D9/D3D9Resource.hpp>
#include <KlayGE/D3D9/D3D9RenderEngine.hpp>

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9Texture : public Texture, public D3D9Resource
	{
	public:
		D3D9Texture(uint32_t width, uint32_t height, uint16_t numMipMaps, PixelFormat format, TextureUsage usage = TU_Default);
		~D3D9Texture();

		std::wstring const & Name() const;

		void CustomAttribute(std::string const & name, void* pData);

		void CopyToTexture(Texture& target);
		void CopyToMemory(int level, void* data);
		void CopyMemoryToTexture(int level, void* data, PixelFormat pf,
			uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset);

		void BuildMipSubLevels();

		boost::shared_ptr<IDirect3DTexture9> const & D3DTexture() const
			{ return this->d3dTexture_; }
		boost::shared_ptr<IDirect3DSurface9> const & DepthStencil() const
			{ return this->renderZBuffer_; }

		void OnLostDevice();
		void OnResetDevice();

	private:
		boost::shared_ptr<IDirect3DDevice9>		d3dDevice_;
		boost::shared_ptr<IDirect3DTexture9>	d3dTexture_;

		boost::shared_ptr<IDirect3DSurface9>	renderZBuffer_;		// The z-buffer for the render surface.

		bool reseted_;
	};

	typedef boost::shared_ptr<D3D9Texture> D3D9TexturePtr;
}

#endif			// _D3D9TEXTURE_HPP
