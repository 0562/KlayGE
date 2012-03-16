// Camera.cpp
// KlayGE ������� ʵ���ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2003-2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ֧��Motion blur (2010.2.22)
// ֧��Stereo (2010.4.2)
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
	enum CameraMode
	{
		CM_Stereo = 1UL << 0,
		CM_Omni = 1UL << 1
	};

	// ���캯��
	//////////////////////////////////////////////////////////////////////////////////
	Camera::Camera()
		: frustum_dirty_(true), mode_(0)
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		uint32_t num_motion_frames = re.NumMotionFrames();
		prev_view_mats_[0].resize(num_motion_frames);
		prev_proj_mats_[0].resize(num_motion_frames);

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
		if (this->StereoMode())
		{
			RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
			float separation = re.StereoSeparation() / 2;
			viewMat_[0] = view_mat * MathLib::translation(+separation, 0.0f, 0.0f);
			viewMat_[1] = view_mat * MathLib::translation(-separation, 0.0f, 0.0f);
			frustum_dirty_ = true;
			frustum_dirty_ = true;
		}
		else
		{
			viewMat_[0] = view_mat;
			frustum_dirty_ = true;
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
		if (this->StereoMode())
		{
			float separation = nearPlane * re.StereoSeparation() / 2;
			float height = 2 * nearPlane * tan(FOV / 2);
			float width = height * aspect;
			projMat_[0] = MathLib::perspective_off_center_lh(-width / 2 + separation, width / 2 + separation, -height / 2, height / 2, nearPlane, farPlane);
			projMat_[1] = MathLib::perspective_off_center_lh(-width / 2 - separation, width / 2 - separation, -height / 2, height / 2, nearPlane, farPlane);
			re.AdjustPerspectiveMatrix(projMat_[0]);
			re.AdjustPerspectiveMatrix(projMat_[1]);
			frustum_dirty_ = true;
		}
		else
		{
			projMat_[0] = MathLib::perspective_fov_lh(FOV, aspect, nearPlane, farPlane);
			re.AdjustPerspectiveMatrix(projMat_[0]);
			frustum_dirty_ = true;
		}
	}

	void Camera::BindUpdateFunc(boost::function<void(Camera&, float, float)> const & update_func)
	{
		update_func_ = update_func;
	}

	void Camera::Update(float app_time, float elapsed_time)
	{
		if (update_func_)
		{
			update_func_(*this, app_time, elapsed_time);
		}

		prev_view_mats_[0].push_back(viewMat_[0]);
		prev_proj_mats_[0].push_back(projMat_[0]);
		if (this->StereoMode())
		{
			prev_view_mats_[1].push_back(viewMat_[1]);
			prev_proj_mats_[1].push_back(projMat_[1]);
		}
	}

	float4x4 const & Camera::ViewMatrix() const
	{
		uint32_t eye = 0;
		if (this->StereoMode())
		{
			RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
			eye = re.StereoActiveEye();
		}
		return viewMat_[eye];
	}

	float4x4 const & Camera::ProjMatrix() const
	{
		uint32_t eye = 0;
		if (this->StereoMode())
		{
			RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
			eye = re.StereoActiveEye();
		}
		return projMat_[eye];
	}

	float4x4 const & Camera::PrevViewMatrix() const
	{
		uint32_t eye = 0;
		if (this->StereoMode())
		{
			RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
			eye = re.StereoActiveEye();
		}
		return prev_view_mats_[eye].front();
	}
	
	float4x4 const & Camera::PrevProjMatrix() const
	{
		uint32_t eye = 0;
		if (this->StereoMode())
		{
			RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
			eye = re.StereoActiveEye();
		}
		return prev_proj_mats_[eye].front();
	}

	Frustum const & Camera::ViewFrustum() const
	{
		uint32_t eye = 0;
		if (this->StereoMode())
		{
			RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
			eye = re.StereoActiveEye();
		}
		if (frustum_dirty_)
		{
			frustum_[0].ClipMatrix(viewMat_[0] * projMat_[0]);
			if (this->StereoMode())
			{
				frustum_[1].ClipMatrix(viewMat_[1] * projMat_[1]);
			}
			frustum_dirty_ = false;
		}
		return frustum_[eye];
	}

	bool Camera::StereoMode() const
	{
		return (mode_ & CM_Stereo) > 0;
	}

	void Camera::StereoMode(bool stereo)
	{
		if (stereo)
		{
			mode_ |= CM_Stereo;

			prev_view_mats_[1].resize(prev_view_mats_[0].size());
			prev_proj_mats_[1].resize(prev_proj_mats_[0].size());
		}
		else
		{
			mode_ &= ~CM_Stereo;
		}
	}

	bool Camera::OmniDirectionalMode() const
	{
		return (mode_ & CM_Omni) > 0;
	}

	void Camera::OmniDirectionalMode(bool omni)
	{
		if (omni)
		{
			mode_ |= CM_Omni;
		}
		else
		{
			mode_ &= ~CM_Omni;
		}
	}
}
