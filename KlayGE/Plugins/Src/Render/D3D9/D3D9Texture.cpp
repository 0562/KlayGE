// D3D9Texture.cpp
// KlayGE D3D9������ ʵ���ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// ���Զ�ȡRenderTarget (2005.6.18)
// �����˿�����RenderTarget (2005.6.22)
// ������AddressingMode, Filtering��Anisotropy (2005.6.27)
// ������MaxMipLevel��MipMapLodBias (2005.6.28)
//
// 2.6.0
// �����˶�surface�ļ�� (2005.5.15)
//
// 2.4.0
// ������1D/2D/3D/cube��֧�� (2005.3.8)
//
// 2.3.0
// �����˶Ը��������ʽ��֧�� (2005.1.25)
// �Ľ���CopyMemoryToTexture (2005.2.1)
// ������CopyToMemory (2005.2.6)
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// 2.0.5
// ����GenerateMipSubLevels������mipmap (2004.4.8)
//
// 2.0.4
// �����˵�Դ��Ŀ���ʽ��ͬʱCopyMemoryToTexture�����Bug (2004.3.19)
//
// 2.0.0
// ���ν��� (2003.8.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/Util.hpp>

#include <cstring>

#include <d3dx9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <KlayGE/D3D9/D3D9Texture.hpp>

#pragma comment(lib, "d3d9.lib")
#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "d3dx9d.lib")
#else
	#pragma comment(lib, "d3dx9.lib")
#endif

namespace
{
	using namespace KlayGE;

	D3DFORMAT ConvertFormat(KlayGE::PixelFormat format)
	{
		switch (format)
		{
		case PF_L8:
			return D3DFMT_L8;

		case PF_A8:
			return D3DFMT_A8;

		case PF_A4L4:
			return D3DFMT_A4L4;

		case PF_A8L8:
			return D3DFMT_A8L8;

		case PF_R5G6B5:
			return D3DFMT_R5G6B5;

		case PF_A4R4G4B4:
			return D3DFMT_A4R4G4B4;

		case PF_X8R8G8B8:
			return D3DFMT_X8R8G8B8;

		case PF_A8R8G8B8:
			return D3DFMT_A8R8G8B8;

		case PF_A2R10G10B10:
			return D3DFMT_A2B10G10R10;

		case PF_R16F:
			return D3DFMT_R16F;

		case PF_G16R16F:
			return D3DFMT_G16R16F;
		case PF_A16B16G16R16F:
			return D3DFMT_A16B16G16R16F;

		case PF_R32F:
			return D3DFMT_R32F;

		case PF_G32R32F:
			return D3DFMT_G32R32F;

		case PF_A32B32G32R32F:
			return D3DFMT_A32B32G32R32F;

		case PF_DXT1:
			return D3DFMT_DXT1;

		case PF_DXT3:
			return D3DFMT_DXT3;

		case PF_DXT5:
			return D3DFMT_DXT5;

		case PF_D16:
			return D3DFMT_D16;

		case PF_D24X8:
			return D3DFMT_D24X8;

		case PF_D24S8:
			return D3DFMT_D24S8;

		default:
			BOOST_ASSERT(false);
			return D3DFMT_UNKNOWN;
		}
	}

	KlayGE::PixelFormat ConvertFormat(D3DFORMAT format)
	{
		switch (format)
		{
		case D3DFMT_L8:
			return PF_L8;

		case D3DFMT_A8:
			return PF_A8;

		case D3DFMT_A4L4:
			return PF_A4L4;

		case D3DFMT_A8L8:
			return PF_A8L8;

		case D3DFMT_R5G6B5:
			return PF_R5G6B5;

		case D3DFMT_A4R4G4B4:
			return PF_A4R4G4B4;

		case D3DFMT_X8R8G8B8:
			return PF_X8R8G8B8;

		case D3DFMT_A8R8G8B8:
			return PF_A8R8G8B8;

		case D3DFMT_A2B10G10R10:
			return PF_A2R10G10B10;

		case D3DFMT_R16F:
			return PF_R16F;

		case D3DFMT_G16R16F:
			return PF_G16R16F;

		case D3DFMT_A16B16G16R16F:
			return PF_A16B16G16R16F;

		case D3DFMT_R32F:
			return PF_R32F;

		case D3DFMT_G32R32F:
			return PF_G32R32F;

		case D3DFMT_A32B32G32R32F:
			return PF_A32B32G32R32F;

		case D3DFMT_DXT1:
			return PF_DXT1;

		case D3DFMT_DXT3:
			return PF_DXT3;

		case D3DFMT_DXT5:
			return PF_DXT5;

		case D3DFMT_D16:
			return PF_D16;

		case D3DFMT_D24X8:
			return PF_D24X8;

		case D3DFMT_D24S8:
			return PF_D24S8;

		default:
			BOOST_ASSERT(false);
			return PF_Unknown;
		}
	}
}

