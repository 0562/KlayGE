// Contex.hpp
// KlayGE ���泡���� ͷ�ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.nets
//
// 2.1.1
// ��������
//
// 2.0.0
// ���ν��� (2003.7.23)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _CONTEXT_HPP
#define _CONTEXT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneManager.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	class Context
	{
	public:
		static Context& Instance()
		{
			static Context context;
			return context;
		}

		void AppInstance(App3DFramework& app)
			{ app_ = &app; }
		App3DFramework& AppInstance()
			{ return *app_; }

		void SceneManagerInstance(SceneManager& mgr)
			{ sceneMgr_ = &mgr; }
		SceneManager& SceneManagerInstance()
			{ return *sceneMgr_; }

		void RenderFactoryInstance(RenderFactory& factory)
			{ renderFactory_ = &factory; }
		RenderFactory& RenderFactoryInstance()
			{ return *renderFactory_; }

		void AudioFactoryInstance(AudioFactory& factory)
			{ audioFactory_ = &factory; }
		AudioFactory& AudioFactoryInstance()
			{ return *audioFactory_; }

		void InputFactoryInstance(InputFactory& factory)
			{ inputFactory_ = &factory; }
		InputFactory& InputFactoryInstance()
			{ return *inputFactory_; }

	private:
		Context()
			{ }

		App3DFramework* app_;

		SceneManager* sceneMgr_;

		RenderFactory*	renderFactory_;
		AudioFactory*	audioFactory_;
		InputFactory*	inputFactory_;
	};
}

#endif		// _CONTEXT_HPP
