// App3D.cpp
// KlayGE App3D�� ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2003-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ����Core (2008.10.16)
//
// 3.7.0
// �Ľ���Update (2007.8.14)
//
// 3.6.0
// ������MakeWindow (2007.6.26)
//
// 3.1.0
// ������OnResize (2005.11.20)
//
// 2.7.0
// ������Quit (2005.6.28)
//
// 2.0.0
// ���ν��� (2003.7.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/Input.hpp>
#include <KlayGE/InputFactory.hpp>
#include <KlayGE/Window.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/UI.hpp>
#include <KlayGE/SceneManager.hpp>

#include <boost/assert.hpp>

#include <KlayGE/App3D.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	App3DFramework::App3DFramework(std::string const & name, RenderSettings const & settings)
						: name_(name), settings_(settings),
							fps_(0), accumulate_time_(0), num_frames_(0)
	{
		Context::Instance().AppInstance(*this);

		main_wnd_ = this->MakeWindow(name_, settings_);
		settings_.left = main_wnd_->Left();
		settings_.top = main_wnd_->Top();
		settings_.width = main_wnd_->Width();
		settings_.height = main_wnd_->Height();
	}

	App3DFramework::~App3DFramework()
	{
		this->Destroy();

		main_wnd_.reset();
	}

	// ����Ӧ�ó���������
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Create()
	{
		Context::Instance().RenderFactoryInstance().RenderEngineInstance().CreateRenderWindow(name_, settings_);

		this->InitObjects();
		this->OnResize(settings_.width, settings_.height);
	}

	void App3DFramework::Destroy()
	{
		this->DelObjects();

		Context::Instance().SceneManagerInstance(SceneManagerPtr());

		UIManager::ForceDestroy();
		Context::Instance().RenderFactoryInstance(RenderFactoryPtr());
	}

	WindowPtr App3DFramework::MakeWindow(std::string const & name, RenderSettings const & settings)
	{
		return MakeSharedPtr<Window>(name, settings);
	}

	void App3DFramework::Run()
	{
		Context::Instance().RenderFactoryInstance().RenderEngineInstance().StartRendering();

		this->DelObjects();
	}

	// ��ȡ��ǰ�����
	/////////////////////////////////////////////////////////////////////////////////
	Camera const & App3DFramework::ActiveCamera() const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		CameraPtr const & camera = re.CurFrameBuffer()->GetViewport().camera;
		BOOST_ASSERT(camera);

		return *camera;
	}

	Camera& App3DFramework::ActiveCamera()
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		CameraPtr const & camera = re.CurFrameBuffer()->GetViewport().camera;
		BOOST_ASSERT(camera);

		return *camera;
	}

	// ���ù۲����
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::LookAt(float3 const & vEye, float3 const & vLookAt)
	{
		this->ActiveCamera().ViewParams(vEye, vLookAt, float3(0, 1, 0));
	}

	void App3DFramework::LookAt(float3 const & vEye, float3 const & vLookAt,
												float3 const & vUp)
	{
		this->ActiveCamera().ViewParams(vEye, vLookAt, vUp);
	}

	// ����Ͷ�����
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Proj(float nearPlane, float farPlane)
	{
		BOOST_ASSERT(nearPlane != 0);
		BOOST_ASSERT(farPlane != 0);
		BOOST_ASSERT(farPlane > nearPlane);

		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		FrameBuffer& fb = *re.CurFrameBuffer();

		this->ActiveCamera().ProjParams(PI / 4, static_cast<float>(fb.Width()) / fb.Height(),
			nearPlane, farPlane);
 	}

	// �˳�����
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Quit()
	{
#ifdef KLAYGE_PLATFORM_WINDOWS
		::PostQuitMessage(0);
#else
		exit(0);
#endif
	}

	// ���³���
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t App3DFramework::Update(uint32_t pass)
	{
		if (0 == pass)
		{
			this->UpdateStats();

			InputEngine& inputEngine = Context::Instance().InputFactoryInstance().InputEngineInstance();
			inputEngine.Update();

			this->DoUpdateOverlay();
		}

		return this->DoUpdate(pass);
	}

	// ��Ӧ���ڴ�С�仯
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::OnResize(uint32_t /*width*/, uint32_t /*height*/)
	{
		this->Proj(this->ActiveCamera().NearPlane(), this->ActiveCamera().FarPlane());
	}

	// ����״̬
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::UpdateStats()
	{
		// measure statistics
		++ num_frames_;
		accumulate_time_ += static_cast<float>(timer_.elapsed());

		// check if new second
		if (accumulate_time_ > 1)
		{
			// new second - not 100% precise
			fps_ = num_frames_ / accumulate_time_;

			accumulate_time_ = 0;
			num_frames_  = 0;
		}

		timer_.restart();
	}

	// ��ȡ��ȾĿ���ÿ��֡��
	/////////////////////////////////////////////////////////////////////////////////
	float App3DFramework::FPS() const
	{
		return fps_;
	}
}
