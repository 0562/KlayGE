// SceneManager.cpp
// KlayGE ������������ ʵ���ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/Context.hpp>
#include <KlayGE/App3D.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Renderable.hpp>

#include <KlayGE/SceneManager.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	SceneManager::SceneManager()
	{
	}

	// �յĲü���
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::ClipScene(Camera const & /*camera*/)
	{
		for (RenderItemsType::iterator iter = renderItems_.begin(); iter != renderItems_.end(); ++ iter)
		{
			this->AddToRenderQueue(*iter);
		}
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::AddRenderable(RenderablePtr const & obj)
	{
		if (obj->CanBeCulled())
		{
			this->DoAddRenderable(obj);
		}
		else
		{
			this->AddToRenderQueue(obj);
		}
	}

	void SceneManager::DoAddRenderable(RenderablePtr const & obj)
	{
		renderItems_.push_back(obj);
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::AddToRenderQueue(RenderablePtr const & obj)
	{
		RenderEffectPtr const & effect(obj->GetRenderEffect());

		RenderQueueType::iterator iter(renderQueue_.find(effect));
		if (iter != renderQueue_.end())
		{
			iter->second.push_back(obj);
		}
		else
		{
			renderQueue_.insert(RenderQueueType::value_type(effect,
				RenderItemsType(1, obj)));
		}
	}

	// ��ճ���������
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::Clear()
	{
		renderItems_.clear();
	}

	// ���³���������
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::Update()
	{
		Context::Instance().AppInstance().Update();

		this->Flush();
	}

	// ����Ⱦ�����е�������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	void SceneManager::Flush()
	{
		numObjectsRendered_ = 0;
		numPrimitivesRendered_ = 0;
		numVerticesRendered_ = 0;

		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());

		this->ClipScene((*renderEngine.ActiveRenderTarget())->GetViewport().camera);

		renderEngine.BeginFrame();

		for (RenderQueueType::iterator queueIter = renderQueue_.begin();
			queueIter != renderQueue_.end(); ++ queueIter)
		{
			renderEngine.SetRenderEffect(queueIter->first);

			for (RenderItemsType::iterator itemIter = queueIter->second.begin();
				itemIter != queueIter->second.end(); ++ itemIter)
			{
				Renderable& obj(*(*itemIter));

				++ numObjectsRendered_;

				obj.OnRenderBegin();
				renderEngine.Render(*obj.GetVertexBuffer());
				obj.OnRenderEnd();

				numPrimitivesRendered_ += renderEngine.NumPrimitivesJustRendered();
				numVerticesRendered_ += renderEngine.NumVerticesJustRendered();
			}
		}
		renderQueue_.clear();

		renderEngine.EndFrame();

		Context::Instance().AppInstance().RenderOver();
	}

	// ��ȡ��Ⱦ����������
	/////////////////////////////////////////////////////////////////////////////////
	size_t SceneManager::NumObjectsRendered() const
	{
		return numObjectsRendered_;
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
