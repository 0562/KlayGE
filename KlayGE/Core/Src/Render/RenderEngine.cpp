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

#include <KlayGE/RenderEngine.hpp>

namespace KlayGE
{
	RenderEngine::RenderEngine()
		: renderEffect_(RenderEffect::NullObject())
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::~RenderEngine()
	{
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
