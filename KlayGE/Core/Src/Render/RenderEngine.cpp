// RenderEngine.cpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/RenderBuffer.hpp>
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
	

	// ��ȡ���������
	/////////////////////////////////////////////////////////////////////////////////
	U32 RenderEngine::MaxVertexBlendMatrices()
	{
		// TODO: implement vertex blending support in DX8 & possibly GL_ARB_VERTEX_BLEND (in subclasses)
		return 1;
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
	Matrix4 RenderEngine::ViewMatrix()
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
	Matrix4 RenderEngine::ProjectionMatrix()
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
}
