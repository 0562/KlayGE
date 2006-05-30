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

		void ClearColor(Color const & /*clr*/)
		{
		}
		void Clear(uint32_t /*masks*/)
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
		: renderEffect_(RenderEffect::NullObject())
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
		if (!rt)
		{
			cur_render_target_ = default_render_window_;
		}
		else
		{
			cur_render_target_ = rt;
		}

		this->DoBindRenderTarget(cur_render_target_);
	}

	// ��ȡ��ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderTargetPtr RenderEngine::CurRenderTarget() const
	{
		return cur_render_target_;
	}

	// ������Ⱦ��Ч
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::SetRenderEffect(RenderEffectPtr const & effect)
	{
		renderEffect_ = (!effect) ? RenderEffect::NullObject() : effect;
	}

	// ��ȡ��Ⱦ��Ч
	/////////////////////////////////////////////////////////////////////////////////
	RenderEffectPtr RenderEngine::GetRenderEffect() const
	{
		return renderEffect_;
	}

	// ������Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::SetRenderState(RenderStateType rst, uint32_t state)
	{
		BOOST_ASSERT(rst < render_states_.size());

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
		this->DoFlushRenderStates();

		renderEffect_->Begin();
		this->DoRender(rl);
		renderEffect_->End();
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
