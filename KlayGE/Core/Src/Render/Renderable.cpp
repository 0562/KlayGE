// Renderable.cpp
// KlayGE ����Ⱦ������ ʵ���ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.3.0
// ������Render (2005.1.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>

#include <KlayGE/Renderable.hpp>

namespace KlayGE
{
	Renderable::~Renderable()
	{
	}

	void Renderable::OnRenderBegin()
	{
	}

	void Renderable::OnRenderEnd()
	{
	}

	Matrix4 Renderable::GetWorld() const
	{
		return Matrix4::Identity();
	}

	bool Renderable::CanBeCulled() const
	{
		return true;
	}

	void Renderable::Render()
	{
		Context::Instance().SceneManagerInstance().PushRenderable(this->shared_from_this());
	}
}
