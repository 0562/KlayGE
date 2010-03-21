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
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		uint32_t num_motion_frames = re.NumMotionFrames();
		prev_view_mats_[0].resize(num_motion_frames);
		prev_proj_mats_[0].resize(num_motion_frames);
		prev_view_mats_[1].resize(num_motion_frames);
		prev_proj_mats_[1].resize(num_motion_frames);

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
		float4x4 view_mat = MathLib::look_at_lh(eyePos_, lookat_, upVec);
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		if (re.StereoMode())
		{
			float separation = re.StereoSeparation();
			viewMat_[0] = view_mat * MathLib::translation(-separation, 0.0f, 0.0f);
			viewMat_[1] = view_mat * MathLib::translation(+separation, 0.0f, 0.0f);
		}
		else
		{
			viewMat_[0] = view_mat;
		}
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

		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		if (re.StereoMode())
		{
			float separation = re.StereoSeparation() * 0.005f;
			float height = 2 * nearPlane * tan(FOV / 2);
			float width = height * aspect;
			projMat_[0] = MathLib::perspective_off_center_lh(-width / 2 + separation, width / 2 + separation, -height / 2, height / 2, nearPlane, farPlane);
			projMat_[1] = MathLib::perspective_off_center_lh(-width / 2 - separation, width / 2 - separation, -height / 2, height / 2, nearPlane, farPlane);
		}
		else
		{
			projMat_[0] = MathLib::perspective_fov_lh(FOV, aspect, nearPlane, farPlane);
		}
	}

	void Camera::Update()
	{
		prev_view_mats_[0].push_back(viewMat_[0]);
		prev_proj_mats_[0].push_back(projMat_[0]);
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		if (re.StereoMode())
		{
			prev_view_mats_[1].push_back(viewMat_[1]);
			prev_proj_mats_[1].push_back(projMat_[1]);
		}
	}

	float4x4 const & Camera::ViewMatrix() const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		return viewMat_[re.StereoActiveEye()];
	}

	float4x4 const & Camera::ProjMatrix() const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		return projMat_[re.StereoActiveEye()];
	}

	float4x4 const & Camera::PrevViewMatrix() const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		return prev_view_mats_[re.StereoActiveEye()].front();
	}
	
	float4x4 const & Camera::PrevProjMatrix() const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		return prev_proj_mats_[re.StereoActiveEye()].front();
	}
}
