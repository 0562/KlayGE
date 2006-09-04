// D3D9TextureCube.cpp
// KlayGE D3D9 Cube������ ʵ���ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.4.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/COMPtr.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Texture.hpp>

#include <cstring>

#include <d3dx9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9RenderEngine.hpp>
#include <KlayGE/D3D9/D3D9Mapping.hpp>
#include <KlayGE/D3D9/D3D9Texture.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma comment(lib, "d3d9.lib")
#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "d3dx9d.lib")
#else
	#pragma comment(lib, "d3dx9.lib")
#endif
#endif

namespace KlayGE
{
	D3D9TextureCube::D3D9TextureCube(uint32_t size, uint16_t numMipMaps, ElementFormat format)
					: D3D9Texture(TT_Cube),
						auto_gen_mipmaps_(false)
	{
		D3D9RenderEngine& renderEngine(*checked_cast<D3D9RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
		d3dDevice_ = renderEngine.D3DDevice();

		numMipMaps_ = numMipMaps;
		format_		= format;
		widths_.assign(1, size);

		bpp_ = NumFormatBits(format);

		d3dTextureCube_ = this->CreateTextureCube(0, D3DPOOL_MANAGED);

		this->QueryBaseTexture();
		this->UpdateParams();
	}

	uint32_t D3D9TextureCube::Width(int level) const
	{
		BOOST_ASSERT(level < numMipMaps_);

		return widths_[level];
	}

	uint32_t D3D9TextureCube::Height(int level) const
	{
		return this->Width(level);
	}

	void D3D9TextureCube::CopyToTexture(Texture& target)
	{
		BOOST_ASSERT(type_ == target.Type());

		D3D9TextureCube& other(static_cast<D3D9TextureCube&>(target));

		uint32_t maxLevel = 1;
		if (this->NumMipMaps() == target.NumMipMaps())
		{
			maxLevel = this->NumMipMaps();
		}

		ID3D9SurfacePtr src, dst;

		for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
		{
			for (uint32_t level = 0; level < maxLevel; ++ level)
			{
				IDirect3DSurface9* temp;

				TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
				src = MakeCOMPtr(temp);

				TIF(other.d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
				dst = MakeCOMPtr(temp);

				if (FAILED(d3dDevice_->StretchRect(src.get(), NULL, dst.get(), NULL, D3DTEXF_LINEAR)))
				{
					TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_LINEAR, 0));
				}
			}
		}

		if (this->NumMipMaps() != target.NumMipMaps())
		{
			target.BuildMipSubLevels();
		}		
	}

	void D3D9TextureCube::CopyToMemoryCube(CubeFaces face, int level, void* data)
	{
		BOOST_ASSERT(level < numMipMaps_);
		BOOST_ASSERT(data != NULL);

		ID3D9SurfacePtr surface;
		{
			IDirect3DSurface9* tmp_surface;
			d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &tmp_surface);
			surface = MakeCOMPtr(tmp_surface);
		}
		if (TU_RenderTarget == usage_)
		{
			IDirect3DSurface9* tmp_surface;
			d3dDevice_->CreateOffscreenPlainSurface(this->Width(level), this->Height(level),
				D3D9Mapping::MappingFormat(format_), D3DPOOL_SYSTEMMEM, &tmp_surface, NULL);

			TIF(D3DXLoadSurfaceFromSurface(tmp_surface, NULL, NULL, surface.get(), NULL, NULL, D3DX_FILTER_NONE, 0));

			surface = MakeCOMPtr(tmp_surface);
		}

