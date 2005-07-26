// D3D9RenderEngine.cpp
// KlayGE D3D9��Ⱦ������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������RenderDeviceCaps (2005.7.17)
// ����StencilBuffer��ز��� (2005.7.20)
//
// 2.7.0
// �Ľ���Render (2005.6.16)
// ȥ����TextureCoordSet (2005.6.26)
// TextureAddressingMode, TextureFiltering��TextureAnisotropy�Ƶ�Texture�� (2005.6.27)
//
// 2.4.0
// ������PolygonMode (2005.3.20)
//
// 2.0.4
// ȥ����WorldMatrices (2004.4.3)
//
// 2.0.3
// �Ż���Render (2004.2.22)
//
// 2.0.1
// �ع���Render (2003.10.10)
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Util.hpp>

#include <KlayGE/Light.hpp>
#include <KlayGE/Material.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderSettings.hpp>

#include <KlayGE/D3D9/D3D9RenderWindow.hpp>
#include <KlayGE/D3D9/D3D9Texture.hpp>
#include <KlayGE/D3D9/D3D9VertexStream.hpp>
#include <KlayGE/D3D9/D3D9IndexStream.hpp>
#include <KlayGE/D3D9/D3D9Mapping.hpp>

#include <cassert>
#include <algorithm>
#include <cstring>

