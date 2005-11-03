// SceneObject.cpp
// KlayGE ���������� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>

#include <boost/assert.hpp>

#include <KlayGE/SceneObject.hpp>

namespace KlayGE
{
	SceneObject::~SceneObject()
	{
	}

	RenderablePtr SceneObject::GetRenderable() const
	{
		return renderable_;
	}

	Box SceneObject::GetBound() const
	{
		BOOST_ASSERT(renderable_);
		return renderable_->GetBound();
	}

	Matrix4 SceneObject::GetModelMatrix() const
	{
		return Matrix4::Identity();
	}

	void SceneObject::AddToSceneManager()
	{
		Context::Instance().SceneManagerInstance().AddSceneObject(this->shared_from_this());
	}

	bool SceneObject::CanBeCulled() const
	{
		return true;
	}

	bool SceneObject::ShortAge() const
	{
		return false;
	}

	vertex_elements_type const & SceneObject::InstanceFormat() const
	{
		return instance_format_;
	}

	void const * SceneObject::InstanceData() const
	{
		return NULL;
	}
}
