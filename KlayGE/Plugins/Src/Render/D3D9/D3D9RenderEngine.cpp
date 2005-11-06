// D3D9RenderEngine.cpp
// KlayGE D3D9��Ⱦ������ ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ȥ���˹̶���ˮ�� (2005.8.18)
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
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <KlayGE/Viewport.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderSettings.hpp>

#include <KlayGE/D3D9/D3D9RenderWindow.hpp>
#include <KlayGE/D3D9/D3D9Texture.hpp>
#include <KlayGE/D3D9/D3D9VertexStream.hpp>
#include <KlayGE/D3D9/D3D9IndexStream.hpp>
#include <KlayGE/D3D9/D3D9RenderEffect.hpp>
#include <KlayGE/D3D9/D3D9Mapping.hpp>
#include <KlayGE/D3D9/D3D9VertexBuffer.hpp>

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/bind.hpp>

#include <KlayGE/D3D9/D3D9RenderEngine.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::D3D9RenderEngine()
						: cullingMode_(RenderEngine::CM_None),
							clearFlags_(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
							last_num_vertex_stream_(1)
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
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_SHADEMODE, D3D9Mapping::Mapping(so)));
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

		if (caps_.max_shader_model >= 3)
		{
			RenderInstance = boost::bind(&D3D9RenderEngine::DoRenderHWInstance, this, _1);
		}
		else
		{
			RenderInstance = boost::bind(&D3D9RenderEngine::DoRenderSWInstance, this, _1);
		}

		return win;
	}

	// ���ü���ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::CullingMode(CullMode mode)
	{
		BOOST_ASSERT(d3dDevice_);

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
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_FILLMODE, D3D9Mapping::Mapping(mode)));
	}

	// ���õ�ǰ��ȾĿ�꣬����ȾĿ������Ѿ����б���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget)
	{
		BOOST_ASSERT(d3dDevice_);

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
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->Clear(0, NULL, clearFlags_, clearClr_, 1, 0));

		TIF(d3dDevice_->BeginScene());
	}

	// ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoRender(VertexBuffer const & vb)
	{
		this->RenderInstance(vb);
	}

	void D3D9RenderEngine::DoRenderSWInstance(VertexBuffer const & vb)
	{
		BOOST_ASSERT(d3dDevice_);
		BOOST_ASSERT(vb.VertexStreamEnd() - vb.VertexStreamBegin() != 0);
		
		uint32_t const num_instance = (*vb.VertexStreamBegin())->Frequency();

		if (num_instance > 1)
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			VertexBufferPtr tmp_vb = rf.MakeVertexBuffer(vb.Type());
			VertexStreamPtr tmp_inst_vs;

			vb.ExpandInstance(tmp_inst_vs, 0);

			for (VertexBuffer::VertexStreamConstIterator iter = vb.VertexStreamBegin();
				iter != vb.VertexStreamEnd(); ++ iter)
			{
				tmp_vb->AddVertexStream(*iter);
			}
			tmp_vb->AddVertexStream(tmp_inst_vs);
			if (vb.UseIndices())
			{
				tmp_vb->SetIndexStream(vb.GetIndexStream());
			}

			this->RenderVBSWInstance(*tmp_vb);

			for (uint32_t i = 1; i < num_instance; ++ i)
			{
				vb.ExpandInstance(tmp_inst_vs, i);
				this->RenderVBSWInstance(*tmp_vb);
			}
		}
		else
		{
			this->RenderVBSWInstance(vb);
		}
	}

	void D3D9RenderEngine::DoRenderHWInstance(VertexBuffer const & vb)
	{
		this->RenderVBHWInstance(vb);
	}

	void D3D9RenderEngine::RenderVBSWInstance(VertexBuffer const & vb)
	{
		D3DPRIMITIVETYPE primType;
		uint32_t primCount;
		D3D9Mapping::Mapping(primType, primCount, vb);

		numPrimitivesJustRendered_ += primCount;
		numVerticesJustRendered_ += vb.UseIndices() ? vb.NumIndices() : vb.NumVertices();

		for (VertexBuffer::VertexStreamConstIterator iter = vb.VertexStreamBegin();
			iter != vb.VertexStreamEnd(); ++ iter)
		{
			VertexStream& stream = *(*iter);
			uint32_t number = static_cast<uint32_t>(iter - vb.VertexStreamBegin());

			D3D9VertexStream& d3d9vs(*checked_cast<D3D9VertexStream*>(&stream));
			TIF(d3dDevice_->SetStreamSource(number,
				d3d9vs.D3D9Buffer().get(), 0,
				static_cast<UINT>(stream.VertexSize())));
		}

		// Clear any previous steam sources
		uint32_t const num_vertex_stream = static_cast<uint32_t>(vb.VertexStreamEnd() - vb.VertexStreamBegin())
			+ (vb.InstanceStream() ? 1 : 0);
		for (uint32_t i = num_vertex_stream; i < last_num_vertex_stream_; ++ i)
		{
			d3dDevice_->SetStreamSource(i, NULL, 0, 0);
		}
		last_num_vertex_stream_ = num_vertex_stream;

		D3D9VertexBuffer const & d3d9_vb(*checked_cast<D3D9VertexBuffer const *>(&vb));
		TIF(d3dDevice_->SetVertexDeclaration(d3d9_vb.VertexDeclaration().get()));

		RenderTechniquePtr tech = renderEffect_->ActiveTechnique();
		uint32_t num_passes = tech->NumPasses();
		if (vb.UseIndices())
		{
			D3D9IndexStream& d3dis(*checked_cast<D3D9IndexStream*>(vb.GetIndexStream().get()));
			d3dDevice_->SetIndices(d3dis.D3D9Buffer().get());

			for (uint32_t i = 0; i < num_passes; ++ i)
			{
				RenderPassPtr pass = tech->Pass(i);

				pass->Begin();
				TIF(d3dDevice_->DrawIndexedPrimitive(primType, 0, 0,
					static_cast<UINT>(vb.NumVertices()), 0, primCount));
				pass->End();
			}
		}
		else
		{
			for (uint32_t i = 0; i < num_passes; ++ i)
			{
				RenderPassPtr pass = tech->Pass(i);

				pass->Begin();
				TIF(d3dDevice_->DrawPrimitive(primType, 0, primCount));
				pass->End();
			}
		}
	}

	void D3D9RenderEngine::RenderVBHWInstance(VertexBuffer const & vb)
	{
		D3DPRIMITIVETYPE primType;
		uint32_t primCount;
		D3D9Mapping::Mapping(primType, primCount, vb);

		numPrimitivesJustRendered_ += primCount;
		numVerticesJustRendered_ += vb.UseIndices() ? vb.NumIndices() : vb.NumVertices();

		for (VertexBuffer::VertexStreamConstIterator iter = vb.VertexStreamBegin();
			iter != vb.VertexStreamEnd(); ++ iter)
		{
			VertexStream& stream = *(*iter);
			uint32_t number = static_cast<uint32_t>(iter - vb.VertexStreamBegin());

			D3D9VertexStream& d3d9vs(*checked_cast<D3D9VertexStream*>(&stream));
			TIF(d3dDevice_->SetStreamSource(number,
				d3d9vs.D3D9Buffer().get(), 0,
				static_cast<UINT>(stream.VertexSize())));

			TIF(d3dDevice_->SetStreamSourceFreq(number, D3DSTREAMSOURCE_INDEXEDDATA | stream.Frequency()));
		}
		if (vb.InstanceStream())
		{
			uint32_t number = static_cast<uint32_t>(vb.VertexStreamEnd() - vb.VertexStreamBegin());

			VertexStream& stream = *vb.InstanceStream();
			D3D9VertexStream& d3d9vs(*checked_cast<D3D9VertexStream*>(&stream));
			TIF(d3dDevice_->SetStreamSource(number,
				d3d9vs.D3D9Buffer().get(), 0,
				static_cast<UINT>(stream.VertexSize())));

			TIF(d3dDevice_->SetStreamSourceFreq(number, D3DSTREAMSOURCE_INSTANCEDATA | stream.Frequency()));
		}

		// Clear any previous steam sources
		uint32_t const num_vertex_stream = static_cast<uint32_t>(vb.VertexStreamEnd() - vb.VertexStreamBegin())
			+ (vb.InstanceStream() ? 1 : 0);
		for (uint32_t i = num_vertex_stream; i < last_num_vertex_stream_; ++ i)
		{
			d3dDevice_->SetStreamSource(i, NULL, 0, 0);
			d3dDevice_->SetStreamSourceFreq(i, 0);
		}
		last_num_vertex_stream_ = num_vertex_stream;

		D3D9VertexBuffer const & d3d9_vb(*checked_cast<D3D9VertexBuffer const *>(&vb));
		TIF(d3dDevice_->SetVertexDeclaration(d3d9_vb.VertexDeclaration().get()));

		RenderTechniquePtr tech = renderEffect_->ActiveTechnique();
		uint32_t num_passes = tech->NumPasses();
		if (vb.UseIndices())
		{
			D3D9IndexStream& d3dis(*checked_cast<D3D9IndexStream*>(vb.GetIndexStream().get()));
			d3dDevice_->SetIndices(d3dis.D3D9Buffer().get());

			for (uint32_t i = 0; i < num_passes; ++ i)
			{
				RenderPassPtr pass = tech->Pass(i);

				pass->Begin();
				TIF(d3dDevice_->DrawIndexedPrimitive(primType, 0, 0,
					static_cast<UINT>(vb.NumVertices()), 0, primCount));
				pass->End();
			}
		}
		else
		{
			for (uint32_t i = 0; i < num_passes; ++ i)
			{
				RenderPassPtr pass = tech->Pass(i);

				pass->Begin();
				TIF(d3dDevice_->DrawPrimitive(primType, 0, primCount));
				pass->End();
			}
		}
	}

	// ����һ֡
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::EndFrame()
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->EndScene());
	}

	// ��/�ر�Alpha���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::AlphaBlend(bool enabled)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ALPHABLENDENABLE, enabled ? D3DZB_TRUE : D3DZB_FALSE));
	}
	
	// ����Alpha�������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::AlphaBlendFunction(AlphaBlendFactor src_factor, AlphaBlendFactor dst_factor)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_SRCBLEND, D3D9Mapping::Mapping(src_factor)));
		TIF(d3dDevice_->SetRenderState(D3DRS_DESTBLEND, D3D9Mapping::Mapping(dst_factor)));
	}

	// ��/�ر���Ȳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferDepthTest(bool enabled)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ZENABLE, enabled ? D3DZB_TRUE : D3DZB_FALSE));
	}

	// ��/�ر���Ȼ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferDepthWrite(bool enabled)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ZWRITEENABLE, enabled ? D3DZB_TRUE : D3DZB_FALSE));
	}

	// ������ȱȽϺ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferFunction(CompareFunction depthFunction)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ZFUNC, D3D9Mapping::Mapping(depthFunction)));
	}

	// �������ƫ��
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBias(uint16_t bias)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_DEPTHBIAS, bias));
	}

	// ��/�ر�Alpha����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::AlphaTest(bool enabled)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ALPHATESTENABLE, enabled ? D3DZB_TRUE : D3DZB_FALSE));
	}

	// ����Alpha�ȽϺ����Ͳο�ֵ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::AlphaFunction(CompareFunction alphaFunction, float refValue)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_ALPHAFUNC, D3D9Mapping::Mapping(alphaFunction)));
		TIF(d3dDevice_->SetRenderState(D3DRS_ALPHAREF, static_cast<uint32_t>(refValue * 255) & 0xFF));
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetSampler(uint32_t stage, SamplerPtr const & sampler)
	{
		BOOST_ASSERT(d3dDevice_);

		TexturePtr texture = sampler->GetTexture();

		if (!sampler || !texture)
		{
			TIF(d3dDevice_->SetTexture(stage, NULL));
		}
		else
		{
			D3D9Texture const & d3d9Tex(*checked_cast<D3D9Texture const *>(texture.get()));
			TIF(d3dDevice_->SetTexture(stage, d3d9Tex.D3DBaseTexture().get()));

			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_BORDERCOLOR,
				D3D9Mapping::MappingToUInt32Color(sampler->BorderColor())));

			// Set addressing mode
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSU,
				D3D9Mapping::Mapping(sampler->AddressingMode(Sampler::TAT_Addr_U))));
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSV,
				D3D9Mapping::Mapping(sampler->AddressingMode(Sampler::TAT_Addr_V))));
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSW,
				D3D9Mapping::Mapping(sampler->AddressingMode(Sampler::TAT_Addr_W))));

			{
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
					BOOST_ASSERT(false);
					tfc = 0;
					break;
				}

				Sampler::TexFilterOp filter = sampler->Filtering();
				if (Sampler::TFO_Anisotropic == filter)
				{
					if (0 == (tfc & Sampler::TFO_Anisotropic))
					{
						filter = Sampler::TFO_Trilinear;
					}
				}
				if (Sampler::TFO_Trilinear == filter)
				{
					if (0 == (tfc & Sampler::TFO_Trilinear))
					{
						filter = Sampler::TFO_Bilinear;
					}
				}
				if (Sampler::TFO_Bilinear == filter)
				{
					if (0 == (tfc & Sampler::TFO_Bilinear))
					{
						filter = Sampler::TFO_Point;
					}
				}
				if (Sampler::TFO_Point == filter)
				{
					if (0 == (tfc & Sampler::TFO_Point))
					{
						filter = Sampler::TFO_None;
					}
				}

				// Set filter
				switch (filter)
				{
				case Sampler::TFO_None:
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_NONE));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_NONE));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE));
					break;

				case Sampler::TFO_Point:
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_POINT));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_POINT));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT));
					break;

				case Sampler::TFO_Bilinear:
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT));
					break;

				case Sampler::TFO_Trilinear:
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));
					break;

				case Sampler::TFO_Anisotropic:
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
					TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));
					break;
				}
			}

			// Set anisotropy
			BOOST_ASSERT(sampler->Anisotropy() < caps_.max_texture_anisotropy);
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAXANISOTROPY, sampler->Anisotropy()));

			// Set max mip level
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MAXMIPLEVEL, sampler->MaxMipLevel()));

			// Set mip map lod bias
			float bias = sampler->MipMapLodBias();
			TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPMAPLODBIAS, *reinterpret_cast<DWORD*>(&bias)));

			if (Matrix4::Identity() == sampler->TextureMatrix())
			{
				TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE));
			}
			else
			{
				// Set 2D input
				// TODO: deal with 3D coordinates when cubic environment mapping supported
				TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));

				D3DMATRIX d3dMat(D3D9Mapping::Mapping(sampler->TextureMatrix()));
				TIF(d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &d3dMat));
			}
		}
	}

	// �ر�ĳ������׶�
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DisableSampler(uint32_t stage)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetTexture(stage, NULL));
		TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE));
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
		BOOST_ASSERT(d3dDevice_);

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
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILFUNC, D3D9Mapping::Mapping(func)));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILREF, refValue));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILMASK, mask));
	}

	// ����ģ�建����ģ�����ʧ�ܣ���Ȳ���ʧ�ܺ�ͨ����Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferOperation(StencilOperation fail,
		StencilOperation depth_fail, StencilOperation pass)
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILFAIL, D3D9Mapping::Mapping(fail)));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILZFAIL, D3D9Mapping::Mapping(depth_fail)));
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILPASS, D3D9Mapping::Mapping(pass)));
	}

	// ��/�رյ㾫��ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::PointSpriteEnable(bool enable)
	{
		d3dDevice_->SetRenderState(D3DRS_POINTSPRITEENABLE, enable);
	}

	// ���õ�������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::PointDistanceAttenuation(float quadratic0, float quadratic1, float quadratic2)
	{
		if ((0 == quadratic0) && (0 == quadratic1) && (0 == quadratic2))
		{
			d3dDevice_->SetRenderState(D3DRS_POINTSCALEENABLE, false);
		}
		else
		{
			d3dDevice_->SetRenderState(D3DRS_POINTSCALEENABLE, true);

			d3dDevice_->SetRenderState(D3DRS_POINTSCALE_A, *reinterpret_cast<DWORD*>(&quadratic0));
			d3dDevice_->SetRenderState(D3DRS_POINTSCALE_B, *reinterpret_cast<DWORD*>(&quadratic1));
			d3dDevice_->SetRenderState(D3DRS_POINTSCALE_C, *reinterpret_cast<DWORD*>(&quadratic2));
		}
	}

	// ���õ��С
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::PointSize(float size)
	{
		d3dDevice_->SetRenderState(D3DRS_POINTSIZE, *reinterpret_cast<DWORD*>(&size));
	}

	// ���õ����С����С
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::PointMinMaxSize(float min_size, float max_size)
	{
		min_size = std::max(min_size, caps_.min_point_size);
		max_size = std::min(max_size, caps_.max_point_size);

		d3dDevice_->SetRenderState(D3DRS_POINTSIZE_MIN, *reinterpret_cast<DWORD*>(&min_size));
		d3dDevice_->SetRenderState(D3DRS_POINTSIZE_MAX, *reinterpret_cast<DWORD*>(&max_size));
	}

	// ��/�رռ�������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ScissorTest(bool enabled)
	{
		d3dDevice_->SetRenderState(D3DRS_SCISSORTESTENABLE, enabled);
	}

	// ���ü�������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		RECT rc = { x, y, width, height };
		d3dDevice_->SetScissorRect(&rc);
	}

	// ����豸����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::FillRenderDeviceCaps()
	{
		BOOST_ASSERT(d3dDevice_);

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
