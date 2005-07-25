// SceneManager.hpp
// KlayGE ������������ ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ������NumObjectsRendered��NumPrimitivesRendered��NumVerticesRendered (2005.3.20)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SCENEMANAGER_HPP
#define _SCENEMANAGER_HPP

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/Renderable.hpp>
#include <KlayGE/MapVector.hpp>

#include <vector>

#include <boost/utility.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class SceneManager : boost::noncopyable
	{
	protected:
		typedef std::vector<RenderablePtr> RenderItemsType;
		typedef MapVector<uint32_t, RenderItemsType> RenderQueueType;

	public:
		SceneManager();

		virtual void ClipScene(Camera const & camera);
		void AddRenderable(RenderablePtr const & obj);

		virtual void Clear();

		void Update();
		void Flush();

		size_t NumObjectsRendered() const;
		size_t NumPrimitivesRendered() const;
		size_t NumVerticesRendered() const;

	protected:
		void AddToRenderQueue(RenderablePtr const & obj);

		virtual void DoAddRenderable(RenderablePtr const & obj);

	protected:
		RenderItemsType renderItems_;

	private:
		RenderQueueType renderQueue_;

		size_t numObjectsRendered_;
		size_t numPrimitivesRendered_;
		size_t numVerticesRendered_;
	};
}

#endif			// _SCENEMANAGER_HPP
