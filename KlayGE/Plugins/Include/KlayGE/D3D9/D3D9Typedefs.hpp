// D3D9Typedefs.hpp
// KlayGE һЩd3d9��ص�typedef ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ���ν��� (2006.5.31)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9TYPEDEFS_HPP
#define _D3D9TYPEDEFS_HPP

#include <d3d9.h>
#include <d3dx9.h>

namespace KlayGE
{
	typedef boost::shared_ptr<IDirect3D9>				ID3D9Ptr;
	typedef boost::shared_ptr<IDirect3DDevice9>			ID3D9DevicePtr;
	typedef boost::shared_ptr<IDirect3DTexture9>		ID3D9TexturePtr;
	typedef boost::shared_ptr<IDirect3DVolumeTexture9>	ID3D9VolumeTexturePtr;
	typedef boost::shared_ptr<IDirect3DCubeTexture9>	ID3D9CubeTexturePtr;
	typedef boost::shared_ptr<IDirect3DBaseTexture9>	ID3D9BaseTexturePtr;
	typedef boost::shared_ptr<IDirect3DSurface9>		ID3D9SurfacePtr;
	typedef boost::shared_ptr<IDirect3DVolume9>			ID3D9VolumePtr;
	typedef boost::shared_ptr<IDirect3DIndexBuffer9>	ID3D9IndexBufferPtr;
	typedef boost::shared_ptr<IDirect3DVertexBuffer9>	ID3D9VertexBufferPtr;
	typedef boost::shared_ptr<IDirect3DVertexDeclaration9> ID3D9VertexDeclarationPtr;
	typedef boost::shared_ptr<IDirect3DQuery9>			ID3D9QueryPtr;
	typedef boost::shared_ptr<IDirect3DSwapChain9>		ID3D9SwapChainPtr;
	typedef boost::shared_ptr<ID3DXEffect>				ID3DXEffectPtr;
	typedef boost::shared_ptr<ID3DXConstantTable>		ID3DXConstantTablePtr;
	typedef boost::shared_ptr<IDirect3DVertexShader9>	ID3D9VertexShaderPtr;
	typedef boost::shared_ptr<IDirect3DPixelShader9>	ID3D9PixelShaderPtr;
}

#endif		// _D3D9TYPEDEFS_HPP
