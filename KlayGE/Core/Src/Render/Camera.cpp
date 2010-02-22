// Camera.cpp
// KlayGE ������� ʵ���ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2003-2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// ֧��Motion blur (2010.2.22)
//
// 2.0.0
// ���ν���(2003.10.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Math.hpp>

#include <KlayGE/Camera.hpp>

namespace KlayGE
{
	// ���캯��
	//////////////////////////////////////////////////////////////////////////////////
	Camera::Camera()
		: prev_view_mats_(Context::Instance().RenderFactoryInstance().RenderEngineInstance().NumMotionFrames()),
			prev_proj_mats_(Context::Instance().RenderFactoryInstance().RenderEngineInstance().NumMotionFrames())
	{
		// ���ù۲����Ĳ���
		this->ViewParams(float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));

		// ����Ͷ�����Ĳ���
		this->ProjParams(PI / 4, 1, 1, 1000);
	}

	// ����������Ĺ۲����
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ViewParams(float3 const & eyePos, float3 const & lookat,
										float3 const & upVec)
	{
		// ���ù۲����Ĳ���
		eyePos_		= eyePos;
		lookat_		= lookat;
		upVec_		= upVec;

		viewVec_ = MathLib::normalize(lookat_ - eyePos_);
		viewMat_ = MathLib::look_at_lh(eyePos_, lookat_, upVec);
	}

	// �����������Ͷ�����
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ProjParams(float FOV, float aspect, float nearPlane,
											float farPlane)
	{
		// ����Ͷ�����Ĳ���
		FOV_		= FOV;
		aspect_		= aspect;
		nearPlane_	= nearPlane;
		farPlane_	= farPlane;

		projMat_ = MathLib::perspective_fov_lh(FOV, aspect, nearPlane, farPlane);
	}

	void Camera::Update()
	{
		prev_view_mats_.push_back(viewMat_);
		prev_proj_mats_.push_back(projMat_);
	}
}
