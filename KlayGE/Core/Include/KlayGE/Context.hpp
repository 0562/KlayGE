// Contex.hpp
// KlayGE ���泡���� ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2007-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ������LoadCfg (2008.10.12)
//
// 2.4.0
// ������BOOST_ASSERT
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

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>

#include <string>
#include <boost/assert.hpp>

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

		RenderSettings LoadCfg(std::string const & cfg_file);

		void AppInstance(App3DFramework& app)
		{
			app_ = &app;
		}
		App3DFramework& AppInstance()
		{
			BOOST_ASSERT(app_);
			return *app_;
		}

		void SceneManagerInstance(SceneManagerPtr const & mgr)
		{
			sceneMgr_ = mgr;
		}
		SceneManager& SceneManagerInstance()
		{
			BOOST_ASSERT(sceneMgr_);
			return *sceneMgr_;
		}

		void RenderFactoryInstance(RenderFactoryPtr const & factory)
		{
			renderFactory_ = factory;
		}
		RenderFactory& RenderFactoryInstance()
		{
			BOOST_ASSERT(renderFactory_);
			return *renderFactory_;
		}

		void AudioFactoryInstance(AudioFactoryPtr const & factory)
		{
			audioFactory_ = factory;
		}
		AudioFactory& AudioFactoryInstance()
		{
			BOOST_ASSERT(audioFactory_);
			return *audioFactory_;
		}

		void InputFactoryInstance(InputFactoryPtr const & factory)
		{
			inputFactory_ = factory;
		}
		InputFactory& InputFactoryInstance()
		{
			BOOST_ASSERT(inputFactory_);
			return *inputFactory_;
		}

		void ShowFactoryInstance(ShowFactoryPtr const & factory)
		{
			showFactory_ = factory;
		}
		ShowFactory& ShowFactoryInstance()
		{
			BOOST_ASSERT(showFactory_);
			return *showFactory_;
		}

	private:
		Context();

		App3DFramework*		app_;

		SceneManagerPtr		sceneMgr_;

		RenderFactoryPtr	renderFactory_;
		AudioFactoryPtr		audioFactory_;
		InputFactoryPtr		inputFactory_;
		ShowFactoryPtr		showFactory_;
	};
}

#endif		// _CONTEXT_HPP