namespace KlayGE
{
	D3D9Texture::D3D9Texture(uint32_t width, uint16_t numMipMaps, PixelFormat format, TextureUsage usage)
					: Texture(usage, TT_1D)
	{
		BOOST_ASSERT(TU_Default == usage);
		BOOST_ASSERT(dynamic_cast<D3D9RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()) != NULL);

		this->InitParams();

		D3D9RenderEngine& renderEngine(static_cast<D3D9RenderEngine&>(Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3dDevice_ = renderEngine.D3DDevice();

		numMipMaps_ = numMipMaps;
		format_		= format;
		width_		= width;
		height_		= 1;
		depth_		= 1;

		bpp_ = PixelFormatBits(format);

		d3dTexture2D_ = this->CreateTexture2D(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);

		this->QueryBaseTexture();
		this->UpdateParams();
	}

	D3D9Texture::D3D9Texture(uint32_t width, uint32_t height,
								uint16_t numMipMaps, PixelFormat format, TextureUsage usage)
					: Texture(usage, TT_2D)
	{
		BOOST_ASSERT(dynamic_cast<D3D9RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()) != NULL);

		this->InitParams();

		D3D9RenderEngine& renderEngine(static_cast<D3D9RenderEngine&>(Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3dDevice_ = renderEngine.D3DDevice();

		numMipMaps_ = numMipMaps;
		format_		= format;
		width_		= width;
		height_		= height;
		depth_		= 1;

		bpp_ = PixelFormatBits(format);

		if (TU_Default == usage_)
		{
			d3dTexture2D_ = this->CreateTexture2D(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);

			this->QueryBaseTexture();
			this->UpdateParams();
		}
		else
		{
			d3dTexture2D_ = this->CreateTexture2D(D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);

			this->QueryBaseTexture();
			this->UpdateParams();

			this->CreateDepthStencilBuffer();
		}
	}

	D3D9Texture::D3D9Texture(uint32_t width, uint32_t height, uint32_t depth,
								uint16_t numMipMaps, PixelFormat format, TextureUsage usage)
					: Texture(usage, TT_3D)
	{
		BOOST_ASSERT(TU_Default == usage);
		BOOST_ASSERT(dynamic_cast<D3D9RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()) != NULL);

		this->InitParams();

		D3D9RenderEngine& renderEngine(static_cast<D3D9RenderEngine&>(Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3dDevice_ = renderEngine.D3DDevice();

		numMipMaps_ = numMipMaps;
		format_		= format;
		width_		= width;
		height_		= height;
		depth_		= depth;

		bpp_ = PixelFormatBits(format);

		d3dTexture3D_ = this->CreateTexture3D(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);

		this->QueryBaseTexture();
		this->UpdateParams();
	}

	D3D9Texture::D3D9Texture(uint32_t size, bool /*cube*/, uint16_t numMipMaps, PixelFormat format, TextureUsage usage)
					: Texture(usage, TT_Cube)
	{
		BOOST_ASSERT(dynamic_cast<D3D9RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()) != NULL);

		this->InitParams();

		D3D9RenderEngine& renderEngine(static_cast<D3D9RenderEngine&>(Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3dDevice_ = renderEngine.D3DDevice();

		numMipMaps_ = numMipMaps;
		format_		= format;
		width_		= size;
		height_		= size;
		depth_		= 1;

		bpp_ = PixelFormatBits(format);

		if (TU_Default == usage_)
		{
			d3dTextureCube_ = this->CreateTextureCube(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);

			this->QueryBaseTexture();
			this->UpdateParams();
		}
		else
		{
			d3dTextureCube_ = this->CreateTextureCube(D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);

			this->QueryBaseTexture();
			this->UpdateParams();

			this->CreateDepthStencilBuffer();
		}
	}

	D3D9Texture::~D3D9Texture()
	{
	}

	void D3D9Texture::InitParams()
	{
		addr_mode_u_ = TAM_Wrap;
		addr_mode_v_ = TAM_Wrap;
		addr_mode_w_ = TAM_Wrap;
		
		min_filter_ = TFO_Point;
		mag_filter_ = TFO_Point;
		mip_filter_ = TFO_Point;

		anisotropy_ = 0;

		max_mip_level_ = 0;
		mip_map_lod_bias_ = 0;
	}

	std::wstring const & D3D9Texture::Name() const
	{
		static const std::wstring name(L"Direct3D9 Texture");
		return name;
	}

	uint32_t D3D9Texture::Width(int level) const
	{
		BOOST_ASSERT(level < numMipMaps_);

		return widths_[level];
	}

	uint32_t D3D9Texture::Height(int level) const
	{
		BOOST_ASSERT(level < numMipMaps_);

		return heights_[level];
	}

	uint32_t D3D9Texture::Depth(int level) const
	{
		BOOST_ASSERT(level < numMipMaps_);

		return depths_[level];
	}

	void D3D9Texture::CopyToTexture(Texture& target)
	{
		D3D9Texture& other(static_cast<D3D9Texture&>(target));

		BOOST_ASSERT(target.Depth(0) == depth_);
		BOOST_ASSERT(target.Type() == type_);

		uint32_t maxLevel = 1;
		if (this->NumMipMaps() == target.NumMipMaps())
		{
			maxLevel = this->NumMipMaps();
		}

		switch (type_)
		{
		case TT_1D:
		case TT_2D:
			{
				IDirect3DSurface9Ptr src, dst;

				for (uint32_t level = 0; level < maxLevel; ++ level)
				{
					IDirect3DSurface9* temp;

					TIF(d3dTexture2D_->GetSurfaceLevel(level, &temp));
					src = MakeCOMPtr(temp);

					TIF(other.d3dTexture2D_->GetSurfaceLevel(level, &temp));
					dst = MakeCOMPtr(temp);

					if (Texture::TU_RenderTarget == target.Usage())
					{
						TIF(d3dDevice_->StretchRect(src.get(), NULL, dst.get(), NULL, D3DTEXF_LINEAR));
					}
					else
					{
						TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_LINEAR, 0));
					}
				}
			}
			break;

		case TT_3D:
			{
				IDirect3DVolume9Ptr src, dst;

				for (uint32_t level = 0; level < maxLevel; ++ level)
				{
					IDirect3DVolume9* temp;

					TIF(d3dTexture3D_->GetVolumeLevel(level, &temp));
					src = MakeCOMPtr(temp);

					TIF(other.d3dTexture3D_->GetVolumeLevel(level, &temp));
					dst = MakeCOMPtr(temp);

					TIF(D3DXLoadVolumeFromVolume(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_DEFAULT, 0));
				}
			}
			break;

		case TT_Cube:
			{
				IDirect3DSurface9Ptr src, dst;

				for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
				{
					for (uint32_t level = 0; level < maxLevel; ++ level)
					{
						IDirect3DSurface9* temp;

						TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
						src = MakeCOMPtr(temp);

						TIF(other.d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
						dst = MakeCOMPtr(temp);

						if (Texture::TU_RenderTarget == target.Usage())
						{
							TIF(d3dDevice_->StretchRect(src.get(), NULL, dst.get(), NULL, D3DTEXF_LINEAR));
						}
						else
						{
							TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_LINEAR, 0));
						}
					}
				}
			}
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		if (this->NumMipMaps() != target.NumMipMaps())
		{
			target.BuildMipSubLevels();
		}		
	}

	void D3D9Texture::CopySurfaceToMemory(boost::shared_ptr<IDirect3DSurface9> const & surface, void* data)
	{
		D3DLOCKED_RECT d3d_rc;
		TIF(surface->LockRect(&d3d_rc, NULL, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY));

		uint8_t* dst = static_cast<uint8_t*>(data);
		uint8_t* src = static_cast<uint8_t*>(d3d_rc.pBits);

		D3DSURFACE_DESC desc;
		surface->GetDesc(&desc);

		uint32_t const srcPitch = d3d_rc.Pitch;
		uint32_t const dstPitch = desc.Width * bpp_ / 8;

		for (uint32_t i = 0; i < desc.Height; ++ i)
		{
			std::copy(src, src + dstPitch, dst);

			src += srcPitch;
			dst += dstPitch;
		}

		surface->UnlockRect();
	}

	void D3D9Texture::CopyToMemory1D(int level, void* data)
	{
		BOOST_ASSERT(data != NULL);

		this->CopyToMemory2D(level, data);
	}

	void D3D9Texture::CopyToMemory2D(int level, void* data)
	{
		BOOST_ASSERT(data != NULL);

		boost::shared_ptr<IDirect3DSurface9> surface;
		{
			IDirect3DSurface9* tmp_surface;
			TIF(d3dTexture2D_->GetSurfaceLevel(level, &tmp_surface));
			surface = MakeCOMPtr(tmp_surface);
		}
		if (TU_RenderTarget == usage_)
		{
			IDirect3DSurface9* tmp_surface;
			TIF(d3dDevice_->CreateOffscreenPlainSurface(this->Width(level), this->Height(level),
				ConvertFormat(format_), D3DPOOL_SYSTEMMEM, &tmp_surface, NULL));

			TIF(D3DXLoadSurfaceFromSurface(tmp_surface, NULL, NULL, surface.get(), NULL, NULL, D3DX_FILTER_NONE, 0));

			surface = MakeCOMPtr(tmp_surface);
		}

		this->CopySurfaceToMemory(surface, data);
	}

	void D3D9Texture::CopyToMemory3D(int level, void* data)
	{
		BOOST_ASSERT(data != NULL);

		D3DLOCKED_BOX d3d_box;
		d3dTexture3D_->LockBox(level, &d3d_box, NULL, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);

		uint8_t* dst = static_cast<uint8_t*>(data);
		uint8_t* src = static_cast<uint8_t*>(d3d_box.pBits);

		uint32_t const srcPitch = d3d_box.RowPitch;
		uint32_t const dstPitch = this->Width(level) * bpp_ / 8;

		for (uint32_t j = 0; j < this->Depth(level); ++ j)
		{
			src = static_cast<uint8_t*>(d3d_box.pBits) + j * d3d_box.SlicePitch;

			for (uint32_t i = 0; i < this->Height(level); ++ i)
			{
				std::copy(src, src + dstPitch, dst);

				src += srcPitch;
				dst += dstPitch;
			}
		}

		d3dTexture3D_->UnlockBox(0);
	}

	void D3D9Texture::CopyToMemoryCube(CubeFaces face, int level, void* data)
	{
		BOOST_ASSERT(data != NULL);

		boost::shared_ptr<IDirect3DSurface9> surface;
		{
			IDirect3DSurface9* tmp_surface;
			d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &tmp_surface);
			surface = MakeCOMPtr(tmp_surface);
		}
		if (TU_RenderTarget == usage_)
		{
			IDirect3DSurface9* tmp_surface;
			d3dDevice_->CreateOffscreenPlainSurface(this->Width(level), this->Height(level),
				ConvertFormat(format_), D3DPOOL_SYSTEMMEM, &tmp_surface, NULL);

			TIF(D3DXLoadSurfaceFromSurface(tmp_surface, NULL, NULL, surface.get(), NULL, NULL, D3DX_FILTER_NONE, 0));

			surface = MakeCOMPtr(tmp_surface);
		}

		this->CopySurfaceToMemory(surface, data);
	}

	void D3D9Texture::CopyMemoryToTexture1D(int level, void* data, PixelFormat pf,
		uint32_t width, uint32_t xOffset)
	{
		this->CopyMemoryToTexture2D(level, data, pf, width, 1, xOffset, 0);
	}

	void D3D9Texture::CopyMemoryToTexture2D(int level, void* data, PixelFormat pf,
		uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset)
	{
		BOOST_ASSERT((TT_1D == type_) || (TT_2D == type_));

		IDirect3DSurface9* temp;
		TIF(d3dTexture2D_->GetSurfaceLevel(level, &temp));
		IDirect3DSurface9Ptr surface = MakeCOMPtr(temp);

		if (surface)
		{
			RECT srcRc = { 0, 0, width, height };
			RECT dstRc = { xOffset, yOffset, xOffset + srcRc.right, yOffset + srcRc.bottom };
			TIF(D3DXLoadSurfaceFromMemory(surface.get(), NULL, &dstRc, data, ConvertFormat(pf),
					width * PixelFormatBits(pf) / 8, NULL, &srcRc, D3DX_FILTER_NONE, 0));
		}
	}

	void D3D9Texture::CopyMemoryToTexture3D(int level, void* data, PixelFormat pf,
			uint32_t width, uint32_t height, uint32_t depth,
			uint32_t xOffset, uint32_t yOffset, uint32_t zOffset)
	{
		BOOST_ASSERT(TT_3D == type_);

		IDirect3DVolume9* temp;
		TIF(d3dTexture3D_->GetVolumeLevel(level, &temp));
		IDirect3DVolume9Ptr volume = MakeCOMPtr(temp);

		if (volume)
		{
			uint32_t const srcRowPitch = width * PixelFormatBits(pf) / 8;
			uint32_t const srcSlicePitch = srcRowPitch * height;

			D3DBOX srcBox = { 0, 0, width, height, 0, depth };
			D3DBOX dstBox = { xOffset, yOffset, xOffset + srcBox.Right, yOffset + srcBox.Bottom,
				zOffset, zOffset + srcBox.Back };
			TIF(D3DXLoadVolumeFromMemory(volume.get(), NULL, &dstBox, data, ConvertFormat(pf),
					srcRowPitch, srcSlicePitch, NULL, &srcBox, D3DX_FILTER_NONE, 0));
		}
	}

	void D3D9Texture::CopyMemoryToTextureCube(CubeFaces face, int level, void* data, PixelFormat pf,
			uint32_t size, uint32_t xOffset)
	{
		BOOST_ASSERT(TT_Cube == type_);

		IDirect3DSurface9* temp;
		TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
		IDirect3DSurface9Ptr surface = MakeCOMPtr(temp);

		if (surface)
		{
			RECT srcRc = { 0, 0, size, size };
			RECT dstRc = { xOffset, xOffset, xOffset + srcRc.right, xOffset + srcRc.bottom };
			TIF(D3DXLoadSurfaceFromMemory(surface.get(), NULL, &dstRc, data, ConvertFormat(pf),
					size * PixelFormatBits(pf) / 8, NULL, &srcRc, D3DX_FILTER_NONE, 0));
		}
	}

	void D3D9Texture::BuildMipSubLevels()
	{
		IDirect3DBaseTexture9Ptr d3dBaseTexture;

		switch (type_)
		{
		case TT_1D:
		case TT_2D:
			{
				IDirect3DTexture9Ptr d3dTexture2D = this->CreateTexture2D(0, D3DPOOL_SYSTEMMEM);

				IDirect3DBaseTexture9* base;
				d3dTexture2D->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&base));
				d3dBaseTexture = MakeCOMPtr(base);

				IDirect3DSurface9* temp;
				TIF(d3dTexture2D_->GetSurfaceLevel(0, &temp));
				IDirect3DSurface9Ptr src = MakeCOMPtr(temp);

				TIF(d3dTexture2D->GetSurfaceLevel(0, &temp));
				IDirect3DSurface9Ptr dst = MakeCOMPtr(temp);

				TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
			}
			break;

		case TT_3D:
			{
				IDirect3DVolumeTexture9Ptr d3dTexture3D = this->CreateTexture3D(0, D3DPOOL_SYSTEMMEM);

				IDirect3DBaseTexture9* base;
				d3dTexture3D->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&base));
				d3dBaseTexture = MakeCOMPtr(base);

