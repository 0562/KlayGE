// 3DApp.cpp
// KlayGE 3DApp�� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#define NOMINMAX

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderWindow.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <cassert>
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
	}

	void App3DFramework::Run()
	{
		Context::Instance().RenderFactoryInstance().RenderEngineInstance().StartRendering();

		this->DelObjects();
	}

	// ��ȡ��ǰ�����
	/////////////////////////////////////////////////////////////////////////////////
	Camera& App3DFramework::ActiveCamera()
	{
		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		RenderTarget& activeRenderTarget(*(*renderEngine.ActiveRenderTarget()));

		return activeRenderTarget.GetViewport().camera;
	}

	// ���ù۲����
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::LookAt(Vector3 const & vEye, Vector3 const & vLookAt,
												Vector3 const & vUp)
	{
		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());

		this->ActiveCamera().ViewParams(vEye, vLookAt, vUp);
		renderEngine.ViewMatrix(this->ActiveCamera().ViewMatrix());
	}

	// ����Ͷ�����
	/////////////////////////////////////////////////////////////////////////////////
	void App3DFramework::Proj(float nearPlane, float farPlane)
	{
		assert(nearPlane != 0);
		assert(farPlane != 0);
		assert(farPlane > nearPlane);

		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		RenderTarget& activeRenderTarget(*(*renderEngine.ActiveRenderTarget()));
		Camera& camera(this->ActiveCamera());

		camera.ProjParams(PI / 4, static_cast<float>(activeRenderTarget.Width()) / activeRenderTarget.Height(),
			nearPlane, farPlane);
		renderEngine.ProjectionMatrix(camera.ProjMatrix());
 	}
}
