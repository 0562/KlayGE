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

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderLayout.hpp>

#include <boost/smart_ptr.hpp>

namespace KlayGE
{
	class SceneObject : public boost::enable_shared_from_this<SceneObject>
	{
	public:
		enum SOAttrib
		{
			SOA_Cullable = 1UL << 0,
			SOA_ShortAge = 1UL << 1,
			SOA_Moveable = 1UL << 2
		};

	public:
		explicit SceneObject(uint32_t attrib);
		virtual ~SceneObject();

		RenderablePtr GetRenderable() const;

		virtual float4x4 GetModelMatrix() const;
		virtual Box GetBound() const;

		virtual void AddToSceneManager();

		bool Cullable() const;
		bool ShortAge() const;
		bool Moveable() const;

		vertex_elements_type const & InstanceFormat() const;
		virtual void const * InstanceData() const;

	protected:
		uint32_t attrib_;

		RenderablePtr renderable_;
		vertex_elements_type instance_format_;
	};
}

#endif		// _SCENEOBJECT_HPP