		this->CopySurfaceToMemory(surface, data);
	}

	void D3D9TextureCube::CopyMemoryToTextureCube(CubeFaces face, int level, void* data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height)
	{
		BOOST_ASSERT(TT_Cube == type_);

		IDirect3DSurface9* temp;
		TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), level, &temp));
		ID3D9SurfacePtr surface = MakeCOMPtr(temp);

		if (surface)
		{
			RECT srcRc = { 0, 0, src_width, src_height };
			RECT dstRc = { dst_xOffset, dst_yOffset, dst_xOffset + dst_width, dst_yOffset + dst_height };
			TIF(D3DXLoadSurfaceFromMemory(surface.get(), NULL, &dstRc, data, D3D9Mapping::MappingFormat(pf),
					src_width * NumFormatBytes(pf), NULL, &srcRc, D3DX_DEFAULT, 0));
		}
	}

	void D3D9TextureCube::BuildMipSubLevels()
	{
		ID3D9BaseTexturePtr d3dBaseTexture;

		if (auto_gen_mipmaps_)
		{
			d3dTextureCube_->GenerateMipSubLevels();
		}
		else
		{
			if (TU_RenderTarget == usage_)
			{
				ID3D9CubeTexturePtr d3dTextureCube = this->CreateTextureCube(D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);

				for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
				{
					IDirect3DSurface9* temp;
					TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), 0, &temp));
					ID3D9SurfacePtr src = MakeCOMPtr(temp);

					TIF(d3dTextureCube->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), 0, &temp));
					ID3D9SurfacePtr dst = MakeCOMPtr(temp);

					TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
				}

				d3dTextureCube->GenerateMipSubLevels();
				d3dTextureCube_ = d3dTextureCube;

				auto_gen_mipmaps_ = true;
			}
			else
			{
				ID3D9CubeTexturePtr d3dTextureCube = this->CreateTextureCube(0, D3DPOOL_SYSTEMMEM);

				IDirect3DBaseTexture9* base;
				d3dTextureCube->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&base));
				d3dBaseTexture = MakeCOMPtr(base);

				for (uint32_t face = D3DCUBEMAP_FACE_POSITIVE_X; face <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++ face)
				{
					IDirect3DSurface9* temp;
					TIF(d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), 0, &temp));
					ID3D9SurfacePtr src = MakeCOMPtr(temp);

					TIF(d3dTextureCube->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), 0, &temp));
					ID3D9SurfacePtr dst = MakeCOMPtr(temp);

					TIF(D3DXLoadSurfaceFromSurface(dst.get(), NULL, NULL, src.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
				}

				TIF(D3DXFilterTexture(d3dBaseTexture.get(), NULL, D3DX_DEFAULT, D3DX_DEFAULT));
				TIF(d3dDevice_->UpdateTexture(d3dBaseTexture.get(), d3dBaseTexture_.get()));
			}
		}
	}

	void D3D9TextureCube::DoOnLostDevice()
	{
		if (TU_RenderTarget == usage_)
		{
			d3dBaseTexture_.reset();
			d3dTextureCube_.reset();
		}
	}

	void D3D9TextureCube::DoOnResetDevice()
	{
		if (TU_RenderTarget == usage_)
		{
			d3dTextureCube_ = this->CreateTextureCube(D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);
			this->QueryBaseTexture();
		}
	}

	ID3D9CubeTexturePtr D3D9TextureCube::CreateTextureCube(uint32_t usage, D3DPOOL pool)
	{
		if (IsDepthFormat(format_))
		{
			usage |= D3DUSAGE_DEPTHSTENCIL;
		}

		IDirect3DCubeTexture9* d3dTextureCube;
		TIF(d3dDevice_->CreateCubeTexture(widths_[0],  numMipMaps_, usage,
			D3D9Mapping::MappingFormat(format_), pool, &d3dTextureCube, NULL));
		return MakeCOMPtr(d3dTextureCube);
	}

	void D3D9TextureCube::QueryBaseTexture()
	{
		IDirect3DBaseTexture9* d3dBaseTexture = NULL;
		d3dTextureCube_->QueryInterface(IID_IDirect3DBaseTexture9, reinterpret_cast<void**>(&d3dBaseTexture));
		d3dBaseTexture_ = MakeCOMPtr(d3dBaseTexture);
	}

	void D3D9TextureCube::UpdateParams()
	{
		D3DSURFACE_DESC desc;
		std::memset(&desc, 0, sizeof(desc));

		numMipMaps_ = static_cast<uint16_t>(d3dTextureCube_->GetLevelCount());
		BOOST_ASSERT(numMipMaps_ != 0);

		widths_.resize(numMipMaps_);
		for (uint16_t level = 0; level < numMipMaps_; ++ level)
		{
			TIF(d3dTextureCube_->GetLevelDesc(level, &desc));

			widths_[level] = desc.Width;
		}

		format_ = D3D9Mapping::MappingFormat(desc.Format);
		bpp_	= NumFormatBits(format_);
	}

	void D3D9TextureCube::Usage(TextureUsage usage)
	{
		if (usage != usage_)
		{
			ID3D9CubeTexturePtr d3dTmpTextureCube;
			switch (usage)
			{
			case TU_Default:
				d3dTmpTextureCube = this->CreateTextureCube(0, D3DPOOL_MANAGED);
				break;
				
			case TU_RenderTarget:
				d3dTmpTextureCube = this->CreateTextureCube(D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);
				break;
			}

			ID3D9SurfacePtr src_surf, dest_surf;
			for (int face = 0; face < 6; ++ face)
			{
				for (uint32_t i = 0; i < d3dTextureCube_->GetLevelCount(); ++ i)
				{
					IDirect3DSurface9* pSrcSurf;
					d3dTextureCube_->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), i, &pSrcSurf);
					src_surf = MakeCOMPtr(pSrcSurf);

					IDirect3DSurface9* pDestSurf;
					d3dTmpTextureCube->GetCubeMapSurface(static_cast<D3DCUBEMAP_FACES>(face), i, &pDestSurf);
					dest_surf = MakeCOMPtr(pDestSurf);

					TIF(D3DXLoadSurfaceFromSurface(dest_surf.get(), NULL, NULL,
						src_surf.get(), NULL, NULL, D3DX_FILTER_NONE, 0));
				}
			}
			d3dTextureCube_ = d3dTmpTextureCube;

			this->QueryBaseTexture();
			this->UpdateParams();

			usage_ = usage;
		}
	}
}
