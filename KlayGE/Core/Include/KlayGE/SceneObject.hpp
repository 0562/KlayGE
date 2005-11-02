// SceneObject.hpp
// KlayGE ���������� ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.31)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _SCENEOBJECT_HPP
#define _SCENEOBJECT_HPP

#include <KlayGE/PreDeclare.hpp>

#include <boost/smart_ptr.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class SceneObject : public boost::enable_shared_from_this<SceneObject>
	{
	public:
		virtual ~SceneObject();

		RenderablePtr GetRenderable() const;

		virtual Matrix4 GetModelMatrix() const;
		virtual Box GetBound() const;

		virtual void AddToSceneManager();

		virtual bool CanBeCulled() const;
		virtual bool ShortAge() const;

		virtual void const * InstanceData() const;

	protected:
		RenderablePtr renderable_;
	};
}

#endif		// _SCENEOBJECT_HPP
