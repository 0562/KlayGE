// RenderEngine.cpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ͳһ��RenderState (2006.5.21)
//
// 2.8.0
// ����StencilBuffer��ز��� (2005.7.20)
//
// 2.7.1
// ViewMatrix��ProjectionMatrix��Ϊconst (2005.7.10)
//
// 2.4.0
// ������NumPrimitivesJustRendered��NumVerticesJustRendered (2005.3.21)
//
// 2.0.3
// �Ż���RenderEffect������ (2004.2.16)
// ȥ����VO_2D (2004.3.1)
// ȥ����SoftwareBlend (2004.3.10)
//
// 2.0.0
// ���ν���(2003.10.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/GraphicsBuffer.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderWindow.hpp>

#include <KlayGE/RenderEngine.hpp>

namespace KlayGE
{
	class NullRenderEngine : public RenderEngine
	{
	public:
		std::wstring const & Name() const
		{
			static std::wstring const name(L"Null Render Engine");
			return name;
		}

		void StartRendering()
		{
		}

		void BeginFrame()
		{
		}
		void EndFrame()
		{
		}

		void Clear(uint32_t /*masks*/, Color const & /*clr*/, float /*depth*/, int32_t /*stencil*/)
		{
		}

		RenderWindowPtr CreateRenderWindow(std::string const & /*name*/, RenderSettings const & /*settings*/)
		{
			return RenderWindow::NullObject();
		}

		void SetSampler(uint32_t /*stage*/, SamplerPtr const & /*sampler*/)
		{
		}
		void DisableSampler(uint32_t /*stage*/)
		{
		}

		uint16_t StencilBufferBitDepth()
		{
			return 0;
		}

		void ScissorRect(uint32_t /*x*/, uint32_t /*y*/, uint32_t /*width*/, uint32_t /*height*/)
		{
		}

		float4 TexelToPixelOffset() const
		{
			return float4(0, 0, 0, 0);
		}

	private:
		void DoBindRenderTarget(RenderTargetPtr /*rt*/)
		{
		}

		void DoRender(RenderLayout const & /*rl*/)
		{
		}

		void DoFlushRenderStates()
		{
		}

		void InitRenderStates()
		{
		}

		void FillRenderDeviceCaps()
		{
		}
	};

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::RenderEngine()
		: render_tech_(RenderTechnique::NullObject())
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::~RenderEngine()
	{
	}

	// ���ؿն���
	/////////////////////////////////////////////////////////////////////////////////
	RenderEnginePtr RenderEngine::NullObject()
	{
		static RenderEnginePtr obj(new NullRenderEngine);
		return obj;
	}

	// ���õ�ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::BindRenderTarget(RenderTargetPtr rt)
	{
		if (cur_render_target_)
		{
			cur_render_target_->OnUnbind();
		}

		if (!rt)
		{
			cur_render_target_ = default_render_target_;
		}
		else
		{
			cur_render_target_ = rt;
		}

		cur_render_target_->OnBind();

		this->DoBindRenderTarget(cur_render_target_);
	}

	// ��ȡ��ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderTargetPtr RenderEngine::CurRenderTarget() const
	{
		return cur_render_target_;
	}

	// ��ȡĬ����ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderTargetPtr RenderEngine::DefaultRenderTarget() const
	{
		return default_render_target_;
	}

	// ������Ⱦ��Ч
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::SetRenderTechnique(RenderTechniquePtr const & tech)
	{
		render_tech_ = (!tech) ? RenderTechnique::NullObject() : tech;
	}

	// ��ȡ��Ⱦ��Ч
	/////////////////////////////////////////////////////////////////////////////////
	RenderTechniquePtr RenderEngine::GetRenderTechnique() const
	{
		return render_tech_;
	}

	// ������Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::SetRenderState(RenderStateType rst, uint32_t state)
	{
		BOOST_ASSERT(static_cast<size_t>(rst) < render_states_.size());

		if (render_states_[rst] != state)
		{
			render_states_[rst] = state;
			dirty_render_states_[rst] = true;
		}
	}

	// ��ȡ��Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t RenderEngine::GetRenderState(RenderStateType rst)
	{
		return render_states_[rst];
	}

	// ��Ⱦһ��vb
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::Render(RenderLayout const & rl)
	{
		render_tech_->Begin();
		this->DoRender(rl);
		render_tech_->End();
	}

	// �ϴ�Render()����Ⱦ��ͼԪ��
	/////////////////////////////////////////////////////////////////////////////////
	size_t RenderEngine::NumPrimitivesJustRendered()
	{
		size_t const ret = numPrimitivesJustRendered_;
		numPrimitivesJustRendered_ = 0;
		return ret;
	}

	// �ϴ�Render()����Ⱦ�Ķ�����
	/////////////////////////////////////////////////////////////////////////////////
	size_t RenderEngine::NumVerticesJustRendered()
	{
		size_t const ret = numVerticesJustRendered_;
		numVerticesJustRendered_ = 0;
		return ret;
	}

	// ��ȡ��Ⱦ�豸����
	/////////////////////////////////////////////////////////////////////////////////
	RenderDeviceCaps const & RenderEngine::DeviceCaps() const
	{
		return caps_;
	}
}