				IDirect3DVolume9* temp;
				TIF(d3dTexture3D_->GetVolumeLevel(0, &temp));
				IDirect3DVolume9Ptr src = MakeCOMPtr(temp);

				TIF(d3dTexture3D->GetVolumeLevel(0, &temp));
				IDirect3DVolume9Ptr dst = MakeCOMPtr(temp);

				TIF(D3DXLoadVolumeFromVolume(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
			}
			break;

		case TT_Cube:
			{
				IDirect3DCubeTexture9Ptr d3dTextureCube = this->CreateTextureCube(0, D3DPOOL_SYSTEMMEM);

				IDirect3DBaseTexture9* base;
				d3dTextureCube->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&base));
				d3dBaseTexture = MakeCOMPtr(base);

				for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
				{
					IDirect3DSurface9* temp;
					TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), 0, &temp));
					IDirect3DSurface9Ptr src = MakeCOMPtr(temp);

					TIF(d3dTextureCube->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), 0, &temp));
					IDirect3DSurface9Ptr dst = MakeCOMPtr(temp);

					TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
				}
			}
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		TIF(D3DXFilterTexture(d3dBaseTexture.get(), NULL, D3DX_DEFAULT, D3DX_DEFAULT));
		TIF(d3dDevice_->UpdateTexture(d3dBaseTexture.get(), d3dBaseTexture_.get()));
	}

	void D3D9Texture::CustomAttribute(std::string const & /*name*/, void* /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void D3D9Texture::DoOnLostDevice()
	{
		d3dDevice_ = static_cast<D3D9RenderEngine const &>(Context::Instance().RenderFactoryInstance().RenderEngineInstance()).D3DDevice();

		switch (type_)
		{
		case TT_1D:
		case TT_2D:
			{
				IDirect3DTexture9Ptr tempTexture2D = this->CreateTexture2D(0, D3DPOOL_SYSTEMMEM);

				for (uint16_t i = 0; i < this->NumMipMaps(); ++ i)
				{
					IDirect3DSurface9* temp;
					TIF(d3dTexture2D_->GetSurfaceLevel(i, &temp));
					IDirect3DSurface9Ptr src = MakeCOMPtr(temp);

					TIF(tempTexture2D->GetSurfaceLevel(i, &temp));
					IDirect3DSurface9Ptr dst = MakeCOMPtr(temp);

					TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
				}
				tempTexture2D->AddDirtyRect(NULL);
				d3dTexture2D_ = tempTexture2D;

				renderZBuffer_.reset();
			}
			break;

		case TT_3D:
			{
				IDirect3DVolumeTexture9Ptr tempTexture3D = this->CreateTexture3D(0, D3DPOOL_SYSTEMMEM);

				for (uint16_t i = 0; i < this->NumMipMaps(); ++ i)
				{
					IDirect3DVolume9* temp;
					TIF(d3dTexture3D_->GetVolumeLevel(i, &temp));
					IDirect3DVolume9Ptr src = MakeCOMPtr(temp);

					TIF(tempTexture3D->GetVolumeLevel(i, &temp));
					IDirect3DVolume9Ptr dst = MakeCOMPtr(temp);

					TIF(D3DXLoadVolumeFromVolume(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
				}
				tempTexture3D->AddDirtyBox(NULL);
				d3dTexture3D_ = tempTexture3D;
			}
			break;

		case TT_Cube:
			{
				IDirect3DCubeTexture9Ptr tempTextureCube = this->CreateTextureCube(0, D3DPOOL_SYSTEMMEM);

				for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
				{
					for (uint16_t level = 0; level < this->NumMipMaps(); ++ level)
					{
						IDirect3DSurface9* temp;
						TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
						IDirect3DSurface9Ptr src = MakeCOMPtr(temp);

						TIF(tempTextureCube->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
						IDirect3DSurface9Ptr dst = MakeCOMPtr(temp);

						TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));

						tempTextureCube->AddDirtyRect(static_cast<D3DCUBEMAP_FACES>(face), NULL);
					}
				}
				d3dTextureCube_ = tempTextureCube;

				renderZBuffer_.reset();
			}
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		this->QueryBaseTexture();
	}

	void D3D9Texture::DoOnResetDevice()
	{
		d3dDevice_ = static_cast<D3D9RenderEngine const &>(Context::Instance().RenderFactoryInstance().RenderEngineInstance()).D3DDevice();

		switch (type_)
		{
		case TT_1D:
		case TT_2D:
			{
				IDirect3DTexture9Ptr tempTexture2D;

				if (TU_Default == usage_)
				{
					tempTexture2D = this->CreateTexture2D(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);
				}
				else
				{
					tempTexture2D = this->CreateTexture2D(D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);
					this->CreateDepthStencilBuffer();
				}

				tempTexture2D->AddDirtyRect(NULL);

				d3dDevice_->UpdateTexture(d3dTexture2D_.get(), tempTexture2D.get());
				d3dTexture2D_ = tempTexture2D;
			}
			break;

		case TT_3D:
			{
				IDirect3DVolumeTexture9Ptr tempTexture3D = this->CreateTexture3D(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);
				tempTexture3D->AddDirtyBox(NULL);

				d3dDevice_->UpdateTexture(d3dTexture3D_.get(), tempTexture3D.get());
				d3dTexture3D_ = tempTexture3D;
			}
			break;

		case TT_Cube:
			{
				IDirect3DCubeTexture9Ptr tempTextureCube;

				if (TU_Default == usage_)
				{
					tempTextureCube = this->CreateTextureCube(D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);
				}
				else
				{
					tempTextureCube = this->CreateTextureCube(D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);
					this->CreateDepthStencilBuffer();
				}

				for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
				{
					tempTextureCube->AddDirtyRect(static_cast<D3DCUBEMAP_FACES>(face), NULL);
				}

				d3dDevice_->UpdateTexture(d3dTextureCube_.get(), tempTextureCube.get());
				d3dTextureCube_ = tempTextureCube;
			}
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		this->QueryBaseTexture();
	}

	D3D9Texture::IDirect3DTexture9Ptr D3D9Texture::CreateTexture2D(uint32_t usage, D3DPOOL pool)
	{
		IDirect3DTexture9* d3dTexture2D;
		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		TIF(D3DXCreateTexture(d3dDevice_.get(), width_, height_,
			numMipMaps_, usage, ConvertFormat(format_),
			pool, &d3dTexture2D));
		return MakeCOMPtr(d3dTexture2D);
	}

	D3D9Texture::IDirect3DVolumeTexture9Ptr D3D9Texture::CreateTexture3D(uint32_t usage, D3DPOOL pool)
	{
		IDirect3DVolumeTexture9* d3dTexture3D;
		TIF(D3DXCreateVolumeTexture(d3dDevice_.get(), width_, height_, depth_,
			numMipMaps_, usage, ConvertFormat(format_),
			pool, &d3dTexture3D));
		return MakeCOMPtr(d3dTexture3D);
	}

	D3D9Texture::IDirect3DCubeTexture9Ptr D3D9Texture::CreateTextureCube(uint32_t usage, D3DPOOL pool)
	{
		IDirect3DCubeTexture9* d3dTextureCube;
		TIF(D3DXCreateCubeTexture(d3dDevice_.get(), width_, 
			numMipMaps_, usage, ConvertFormat(format_),
			pool, &d3dTextureCube));
		return MakeCOMPtr(d3dTextureCube);
	}

	void D3D9Texture::CreateDepthStencilBuffer()
	{
		IDirect3DSurface9* tempSurf;
		D3DSURFACE_DESC tempDesc;

		// Get the format of the depth stencil surface.
		d3dDevice_->GetDepthStencilSurface(&tempSurf);
		tempSurf->GetDesc(&tempDesc);
		tempSurf->Release();

		TIF(d3dDevice_->CreateDepthStencilSurface(width_, height_, tempDesc.Format, tempDesc.MultiSampleType, 0, 
			FALSE, &tempSurf, NULL));
		renderZBuffer_ = MakeCOMPtr(tempSurf);
	}

	void D3D9Texture::QueryBaseTexture()
	{
		IDirect3DBaseTexture9* d3dBaseTexture = NULL;
		switch (type_)
		{
		case TT_1D:
		case TT_2D:
            d3dTexture2D_->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&d3dBaseTexture));
			break;

		case TT_3D:
			d3dTexture3D_->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&d3dBaseTexture));
			break;

		case TT_Cube:
			d3dTextureCube_->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&d3dBaseTexture));
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		d3dBaseTexture_ = MakeCOMPtr(d3dBaseTexture);
	}

	void D3D9Texture::UpdateParams()
	{
		// Check the actual dimensions vs requested
		switch (type_)
		{
		case TT_1D:
		case TT_2D:
			{
				D3DSURFACE_DESC desc;

				numMipMaps_ = static_cast<uint16_t>(d3dTexture2D_->GetLevelCount());
				BOOST_ASSERT(numMipMaps_ != 0);

				widths_.resize(numMipMaps_);
				heights_.resize(numMipMaps_);
				depths_.resize(numMipMaps_);
				for (uint16_t level = 0; level < numMipMaps_; ++ level)
				{
					TIF(d3dTexture2D_->GetLevelDesc(level, &desc));

					widths_[level] = desc.Width;
					heights_[level] = desc.Height;
					depths_[level] = 1;
				}					

				format_ = ConvertFormat(desc.Format);
				bpp_	= PixelFormatBits(format_);
			}
			break;

		case TT_3D:
			{
				D3DVOLUME_DESC desc;

				numMipMaps_ = static_cast<uint16_t>(d3dTexture3D_->GetLevelCount());
				BOOST_ASSERT(numMipMaps_ != 0);

				widths_.resize(numMipMaps_);
				heights_.resize(numMipMaps_);
				depths_.resize(numMipMaps_);
				for (uint16_t level = 0; level < numMipMaps_; ++ level)
				{
					TIF(d3dTexture3D_->GetLevelDesc(level, &desc));

					widths_[level] = desc.Width;
					heights_[level] = desc.Height;
					depths_[level] = desc.Depth;
				}					

				format_ = ConvertFormat(desc.Format);
				bpp_	= PixelFormatBits(format_);
			}
			break;

		case TT_Cube:
			{
				D3DSURFACE_DESC desc;

				numMipMaps_ = static_cast<uint16_t>(d3dTextureCube_->GetLevelCount());
				BOOST_ASSERT(numMipMaps_ != 0);

				widths_.resize(numMipMaps_);
				heights_.resize(numMipMaps_);
				depths_.resize(numMipMaps_);
				for (uint16_t level = 0; level < numMipMaps_; ++ level)
				{
					TIF(d3dTextureCube_->GetLevelDesc(level, &desc));

					widths_[level] = desc.Width;
					heights_[level] = desc.Height;
					depths_[level] = 1;
				}

				format_ = ConvertFormat(desc.Format);
				bpp_	= PixelFormatBits(format_);
			}
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		width_	= widths_[0];
		height_	= heights_[0];
		depth_ = depths_[0];
	}

	// ��������Ѱַģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Texture::AddressingMode(TexAddressingType type, TexAddressingMode tam)
	{
		switch (type)
		{
		case TAT_Addr_U:
			addr_mode_u_ = tam;
			break;

		case TAT_Addr_V:
			addr_mode_v_ = tam;
			break;

		case TAT_Addr_W:
			addr_mode_w_ = tam;
			break;
		}
	}

	// ��ȡ����Ѱַģʽ
	/////////////////////////////////////////////////////////////////////////////////
	Texture::TexAddressingMode D3D9Texture::AddressingMode(TexAddressingType type) const
	{
		switch (type)
		{
		case TAT_Addr_U:
			return addr_mode_u_;

		case TAT_Addr_V:
			return addr_mode_v_;

		case TAT_Addr_W:
			return addr_mode_w_;

		default:
			BOOST_ASSERT(false);
			return addr_mode_u_;
		}
	}
	
	// �����������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Texture::Filtering(TexFilterType type, TexFilterOp op)
	{
		switch (type)
		{
		case TFT_Min:
			min_filter_ = op;
			break;

		case TFT_Mag:
			mag_filter_ = op;
			break;

		case TFT_Mip:
			mip_filter_ = op;
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}
	}

	// ��ȡ�������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	Texture::TexFilterOp D3D9Texture::Filtering(TexFilterType type) const
	{
		switch (type)
		{
		case TFT_Min:
			return min_filter_;

		case TFT_Mag:
			return mag_filter_;

		case TFT_Mip:
			return mip_filter_;

		default:
			BOOST_ASSERT(false);
			return min_filter_;
		}
	}

	// �����������Թ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Texture::Anisotropy(uint32_t maxAnisotropy)
	{
		anisotropy_ = maxAnisotropy;
	}

	// ��ȡ�������Թ���
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t D3D9Texture::Anisotropy() const
	{
		return anisotropy_;
	}

	// ��������mip�ȼ�
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Texture::MaxMipLevel(uint32_t level)
	{
		max_mip_level_ = level;
	}

	// ��ȡ����mip�ȼ�
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t D3D9Texture::MaxMipLevel() const
	{
		return max_mip_level_;
	}

	// ����mip mapƫ����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Texture::MipMapLodBias(float bias)
	{
		mip_map_lod_bias_ = bias;
	}

	// ��ȡmip mapƫ����
	/////////////////////////////////////////////////////////////////////////////////
	float D3D9Texture::MipMapLodBias() const
	{
		return mip_map_lod_bias_;
	}
}
