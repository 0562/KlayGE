// SceneManager.cpp
// KlayGE ������������ ʵ���ļ�
// Ver 3.5.0
// ��Ȩ����(C) ������, 2003-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.5.0
// �����˸���techniqueȨ�ص����� (2007.1.14)
//
// 3.1.0
// �Զ�����Instance (2005.11.13)
//
// 3.0.0
// ��֤�˻���˳�� (2005.8.16)
//
// 2.6.0
// ������CanBeCulled��bug (2005.5.26)
//
// 2.4.0
// ������NumObjectsRendered��NumPrimitivesRendered��NumVerticesRendered (2005.3.20)
//
// 2.0.0
// ���ν���(2003.10.1)
//
// 2.0.2
// ��ǿ��PushRenderable (2003.11.25)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/App3D.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/SceneObject.hpp>

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <KlayGE/SceneManager.hpp>

namespace
{
	template <typename T>
	bool cmp_weight(T const & lhs, T const & rhs)
	{
		if (!lhs.first)
		{
			return true;
		}
		else
		{
			if (!rhs.first)
			{
				return false;
			}
			else
			{
				return lhs.first->Weight() < rhs.first->Weight();
			}
		}
	}
}

namespace KlayGE
{
	class NullSceneManager : public SceneManager
	{
	public:
		NullSceneManager()
		{
		}

		void ClipScene(Camera const & /*camera*/)
		{
			visible_objs_ = scene_objs_;
		}

		void Clear()
		{
			scene_objs_.resize(0);
		}

		void DoAddSceneObject(SceneObjectPtr const & obj)
		{
			scene_objs_.push_back(obj);
		}

		SceneObjectsType::iterator DoDelSceneObject(SceneObjectsType::iterator iter)
		{
			return scene_objs_.erase(iter);
		}

	private:
		NullSceneManager(NullSceneManager const & rhs);
		NullSceneManager& operator=(NullSceneManager const & rhs);
	};

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	SceneManager::SceneManager()
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	SceneManager::~SceneManager()
	{
	}

	// ���ؿն���
	/////////////////////////////////////////////////////////////////////////////////
	SceneManagerPtr SceneManager::NullObject()
	{
		static SceneManagerPtr obj(new NullSceneManager);
		return obj;
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::AddSceneObject(SceneObjectPtr const & obj)
	{
		this->DoAddSceneObject(obj);
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::AddRenderable(RenderablePtr const & obj)
	{
		RenderTechniquePtr const & tech = obj->GetRenderTechnique();
		BOOST_AUTO(iter, std::find_if(render_queue_.begin(), render_queue_.end(),
			boost::bind(select1st<RenderQueueType::value_type>(), _1) == tech));
		if (iter != render_queue_.end())
		{
			iter->second.push_back(obj);
		}
		else
		{
			render_queue_.push_back(std::make_pair(tech, RenderItemsType(1, obj)));
		}
	}

	// ���³���������
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::Update()
	{
		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		renderEngine.BeginFrame();

		App3DFramework& app = Context::Instance().AppInstance();
		for (uint32_t i = 0; i < app.NumPasses(); ++ i)
		{
			app.Update(i);
			this->Flush();
		}

		renderEngine.EndFrame();
	}

	// ����Ⱦ�����е�������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::Flush()
	{
		numObjectsRendered_ = 0;
		numRenderablesRendered_ = 0;
		numPrimitivesRendered_ = 0;
		numVerticesRendered_ = 0;

		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		App3DFramework& app = Context::Instance().AppInstance();

		this->ClipScene(app.ActiveCamera());
		numObjectsRendered_ = visible_objs_.size();

		std::vector<std::pair<RenderablePtr, SceneObjectsType> > renderables;
		BOOST_FOREACH(BOOST_TYPEOF(visible_objs_)::const_reference so, visible_objs_)
		{
			RenderablePtr const & renderable = so->GetRenderable();

			size_t i = 0;
			while ((i < renderables.size()) && (renderables[i].first != renderable))
			{
				++ i;
			}

			if (i < renderables.size())
			{
				renderables[i].second.push_back(so);
			}
			else
			{
				renderables.push_back(std::make_pair(renderable, SceneObjectsType(1, so)));
			}
		}
		BOOST_FOREACH(BOOST_TYPEOF(renderables)::const_reference renderable, renderables)
		{
			Renderable& ra(*renderable.first);
			ra.AssignInstances(renderable.second.begin(), renderable.second.end());
			ra.AddToRenderQueue();
		}

		std::sort(render_queue_.begin(), render_queue_.end(), cmp_weight<std::pair<RenderTechniquePtr, RenderItemsType> >);

		BOOST_FOREACH(BOOST_TYPEOF(render_queue_)::reference items, render_queue_)
		{
			renderEngine.SetRenderTechnique(items.first);

			BOOST_FOREACH(BOOST_TYPEOF(items.second)::reference item, items.second)
			{
				item->Render();

				++ numRenderablesRendered_;
				numPrimitivesRendered_ += renderEngine.NumPrimitivesJustRendered();
				numVerticesRendered_ += renderEngine.NumVerticesJustRendered();
			}
		}
		visible_objs_.resize(0);
		render_queue_.resize(0);

		for (BOOST_AUTO(iter, scene_objs_.begin()); iter != scene_objs_.end();)
		{
			if ((*iter)->ShortAge())
			{
				iter = this->DoDelSceneObject(iter);
			}
			else
			{
				++ iter;
			}
		}

		app.RenderOver();
	}

	// ��ȡ��Ⱦ����������
	/////////////////////////////////////////////////////////////////////////////////
	size_t SceneManager::NumObjectsRendered() const
	{
		return numObjectsRendered_;
	}

	// ��ȡ��Ⱦ�Ŀ���Ⱦ��������
	/////////////////////////////////////////////////////////////////////////////////
	size_t SceneManager::NumRenderablesRendered() const
	{
		return numRenderablesRendered_;
	}

	// ��ȡ��Ⱦ��ͼԪ����
	/////////////////////////////////////////////////////////////////////////////////
	size_t SceneManager::NumPrimitivesRendered() const
	{
		return numPrimitivesRendered_;
	}

	// ��ȡ��Ⱦ�Ķ�������
	/////////////////////////////////////////////////////////////////////////////////
	size_t SceneManager::NumVerticesRendered() const
	{
		return numVerticesRendered_;
	}
}
