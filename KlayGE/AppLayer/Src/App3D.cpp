// App3D.cpp
// KlayGE App3D�� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
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

#define NOMINMAX

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderWindow.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Input.hpp>
#include <KlayGE/InputFactory.hpp>

#include <boost/assert.hpp>
#include <windows.h>

#include <KlayGE/App3D.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	App3DFramework::App3DFramework()
	{
		Context::Instance().AppInstance(*this);
	}

	App3DFramework::~App3DFramework()
	{
		this->DelObjects();
	}

	// ����Ӧ�ó��򴰿ں�D3D�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Create(std::string const & name, RenderSettings const & settings)
	{
		Context::Instance().RenderFactoryInstance().RenderEngineInstance().CreateRenderWindow(name, settings);

		this->InitObjects();
		this->OnResize(settings.width, settings.height);
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
		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		RenderTarget& activeRenderTarget(*renderEngine.CurRenderTarget());
		CameraPtr camera = activeRenderTarget.GetViewport().camera;
		BOOST_ASSERT(camera);

		return *camera;
	}

	Camera& App3DFramework::ActiveCamera()
	{
		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		RenderTarget& activeRenderTarget(*renderEngine.CurRenderTarget());
		CameraPtr camera = activeRenderTarget.GetViewport().camera;
		BOOST_ASSERT(camera);

		return *camera;
	}

	// ���ù۲����
	/////////////////////////////////////////////////////////////////////////////////
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

		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		RenderTarget& activeRenderTarget(*renderEngine.CurRenderTarget());

		this->ActiveCamera().ProjParams(PI / 4, static_cast<float>(activeRenderTarget.Width()) / activeRenderTarget.Height(),
			nearPlane, farPlane);
 	}

	// �˳�����
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Quit()
	{
#ifdef WIN32
		::PostQuitMessage(0);
#else
		exit(0);
#endif
	}

	// ���³���
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Update(uint32_t pass)
	{
		if (0 == pass)
		{
			InputEngine& inputEngine = Context::Instance().InputFactoryInstance().InputEngineInstance();
			inputEngine.Update();
		}

		this->DoUpdate(pass);
	}

	// ��Ӧ���ڴ�С�仯
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::OnResize(uint32_t /*width*/, uint32_t /*height*/)
	{
	}
}
