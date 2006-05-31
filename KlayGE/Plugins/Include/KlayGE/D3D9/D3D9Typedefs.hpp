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

#define NOMINMAX
#include <d3d9.h>

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
	typedef boost::shared_ptr<IDirect3DSurface9>		ID3D9SurfacePtr;
}

#endif		// _D3D9TYPEDEFS_HPP
