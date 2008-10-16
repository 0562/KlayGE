// Context.cpp
// KlayGE ���泡���� ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2007-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ������LoadCfg (2008.10.12)
//
// 3.7.0
// ���ν��� (2007.12.19)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/InputFactory.hpp>
#include <KlayGE/ShowFactory.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/RenderSettings.hpp>

#include <fstream>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4251 4275 4512 4702)
#endif
#include <boost/program_options.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <KlayGE/Context.hpp>

#include <KlayGE/D3D9/D3D9RenderFactory.hpp>
#include <KlayGE/D3D10/D3D10RenderFactory.hpp>
#include <KlayGE/OpenGL/OGLRenderFactory.hpp>

#include <KlayGE/OpenAL/OALAudioFactory.hpp>
#include <KlayGE/DSound/DSAudioFactory.hpp>

#include <KlayGE/OCTree/OCTree.hpp>

#include <KlayGE/DInput/DInputFactory.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#include <KlayGE/DShow/DShowFactory.hpp>
#endif

namespace KlayGE
{
	Context::Context()
	{
#ifdef KLAYGE_COMPILER_MSVC
#ifdef KLAYGE_DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

		sceneMgr_ = SceneManager::NullObject();

		renderFactory_ = RenderFactory::NullObject();
		audioFactory_ = AudioFactory::NullObject();
		inputFactory_ = InputFactory::NullObject();
		showFactory_ = ShowFactory::NullObject();
	}

	RenderSettings Context::LoadCfg(std::string const & cfg_file)
	{
		int octree_depth = 3;
		int width = 800;
		int height = 600;
		int color_fmt = EF_ARGB8;
		bool full_screen = false;

		boost::program_options::options_description desc("Configuration");
		desc.add_options()
			("context.render_factory", boost::program_options::value<std::string>(), "Render Factory")
			("context.audio_factory", boost::program_options::value<std::string>(), "Audio Factory")
			("context.input_factory", boost::program_options::value<std::string>(), "Input Factory")
			("context.show_factory", boost::program_options::value<std::string>(), "Show Factory")
			("context.scene_manager", boost::program_options::value<std::string>(), "Scene Manager")
			("octree.depth", boost::program_options::value<int>(&octree_depth)->default_value(3), "Octree depth")
			("screen.width", boost::program_options::value<int>(&width)->default_value(800), "Screen Width")
			("screen.height", boost::program_options::value<int>(&height)->default_value(600), "Screen Height")
			("screen.color_fmt", boost::program_options::value<int>(&color_fmt)->default_value(13), "Screen Color Format")
			("screen.fullscreen", boost::program_options::value<bool>(&full_screen)->default_value(false), "Full Screen");

		std::string rf_name;
		std::string af_name;
		std::string if_name;
		std::string sf_name;
		std::string sm_name;

		std::ifstream cfg_fs(ResLoader::Instance().Locate(cfg_file).c_str());
		if (cfg_fs)
		{
			boost::program_options::variables_map vm;
			boost::program_options::store(boost::program_options::parse_config_file(cfg_fs, desc), vm);
			boost::program_options::notify(vm);

			if (vm.count("context.render_factory"))
			{
				rf_name = vm["context.render_factory"].as<std::string>();
			}
			else
			{
#ifdef KLAYGE_PLATFORM_WINDOWS
				rf_name = "D3D9";
#else
				rf_name = "OpenGL";
#endif
			}

			if (vm.count("context.audio_factory"))
			{
				af_name = vm["context.audio_factory"].as<std::string>();
			}
			else
			{
				af_name = "OpenAL";
			}


			if (vm.count("context.input_factory"))
			{
				if_name = vm["context.input_factory"].as<std::string>();
			}
			else
			{
#ifdef KLAYGE_PLATFORM_WINDOWS
				if_name = "DInput";
#endif
			}

			if (vm.count("context.show_factory"))
			{
#ifdef KLAYGE_COMPILER_MSVC
				// DShow plugin can only be compiled by vc for now
				sf_name = vm["context.show_factory"].as<std::string>();
#endif
			}
			else
			{
#ifdef KLAYGE_PLATFORM_WINDOWS
				sf_name = "DShow";
#endif
			}

			if (vm.count("context.scene_manager"))
			{
				sm_name = vm["context.scene_manager"].as<std::string>();
			}
		}


#ifdef KLAYGE_PLATFORM_WINDOWS
		if ("D3D10" == rf_name)
		{
			Context::Instance().RenderFactoryInstance(D3D10RenderFactoryInstance());
		}
		if ("D3D9" == rf_name)
		{
			Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());
		}
#endif
		if ("OpenGL" == rf_name)
		{
			Context::Instance().RenderFactoryInstance(OGLRenderFactoryInstance());
		}

		if ("OpenAL" == rf_name)
		{
			Context::Instance().AudioFactoryInstance(OALAudioFactoryInstance());
		}
#ifdef KLAYGE_PLATFORM_WINDOWS
		if ("DSound" == rf_name)
		{
			Context::Instance().AudioFactoryInstance(DSAudioFactoryInstance());
		}
#endif

#ifdef KLAYGE_PLATFORM_WINDOWS
		if ("DInput" == if_name)
		{
			Context::Instance().InputFactoryInstance(DInputFactoryInstance());
		}
#endif

#ifdef KLAYGE_PLATFORM_WINDOWS
#ifdef KLAYGE_COMPILER_MSVC
		if ("DShow" == sf_name)
		{
			Context::Instance().ShowFactoryInstance(DShowFactoryInstance());
		}
#endif
#endif

		if ("Octree" == sm_name)
		{
			SceneManagerPtr sm = MakeSharedPtr<OCTree>(octree_depth);
			Context::Instance().SceneManagerInstance(sm);
		}

		RenderSettings settings;
		settings.width = width;
		settings.height = height;
		settings.color_fmt = static_cast<ElementFormat>(color_fmt);
		settings.full_screen = full_screen;

		return settings;
	}
}
