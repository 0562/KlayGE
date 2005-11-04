// SceneObjectHelper.cpp
// KlayGE һЩ���õĿ���Ⱦ���� ʵ���ļ�
// Ver 2.7.1
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.1
// ������RenderableHelper���� (2005.7.10)
//
// 2.6.0
// ������RenderableSkyBox (2005.5.26)
//
// 2.5.0
// ������RenderablePoint��RenderableLine��RenderableTriangle (2005.4.13)
//
// 2.4.0
// ���ν��� (2005.3.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/RenderableHelper.hpp>

#include <boost/tuple/tuple.hpp>

#include <KlayGE/SceneObjectHelper.hpp>

namespace KlayGE
{
	SceneObjectHelper::SceneObjectHelper(uint32_t attrib)
		: SceneObject(attrib)
	{
	}

	SceneObjectHelper::SceneObjectHelper(RenderablePtr renderable, uint32_t attrib)
		: SceneObject(attrib)
	{
		renderable_ = renderable;
	}

	SceneObjectSkyBox::SceneObjectSkyBox()
		: SceneObjectHelper(RenderablePtr(new RenderableSkyBox), 0)
	{
	}

	void SceneObjectSkyBox::CubeMap(TexturePtr const & cube)
	{
		checked_cast<RenderableSkyBox*>(renderable_.get())->CubeMap(cube);
	}
}
