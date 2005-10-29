// RenderEngine.cpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/VertexBuffer.hpp>
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

		void ClearColor(Color const & clr)
		{
		}

		void ShadingType(ShadeOptions so)
		{
		}

		RenderWindowPtr CreateRenderWindow(std::string const & name, RenderSettings const & settings)
		{
			return RenderWindow::NullObject();
		}

		void CullingMode(CullMode mode)
		{
		}
		void PolygonMode(FillMode mode)
		{
		}

		void AlphaBlend(bool enabled)
		{
		}
		void AlphaBlendFunction(AlphaBlendFactor src_factor, AlphaBlendFactor dst_factor)
		{
		}

		void DepthBufferDepthTest(bool enabled)
		{
		}
		void DepthBufferDepthWrite(bool enabled)
		{
		}
		void DepthBufferFunction(CompareFunction depthFunction)
		{
		}
		void DepthBias(uint16_t bias)
		{
		}

		void AlphaTest(bool enabled)
		{
		}
		void AlphaFunction(CompareFunction alphaFunction, float refValue)
		{
		}

		void SetSampler(uint32_t stage, SamplerPtr const & sampler)
		{
		}
		void DisableSampler(uint32_t stage)
		{
		}

		void StencilCheckEnabled(bool enabled)
		{
		}
		bool HasHardwareStencil()
		{
			return false;
		}

		uint16_t StencilBufferBitDepth()
		{
			return 0;
		}

		void StencilBufferFunction(CompareFunction func, uint32_t refValue, uint32_t mask)
		{
		}
		void StencilBufferOperation(StencilOperation fail, StencilOperation depth_fail, StencilOperation pass)
		{
		}

		void PointSpriteEnable(bool enable)
		{
		}
		void PointDistanceAttenuation(float quadratic0, float quadratic1, float quadratic2)
		{
		}
		void PointSize(float size)
		{
		}
		void PointMinMaxSize(float min_size, float max_size)
		{
		}

		void ScissorTest(bool enabled)
		{
		}
		void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
		}

	private:
		void DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget)
		{
		}

		void DoRender(VertexBuffer const & vb)
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
	void RenderEngine::ActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget)
	{
		BOOST_ASSERT(n < renderTargets_.size());

		renderTargets_[n] = renderTarget;
		this->DoActiveRenderTarget(n, renderTarget);
	}

	// ��ȡ��ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderTargetPtr RenderEngine::ActiveRenderTarget(uint32_t n) const
	{
		BOOST_ASSERT(n < renderTargets_.size());

		return renderTargets_[n];
	}

	// ������Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::SetRenderEffect(RenderEffectPtr const & effect)
	{
		renderEffect_ = (!effect) ? RenderEffect::NullObject() : effect;
	}

	// ��ȡ��Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	RenderEffectPtr RenderEngine::GetRenderEffect() const
	{
		return renderEffect_;
	}

	// ��Ⱦһ��vb
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::Render(VertexBuffer const & vb)
	{
		renderEffect_->Begin();
		this->DoRender(vb);
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
