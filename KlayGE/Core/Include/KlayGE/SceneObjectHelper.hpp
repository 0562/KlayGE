// SceneObjectHelper.hpp
// KlayGE һЩ���õĳ������� ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SCENEOBJECTHELPER_HPP
#define _SCENEOBJECTHELPER_HPP

#pragma KLAYGE_ONCE

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneObject.hpp>
#include <KlayGE/Box.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API SceneObjectHelper : public SceneObject
	{
	public:
		SceneObjectHelper(uint32_t attrib);
		SceneObjectHelper(RenderablePtr renderable, uint32_t attrib);
		virtual ~SceneObjectHelper()
		{
		}
	};

	class KLAYGE_CORE_API SceneObjectSkyBox : public SceneObjectHelper
	{
	public:
		SceneObjectSkyBox();
		virtual ~SceneObjectSkyBox()
		{
		}

		void CubeMap(TexturePtr const & cube);
	};
}

#endif		// _RENDERABLEHELPER_HPP
