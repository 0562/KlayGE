// OCTree.cpp
// KlayGE �˲����� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Vector.hpp>
#include <KlayGE/Plane.hpp>
#include <KlayGE/Camera.hpp>

#include <KlayGE/OCTree/OCTree.hpp>

namespace KlayGE
{
	OCTree::OCTree(Box const & box)
		: root_(box)
	{
	}

	void OCTree::ClipScene(Camera const & camera)
	{
		frustum_.CalculateFrustum(camera.ViewMatrix() * camera.ProjMatrix());

		root_.Clip(frustum_);

		std::vector<RenderablePtr> renderables;
		root_.GetRenderables(renderables);

		renderQueue_.clear();

		for (std::vector<RenderablePtr>::iterator iter = renderables.begin();
			iter != renderables.end(); ++ iter)
		{
			SceneManager::PushRenderable(*iter);
		}

		root_.Clear();
	}

	void OCTree::PushRenderable(RenderablePtr const & obj)
	{
		root_.AddRenderable(obj);
	}
}
