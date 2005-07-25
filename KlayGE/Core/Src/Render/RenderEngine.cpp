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
		: renderEffect_(RenderEffect::NullObject()),
			renderPasses_(1),
			worldMat_(Matrix4::Identity()),
			viewMat_(Matrix4::Identity()),
			projMat_(Matrix4::Identity())
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::~RenderEngine()
	{
	}

	// ������ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::RenderTargetListIterator RenderEngine::AddRenderTarget(RenderTargetPtr const & target)
	{
		renderTargetList_.push_back(target);
		RenderTargetListIterator iter(renderTargetList_.end());
		-- iter;

		return iter;
	}

	// ��ȾĿ���б��Begin������
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::RenderTargetListIterator RenderEngine::RenderTargetListBegin()
	{
		return renderTargetList_.begin();
	}

	// ��ʾĿ���б��End������
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::RenderTargetListIterator RenderEngine::RenderTargetListEnd()
	{
		return renderTargetList_.end();
	}

	// ����ȾĿ���б���ɾ����ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderTargetPtr RenderEngine::RemoveRenderTarget(RenderTargetListIterator iter)
	{
		RenderTargetPtr ret(*iter);
		renderTargetList_.erase(iter);
		return ret;
	}

	// ���õ�ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::ActiveRenderTarget(RenderTargetListIterator iter)
	{
		activeRenderTarget_ = iter;
	}

	// ��ȡ��ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	RenderEngine::RenderTargetListIterator const & RenderEngine::ActiveRenderTarget() const
	{
		return activeRenderTarget_;
	}

	// ������Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::SetRenderEffect(RenderEffectPtr const & effect)
	{
		if (renderEffect_ != effect)
		{
			renderEffect_->End();
			renderEffect_ = (!effect) ? RenderEffect::NullObject() : effect;
			renderPasses_ = renderEffect_->Begin();
		}
	}

	// ��ȡ��Ⱦ״̬
	/////////////////////////////////////////////////////////////////////////////////
	RenderEffectPtr RenderEngine::GetRenderEffect() const
	{
		return renderEffect_;
	}
	
	// ��ȡ�������
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 RenderEngine::WorldMatrix() const
	{
		return worldMat_;
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::WorldMatrix(Matrix4 const & mat)
	{
		worldMat_ = mat;
		this->DoWorldMatrix();
	}

	// ��ȡ�۲����
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 RenderEngine::ViewMatrix() const
	{
		return viewMat_;
	}

	// ���ù۲����
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::ViewMatrix(Matrix4 const & mat)
	{
		viewMat_ = mat;
		this->DoViewMatrix();
	}

	// ��ȡͶ�����
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 RenderEngine::ProjectionMatrix() const
	{
		return projMat_;
	}

	// ����Ͷ�����
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::ProjectionMatrix(Matrix4 const & mat)
	{
		projMat_ = mat;
		this->DoProjectionMatrix();
	}

	// ��Ⱦһ��vb
	/////////////////////////////////////////////////////////////////////////////////
	void RenderEngine::Render(VertexBuffer const & vb)
	{
		renderEffect_->FlushParams();

		this->DoRender(vb);
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