#include <KlayGE/D3D9/D3D9RenderEngine.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::D3D9RenderEngine()
						: cullingMode_(RenderEngine::CM_None),
							clearFlags_(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER)
	{
		// Create our Direct3D object
		d3d_ = MakeCOMPtr(Direct3DCreate9(D3D_SDK_VERSION));
		Verify(d3d_);

		adapterList_.Enumerate(d3d_);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::~D3D9RenderEngine()
	{
		renderEffect_.reset();
		renderTargets_.clear();

		currentVertexDecl_.reset();

		d3dDevice_.reset();
		d3d_.reset();
	}

	// ������Ⱦϵͳ������
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring const & D3D9RenderEngine::Name() const
	{
		static std::wstring const name(L"Direct3D9 Render Engine");
		return name;
	}

	// ��ȡD3D�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	boost::shared_ptr<IDirect3D9> const & D3D9RenderEngine::D3D() const
	{
		return d3d_;
	}

	// ��ȡD3D Device�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	boost::shared_ptr<IDirect3DDevice9> const & D3D9RenderEngine::D3DDevice() const
	{
		return d3dDevice_;
	}

	// ��ȡD3D�������б�
	/////////////////////////////////////////////////////////////////////////////////
	D3D9AdapterList const & D3D9RenderEngine::D3DAdapters() const
	{
		return adapterList_;
	}

	// ��ȡ��ǰ������
	/////////////////////////////////////////////////////////////////////////////////
	D3D9Adapter const & D3D9RenderEngine::ActiveAdapter() const
	{
		return adapterList_.Adapter(adapterList_.CurrentAdapterIndex());
	}

	// ��ʼ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StartRendering()
	{
		bool gotMsg;
		MSG  msg;

		::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

		RenderTarget& renderTarget(*this->ActiveRenderTarget(0));
		while (WM_QUIT != msg.message)
		{
			// ��������Ǽ���ģ��� PeekMessage()�Ա����ǿ����ÿ���ʱ����Ⱦ����
			// ��Ȼ, �� GetMessage() ���� CPU ռ����
			if (renderTarget.Active())
			{
				gotMsg = (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
			}
			else
			{
				gotMsg = (::GetMessage(&msg, NULL, 0, 0) != 0);
			}

			if (gotMsg)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				// �ڿ���ʱ����Ⱦ֡ (û�еȴ�����Ϣ)
				if (renderTarget.Active())
				{
					renderTarget.Update();
				}
			}
		}
	}

	// ���û�����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::AmbientLight(Color const & col)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(col.r(), col.g(), col.b(), 1.0f)));
	}

	// ���������ɫ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ClearColor(Color const & clr)
	{
		clearClr_ = D3DCOLOR_COLORVALUE(clr.r(), clr.g(), clr.b(), 1.0f);
	}

	// ���ù�Ӱ����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ShadingType(ShadeOptions so)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_SHADEMODE, D3D9Mapping::Mapping(so)));
	}

	// ��/�رչ�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::EnableLighting(bool enabled)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_LIGHTING, enabled));
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	RenderWindowPtr D3D9RenderEngine::CreateRenderWindow(std::string const & name,
		RenderSettings const & settings)
	{
		D3D9RenderWindowPtr win(new D3D9RenderWindow(d3d_, this->ActiveAdapter(),
			name, settings));

		d3dDevice_ = win->D3DDevice();
		Verify(d3dDevice_);

		this->FillRenderDeviceCaps();
		renderTargets_.resize(caps_.max_simultaneous_rts);

		this->ActiveRenderTarget(0, win);

		this->DepthBufferDepthTest(settings.depthBuffer);
		this->DepthBufferDepthWrite(settings.depthBuffer);

		return win;
	}

	// ���ü���ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::CullingMode(CullMode mode)
	{
		assert(d3dDevice_);

		cullingMode_ = mode;

		if (this->ActiveRenderTarget(0)->RequiresTextureFlipping())
		{
			if (CM_Clockwise == mode)
			{
				mode = CM_AntiClockwise;
			}
			else
			{
				if (CM_AntiClockwise == mode)
				{
					mode = CM_Clockwise;
				}
			}
		}

		TIF(d3dDevice_->SetRenderState(D3DRS_CULLMODE, D3D9Mapping::Mapping(mode)));
	}

	// ���ö�������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::PolygonMode(FillMode mode)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_FILLMODE, D3D9Mapping::Mapping(mode)));
	}

	// ���ù�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetLight(uint32_t index, Light const & lt)
	{
		assert(d3dDevice_);

		D3DLIGHT9 d3dLight;
		std::memset(&d3dLight, 0, sizeof(d3dLight));

		d3dLight.Type = D3D9Mapping::Mapping(lt.lightType);

		if (Light::LT_Spot == lt.lightType)
		{
			d3dLight.Falloff	= lt.spotFalloff;
			d3dLight.Theta		= lt.spotInner;
			d3dLight.Phi		= lt.spotOuter;
		}

		d3dLight.Diffuse	= D3D9Mapping::MappingToFloat4Color(lt.diffuse);
		d3dLight.Specular	= D3D9Mapping::MappingToFloat4Color(lt.specular);
		d3dLight.Ambient	= D3D9Mapping::MappingToFloat4Color(lt.ambient);

		if (lt.lightType != Light::LT_Directional)
		{
			d3dLight.Position = D3D9Mapping::Mapping(lt.position);
		}
		if (lt.lightType != Light::LT_Point)
		{
			d3dLight.Direction = D3D9Mapping::Mapping(lt.direction);
		}

		d3dLight.Range = lt.range;
		d3dLight.Attenuation0 = lt.attenuationConst;
		d3dLight.Attenuation1 = lt.attenuationLinear;
		d3dLight.Attenuation2 = lt.attenuationQuad;

		TIF(d3dDevice_->SetLight(index, &d3dLight));
	}

	// ��/�ر�ĳ����Դ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::LightEnable(uint32_t index, bool enabled)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->LightEnable(index, enabled));
	}

	// ʵ�������������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoWorldMatrix()
	{
		assert(d3dDevice_);

		D3DMATRIX d3dmat(D3D9Mapping::Mapping(worldMat_));
		TIF(d3dDevice_->SetTransform(D3DTS_WORLD, &d3dmat));
	}

	// ʵ�����ù۲����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoViewMatrix()
	{
		assert(d3dDevice_);

		D3DMATRIX d3dMat(D3D9Mapping::Mapping(viewMat_));
		TIF(d3dDevice_->SetTransform(D3DTS_VIEW, &d3dMat));
	}

	// ʵ������Ͷ�����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoProjectionMatrix()
	{
		assert(d3dDevice_);

		D3DMATRIX d3dMat(D3D9Mapping::Mapping(projMat_));

		if (this->ActiveRenderTarget(0)->RequiresTextureFlipping())
		{
			d3dMat._22 = -d3dMat._22;
		}

		TIF(d3dDevice_->SetTransform(D3DTS_PROJECTION, &d3dMat));
	}

	// ���ò���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetMaterial(Material const & m)
	{
		assert(d3dDevice_);

		D3DMATERIAL9 material;

		material.Diffuse	= D3D9Mapping::MappingToFloat4Color(m.diffuse);
		material.Ambient	= D3D9Mapping::MappingToFloat4Color(m.ambient);
		material.Specular	= D3D9Mapping::MappingToFloat4Color(m.specular);
		material.Emissive	= D3D9Mapping::MappingToFloat4Color(m.emissive);
		material.Power		= m.shininess;

		TIF(d3dDevice_->SetMaterial(&material));
	}

	// ���õ�ǰ��ȾĿ�꣬����ȾĿ������Ѿ����б���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget)
	{
		assert(d3dDevice_);

		IDirect3DSurface9* backBuffer;
		renderTarget->CustomAttribute("DDBACKBUFFER", &backBuffer);
		TIF(d3dDevice_->SetRenderTarget(n, backBuffer));

		IDirect3DSurface9* zBuffer;
		renderTarget->CustomAttribute("D3DZBUFFER", &zBuffer);
		if (zBuffer)
		{
			this->DepthBufferDepthTest(true);
			TIF(d3dDevice_->SetDepthStencilSurface(zBuffer));
		}
		else
		{
			this->DepthBufferDepthTest(false);
		}

		this->CullingMode(cullingMode_);

		Viewport const & vp(renderTarget->GetViewport());
		D3DVIEWPORT9 d3dvp = { vp.left, vp.top, vp.width, vp.height, 0, 1 };
		TIF(d3dDevice_->SetViewport(&d3dvp));
	}

	// ��ʼһ֡
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::BeginFrame()
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->Clear(0, NULL, clearFlags_, clearClr_, 1, 0));

		TIF(d3dDevice_->BeginScene());
	}

	// ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoRender(VertexBuffer const & vb)
	{
		assert(d3dDevice_);
		assert(vb.VertexStreamEnd() - vb.VertexStreamBegin() != 0);

		D3DPRIMITIVETYPE primType;
		uint32_t primCount;
		D3D9Mapping::Mapping(primType, primCount, vb);

		numPrimitivesJustRendered_ += primCount;
		numVerticesJustRendered_ += vb.UseIndices() ? vb.NumIndices() : vb.NumVertices();

		VertexDeclType shaderDecl;
		shaderDecl.reserve(currentDecl_.size());

		D3DVERTEXELEMENT9 element;

		for (VertexBuffer::VertexStreamConstIterator iter = vb.VertexStreamBegin();
			iter != vb.VertexStreamEnd(); ++ iter)
		{
			VertexStream& stream(*(*iter));
			assert(dynamic_cast<D3D9VertexStream*>(&stream) != NULL);

			D3D9Mapping::Mapping(element, shaderDecl.size(), stream);
			shaderDecl.push_back(element);

			D3D9VertexStream& d3d9vs(static_cast<D3D9VertexStream&>(stream));
			TIF(d3dDevice_->SetStreamSource(element.Stream,
				d3d9vs.D3D9Buffer().get(), 0,
				static_cast<UINT>(stream.SizeOfElement() * stream.ElementsPerVertex())));
		}

		{
			element.Stream		= 0xFF;
			element.Type		= D3DDECLTYPE_UNUSED;
			element.Usage		= 0;
			element.UsageIndex	= 0;
			shaderDecl.push_back(element);
		}

		// Clear any previous steam sources
		for (size_t i = shaderDecl.size() - 1; i < currentDecl_.size(); ++ i)
		{
			d3dDevice_->SetStreamSource(static_cast<uint32_t>(i), NULL, 0, 0);
		}


		if ((currentDecl_.size() != shaderDecl.size())
			|| std::memcmp(&currentDecl_[0], &shaderDecl[0],
								sizeof(shaderDecl[0]) * shaderDecl.size()) != 0)
		{
			currentDecl_ = shaderDecl;

			IDirect3DVertexDeclaration9* theVertexDecl;
			TIF(d3dDevice_->CreateVertexDeclaration(&currentDecl_[0], &theVertexDecl));
			currentVertexDecl_ = MakeCOMPtr(theVertexDecl);
		}

		TIF(d3dDevice_->SetVertexDeclaration(currentVertexDecl_.get()));


		if (vb.UseIndices())
		{
			D3D9IndexStream& d3dis(static_cast<D3D9IndexStream&>(*vb.GetIndexStream()));
			d3dDevice_->SetIndices(d3dis.D3D9Buffer().get());

			for (uint32_t i = 0; i < renderPasses_; ++ i)
			{
				renderEffect_->BeginPass(i);

				TIF(d3dDevice_->DrawIndexedPrimitive(primType, 0, 0,
					static_cast<UINT>(vb.NumVertices()), 0, primCount));

				renderEffect_->EndPass();
			}
		}
		else
		{
			for (uint32_t i = 0; i < renderPasses_; ++ i)
			{
				renderEffect_->BeginPass(i);

				TIF(d3dDevice_->DrawPrimitive(primType, 0, primCount));

				renderEffect_->EndPass();
			}
		}
	}

	// ����һ֡
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::EndFrame()
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->EndScene());
	}

	// ��/�ر���Ȳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferDepthTest(bool depthTest)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ZENABLE, depthTest ? D3DZB_TRUE : D3DZB_FALSE));
	}

	// ��/�ر���Ȼ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferDepthWrite(bool depthWrite)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ZWRITEENABLE, depthWrite));
	}

	// ������ȱȽϺ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferFunction(CompareFunction depthFunction)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ZFUNC, D3D9Mapping::Mapping(depthFunction)));
	}

	// �������ƫ��
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBias(uint16_t bias)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_DEPTHBIAS, bias));
	}

	// ������Ч��
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::Fog(FogMode mode, Color const & color,
		float expDensity, float linearStart, float linearEnd)
	{
		assert(d3dDevice_);

		if (Fog_None == mode)
		{
			// just disable
			d3dDevice_->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
			d3dDevice_->SetRenderState(D3DRS_FOGENABLE, false);
		}
		else
		{
			// Allow fog
			d3dDevice_->SetRenderState(D3DRS_FOGENABLE, true);

			// Set pixel fog mode
			d3dDevice_->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
			d3dDevice_->SetRenderState(D3DRS_FOGTABLEMODE, D3D9Mapping::Mapping(mode));

			d3dDevice_->SetRenderState(D3DRS_FOGCOLOR, D3D9Mapping::MappingToUInt32Color(color));
			d3dDevice_->SetRenderState(D3DRS_FOGSTART, *reinterpret_cast<uint32_t*>(&linearStart));
			d3dDevice_->SetRenderState(D3DRS_FOGEND, *reinterpret_cast<uint32_t*>(&linearEnd));
			d3dDevice_->SetRenderState(D3DRS_FOGDENSITY, *reinterpret_cast<uint32_t*>(&expDensity));
		}
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetTexture(uint32_t stage, TexturePtr const & texture)
	{
		assert(d3dDevice_);

		if (!texture)
		{
			TIF(d3dDevice_->SetTexture(stage, NULL));
		}
		else
		{
			assert(dynamic_cast<D3D9Texture*>(texture.get()) != NULL);

			D3D9Texture const & d3d9Tex = static_cast<D3D9Texture const &>(*texture);
			TIF(d3dDevice_->SetTexture(stage, d3d9Tex.D3DBaseTexture().get()));

			uint32_t tfc;
			switch (texture->Type())
			{
			case Texture::TT_1D:
				tfc = caps_.texture_1d_filter_caps;
				break;

			case Texture::TT_2D:
				tfc = caps_.texture_2d_filter_caps;
				break;

			case Texture::TT_3D:
				tfc = caps_.texture_3d_filter_caps;
				break;

			case Texture::TT_Cube:
				tfc = caps_.texture_cube_filter_caps;
				break;

			default:
				assert(false);
				tfc = 0;
				break;
			}

			// Set addressing mode
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSU,
				D3D9Mapping::Mapping(texture->AddressingMode(Texture::TAT_Addr_U))));
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSV,
				D3D9Mapping::Mapping(texture->AddressingMode(Texture::TAT_Addr_V))));
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSW,
				D3D9Mapping::Mapping(texture->AddressingMode(Texture::TAT_Addr_W))));

			// Set filter
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER,
				D3D9Mapping::Mapping(tfc, texture->Filtering(Texture::TFT_Min))));
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER,
				D3D9Mapping::Mapping(tfc, texture->Filtering(Texture::TFT_Mag))));
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER,
				D3D9Mapping::Mapping(tfc, texture->Filtering(Texture::TFT_Mip))));

			// Set anisotropy
			assert(texture->Anisotropy() < caps_.max_texture_anisotropy);
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAXANISOTROPY, texture->Anisotropy()));

			// Set max mip level
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAXMIPLEVEL, texture->MaxMipLevel()));

			// Set mip map lod bias
			float bias = texture->MipMapLodBias();
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPMAPLODBIAS, *reinterpret_cast<DWORD*>(&bias)));
		}
	}

	// �ر�ĳ������׶�
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DisableTextureStage(uint32_t stage)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetTexture(stage, NULL));
		TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE));
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureCoordCalculation(uint32_t stage, TexCoordCalcMethod m)
	{
		assert(d3dDevice_);

		HRESULT hr = S_OK;
		D3DXMATRIX matTrans;
		switch (m)
		{
		case TCC_None:
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			D3DXMatrixIdentity(&matTrans);
			hr = d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &matTrans);
			break;

		case TCC_EnvironmentMap:
			// Sets the flags required for an environment map effect
			hr = d3dDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			D3DXMatrixIdentity(&matTrans);
			matTrans._11 = 0.5f;
			matTrans._41 = 0.5f;
			matTrans._22 = -0.5f;
			matTrans._42 = 0.5f;
			hr = d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &matTrans);
			break;

		case TCC_EnvironmentMapPlanar:
			// Sets the flags required for an environment map effect
			hr = d3dDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			D3DXMatrixIdentity(&matTrans);
			matTrans._11 = 0.5f;
			matTrans._41 = 0.5f;
			matTrans._22 = -0.5f;
			matTrans._42 = 0.5f;
			hr = d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &matTrans);
			break;
		}

		TIF(hr);
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureMatrix(uint32_t stage, Matrix4 const & mat)
	{
		assert(d3dDevice_);

		if (Matrix4::Identity() == mat)
		{
			TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE));
		}
		else
		{
			// Set 2D input
			// TODO: deal with 3D coordinates when cubic environment mapping supported
			TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));

			D3DMATRIX d3dMat(D3D9Mapping::Mapping(mat));
			TIF(d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &d3dMat));
		}
	}

	// ��ģ�建����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilCheckEnabled(bool enabled)
	{
		if (enabled)
		{
			clearFlags_ |= D3DCLEAR_STENCIL;
		}
		else
		{
			clearFlags_ &= ~D3DCLEAR_STENCIL;
		}

		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILENABLE, enabled));
	}

	// Ӳ���Ƿ�֧��ģ�建����
	/////////////////////////////////////////////////////////////////////////////////
	bool D3D9RenderEngine::HasHardwareStencil()
	{
		assert(d3dDevice_);

		IDirect3DSurface9* surf;
		D3DSURFACE_DESC surfDesc;
		d3dDevice_->GetDepthStencilSurface(&surf);
		boost::shared_ptr<IDirect3DSurface9> surf_ptr = MakeCOMPtr(surf);
		surf_ptr->GetDesc(&surfDesc);

		if (D3DFMT_D24S8 == surfDesc.Format)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// ����ģ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	uint16_t D3D9RenderEngine::StencilBufferBitDepth()
	{
		return 8;
	}

	// ����ģ��ȽϺ������ο�ֵ������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferFunction(CompareFunction func, uint32_t refValue, uint32_t mask)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILFUNC, D3D9Mapping::Mapping(func)));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILREF, refValue));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILMASK, mask));
	}

	// ����ģ�建����ģ�����ʧ�ܣ���Ȳ���ʧ�ܺ�ͨ����Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferOperation(StencilOperation fail,
		StencilOperation depth_fail, StencilOperation pass)
	{
		assert(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILFAIL, D3D9Mapping::Mapping(fail)));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILZFAIL, D3D9Mapping::Mapping(depth_fail)));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILPASS, D3D9Mapping::Mapping(pass)));
	}

	// ����豸����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::FillRenderDeviceCaps()
	{
		assert(d3dDevice_);

		D3DCAPS9 d3d_caps;
		d3dDevice_->GetDeviceCaps(&d3d_caps);

		caps_ = D3D9Mapping::Mapping(d3d_caps);
	}

	// ��Ӧ�豸��ʧ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::OnLostDevice()
	{
	}
	
	// ��Ӧ�豸��λ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::OnResetDevice()
	{
		for (uint32_t i = 0; i < renderTargets_.size(); ++ i)
		{
			this->ActiveRenderTarget(i, renderTargets_[i]);
		}
	}
}
