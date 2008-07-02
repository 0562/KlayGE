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
#include <KlayGE/COMPtr.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/GraphicsBuffer.hpp>
#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/RenderStateObject.hpp>
#include <KlayGE/Sampler.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderSettings.hpp>

#include <KlayGE/D3D9/D3D9RenderWindow.hpp>
#include <KlayGE/D3D9/D3D9FrameBuffer.hpp>
#include <KlayGE/D3D9/D3D9Texture.hpp>
#include <KlayGE/D3D9/D3D9GraphicsBuffer.hpp>
#include <KlayGE/D3D9/D3D9Mapping.hpp>
#include <KlayGE/D3D9/D3D9RenderLayout.hpp>
#include <KlayGE/D3D9/D3D9ShaderObject.hpp>

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <KlayGE/D3D9/D3D9RenderEngine.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#endif

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::D3D9RenderEngine()
		: last_num_vertex_stream_(0)
	{
		// Create our Direct3D object
		d3d_ = MakeCOMPtr(Direct3DCreate9(D3D_SDK_VERSION));
		Verify(d3d_ != ID3D9Ptr());

		adapterList_.Enumerate(d3d_);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::~D3D9RenderEngine()
	{
		cur_frame_buffer_.reset();
		default_frame_buffer_.reset();

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
	ID3D9Ptr const & D3D9RenderEngine::D3DObject() const
	{
		return d3d_;
	}

	// ��ȡD3D Device�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	ID3D9DevicePtr const & D3D9RenderEngine::D3DDevice() const
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

		FrameBuffer& fb = *this->CurFrameBuffer();
		while (WM_QUIT != msg.message)
		{
			// ��������Ǽ���ģ��� PeekMessage()�Ա����ǿ����ÿ���ʱ����Ⱦ����
			// ��Ȼ, �� GetMessage() ���� CPU ռ����
			if (fb.Active())
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
				if (fb.Active())
				{
					Context::Instance().SceneManagerInstance().Update();
					fb.SwapBuffers();
				}
			}
		}
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::CreateRenderWindow(std::string const & name,
		RenderSettings const & settings)
	{
		D3D9RenderWindowPtr win(new D3D9RenderWindow(d3d_, this->ActiveAdapter(),
			name, settings));
		default_frame_buffer_ = win;

		win->Attach(FrameBuffer::ATT_Color0, D3D9SurfaceRenderViewPtr(new D3D9SurfaceRenderView(win->D3DBackBuffer())));
		if (win->D3DDepthStencilBuffer())
		{
			win->Attach(FrameBuffer::ATT_DepthStencil, D3D9SurfaceRenderViewPtr(new D3D9SurfaceRenderView(win->D3DDepthStencilBuffer())));
		}

		this->BindFrameBuffer(win);
	}

	void D3D9RenderEngine::D3DDevice(ID3D9DevicePtr const & device)
	{
		d3dDevice_ = device;
		Verify(d3dDevice_ != ID3D9DevicePtr());

		this->FillRenderDeviceCaps();
		this->InitRenderStates();

		if (caps_.hw_instancing_support)
		{
			RenderInstance = boost::bind(&D3D9RenderEngine::DoRenderHWInstance, this, _1, _2);
		}
		else
		{
			RenderInstance = boost::bind(&D3D9RenderEngine::DoRenderSWInstance, this, _1, _2);
		}
	}

	void D3D9RenderEngine::InitRenderStates()
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();
		cur_rs_obj_ = rf.MakeRasterizerStateObject(RasterizerStateDesc());
		cur_dss_obj_ = rf.MakeDepthStencilStateObject(DepthStencilStateDesc());
		cur_bs_obj_ = rf.MakeBlendStateObject(BlendStateDesc());
		cur_rs_obj_->Active();
		cur_dss_obj_->Active();
		cur_bs_obj_->Active();
	}

	// ���õ�ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoBindFrameBuffer(FrameBufferPtr fb)
	{
		BOOST_ASSERT(d3dDevice_);
		BOOST_ASSERT(fb);
	}

	// ��ʼһ֡
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::BeginFrame()
	{
		BOOST_ASSERT(d3dDevice_);

		TIF(d3dDevice_->BeginScene());
	}

	// ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DoRender(RenderTechnique const & tech, RenderLayout const & rl)
	{
		if (rl.InstanceStream() && !rl.UseIndices())
		{
			this->DoRenderSWInstance(tech, rl);
		}
		else
		{
			this->RenderInstance(tech, rl);
		}
	}

	void D3D9RenderEngine::DoRenderSWInstance(RenderTechnique const & tech, RenderLayout const & rl)
	{
		BOOST_ASSERT(d3dDevice_);
		BOOST_ASSERT(rl.NumVertexStreams() != 0);

		uint32_t const num_instance = rl.NumInstance();

		if (num_instance > 1)
		{
			BOOST_ASSERT(rl.InstanceStream());

			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			RenderLayoutPtr tmp_rl = rf.MakeRenderLayout();
			tmp_rl->TopologyType(rl.TopologyType());
			GraphicsBufferPtr tmp_inst_vs;

			rl.ExpandInstance(tmp_inst_vs, 0);

			for (uint32_t i = 0; i < rl.NumVertexStreams(); ++ i)
			{
				tmp_rl->BindVertexStream(rl.GetVertexStream(i), rl.VertexStreamFormat(i));
			}
			tmp_rl->BindVertexStream(tmp_inst_vs, rl.InstanceStreamFormat());
			if (rl.UseIndices())
			{
				tmp_rl->BindIndexStream(rl.GetIndexStream(), rl.IndexStreamFormat());
			}

			this->RenderRLSWInstance(tech, *tmp_rl);

			for (uint32_t i = 1; i < num_instance; ++ i)
			{
				rl.ExpandInstance(tmp_inst_vs, i);
				this->RenderRLSWInstance(tech, *tmp_rl);
			}
		}
		else
		{
			this->RenderRLSWInstance(tech, rl);
		}
	}

	void D3D9RenderEngine::DoRenderHWInstance(RenderTechnique const & tech, RenderLayout const & rl)
	{
		uint32_t this_num_vertex_stream = rl.NumVertexStreams();
		for (uint32_t i = 0; i < rl.NumVertexStreams(); ++ i)
		{
			GraphicsBufferPtr stream = rl.GetVertexStream(i);

			D3D9VertexBuffer& d3d9vb(*checked_pointer_cast<D3D9VertexBuffer>(stream));
			TIF(d3dDevice_->SetStreamSource(i, d3d9vb.D3D9Buffer().get(), 0, rl.VertexSize(i)));
		}
		if (rl.InstanceStream())
		{
			for (uint32_t i = 0; i < rl.NumVertexStreams(); ++ i)
			{
				TIF(d3dDevice_->SetStreamSourceFreq(i,
					D3DSTREAMSOURCE_INDEXEDDATA | rl.VertexStreamFrequency(i)));
			}

			uint32_t number = rl.NumVertexStreams();
			GraphicsBufferPtr stream = rl.InstanceStream();

			D3D9VertexBuffer& d3d9vb(*checked_pointer_cast<D3D9VertexBuffer>(stream));
			TIF(d3dDevice_->SetStreamSource(number, d3d9vb.D3D9Buffer().get(), 0, rl.InstanceSize()));

			TIF(d3dDevice_->SetStreamSourceFreq(number,
				D3DSTREAMSOURCE_INSTANCEDATA | 1UL));

			++ this_num_vertex_stream;
		}
		else
		{
			for (uint32_t i = 0; i < rl.NumVertexStreams() + 1; ++ i)
			{
				TIF(d3dDevice_->SetStreamSourceFreq(i, 1UL));
			}
		}

		for (uint32_t i = this_num_vertex_stream; i < last_num_vertex_stream_; ++ i)
		{
			TIF(d3dDevice_->SetStreamSource(i, NULL, 0, 0));
			TIF(d3dDevice_->SetStreamSourceFreq(i, 1UL));
		}

		last_num_vertex_stream_ = this_num_vertex_stream;

		this->RenderRL(tech, rl);
	}

	void D3D9RenderEngine::RenderRLSWInstance(RenderTechnique const & tech, RenderLayout const & rl)
	{
		uint32_t this_num_vertex_stream = rl.NumVertexStreams();
		for (uint32_t i = 0; i < rl.NumVertexStreams(); ++ i)
		{
			GraphicsBufferPtr stream = rl.GetVertexStream(i);

			D3D9VertexBuffer& d3d9vb(*checked_pointer_cast<D3D9VertexBuffer>(stream));
			TIF(d3dDevice_->SetStreamSource(i, d3d9vb.D3D9Buffer().get(), 0, rl.VertexSize(i)));
		}

		for (uint32_t i = this_num_vertex_stream; i < last_num_vertex_stream_; ++ i)
		{
			TIF(d3dDevice_->SetStreamSource(i, NULL, 0, 0));
			TIF(d3dDevice_->SetStreamSourceFreq(i, 1UL));
		}

		last_num_vertex_stream_ = this_num_vertex_stream;

		this->RenderRL(tech, rl);
	}

	void D3D9RenderEngine::RenderRL(RenderTechnique const & tech, RenderLayout const & rl)
	{
		D3DPRIMITIVETYPE primType;
		uint32_t primCount;
		D3D9Mapping::Mapping(primType, primCount, rl);

		numPrimitivesJustRendered_ += primCount;
		numVerticesJustRendered_ += rl.UseIndices() ? rl.NumIndices() : rl.NumVertices();

		D3D9RenderLayout const & d3d9_rl(*checked_cast<D3D9RenderLayout const *>(&rl));
		TIF(d3dDevice_->SetVertexDeclaration(d3d9_rl.VertexDeclaration().get()));

		uint32_t num_passes = tech.NumPasses();
		if (rl.UseIndices())
		{
			D3D9IndexBuffer& d3dib(*checked_pointer_cast<D3D9IndexBuffer>(rl.GetIndexStream()));
			d3dib.SwitchFormat(rl.IndexStreamFormat());
			d3dDevice_->SetIndices(d3dib.D3D9Buffer().get());

			for (uint32_t i = 0; i < num_passes; ++ i)
			{
				tech.Pass(i)->Apply();

				TIF(d3dDevice_->DrawIndexedPrimitive(primType, 0, 0,
					static_cast<UINT>(rl.NumVertices()), 0, primCount));
			}
		}
		else
		{
			d3dDevice_->SetIndices(NULL);

			for (uint32_t i = 0; i < num_passes; ++ i)
			{
				tech.Pass(i)->Apply();

				TIF(d3dDevice_->DrawPrimitive(primType, 0, primCount));
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

	// ��ȡģ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	uint16_t D3D9RenderEngine::StencilBufferBitDepth()
	{
		BOOST_ASSERT(d3dDevice_);

		IDirect3DSurface9* surf;
		D3DSURFACE_DESC surfDesc;
		d3dDevice_->GetDepthStencilSurface(&surf);
		ID3D9SurfacePtr surf_ptr = MakeCOMPtr(surf);
		surf_ptr->GetDesc(&surfDesc);

		if (D3DFMT_D24S8 == surfDesc.Format)
		{
			return 8;
		}
		else
		{
			return 0;
		}
	}

	// ���ü�������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		RECT rc = { x, y, width, height };
		d3dDevice_->SetScissorRect(&rc);
	}

	void D3D9RenderEngine::Resize(uint32_t width, uint32_t height)
	{
		checked_pointer_cast<D3D9RenderWindow>(default_frame_buffer_)->Resize(width, height);
	}

	bool D3D9RenderEngine::FullScreen() const
	{
		return checked_pointer_cast<D3D9RenderWindow>(default_frame_buffer_)->FullScreen();
	}

	void D3D9RenderEngine::FullScreen(bool fs)
	{
		checked_pointer_cast<D3D9RenderWindow>(default_frame_buffer_)->FullScreen(fs);
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
		this->InitRenderStates();
	}

	// ��Ӧ�豸��λ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::OnResetDevice()
	{
		this->BindFrameBuffer(cur_frame_buffer_);
	}
}
