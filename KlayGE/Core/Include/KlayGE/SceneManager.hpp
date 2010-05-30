// SceneManager.hpp
// KlayGE ������������ ͷ�ļ�
// Ver 3.9.0
// ��Ȩ����(C) ������, 2003-2009
// Homepage: http://klayge.sourceforge.net
//
// 3.9.0
// ������SceneObjects (2009.7.30)
//
// 2.4.0
// ������NumObjectsRendered��NumPrimitivesRendered��NumVerticesRendered (2005.3.20)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SCENEMANAGER_HPP
#define _SCENEMANAGER_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/Renderable.hpp>
#include <KlayGE/MapVector.hpp>
#include <KlayGE/Frustum.hpp>

#include <vector>

#include <boost/noncopyable.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API SceneManager : boost::noncopyable
	{
	public:
		typedef std::vector<SceneObjectPtr> SceneObjectsType;

	protected:
		typedef std::vector<RenderablePtr> RenderItemsType;
		typedef std::vector<std::pair<RenderTechniquePtr, RenderItemsType> > RenderQueueType;

	public:
		SceneManager();
		virtual ~SceneManager();

		static SceneManagerPtr NullObject();

		virtual void ClipScene();
		void AddSceneObject(SceneObjectPtr const & obj);
		void AddRenderable(RenderablePtr const & obj);

		SceneObjectsType& SceneObjects();
		SceneObjectsType const & SceneObjects() const;

		virtual bool AABBVisible(Box const & box);

		virtual void Clear() = 0;

		void Update();

		size_t NumObjectsRendered() const;
		size_t NumRenderablesRendered() const;
		size_t NumPrimitivesRendered() const;
		size_t NumVerticesRendered() const;

	protected:
		void Flush();

		virtual void DoAddSceneObject(SceneObjectPtr const & obj) = 0;
		virtual SceneObjectsType::iterator DoDelSceneObject(SceneObjectsType::iterator iter) = 0;

	protected:
		Frustum frustum_;
		SceneObjectsType scene_objs_;
		std::vector<char> visible_marks_;
		uint32_t urt_;

	private:
		void FlushScene();

	private:
		RenderQueueType render_queue_;

		size_t numObjectsRendered_;
		size_t numRenderablesRendered_;
		size_t numPrimitivesRendered_;
		size_t numVerticesRendered_;
	};
}

#endif			// _SCENEMANAGER_HPP
