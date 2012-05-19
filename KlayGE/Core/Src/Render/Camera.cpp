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
		CM_Omni = 1UL << 1
	};

	// ���캯��
	//////////////////////////////////////////////////////////////////////////////////
	Camera::Camera()
		: frustum_dirty_(true), mode_(0)
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		uint32_t num_motion_frames = re.NumMotionFrames();
		prev_view_mats_.resize(num_motion_frames);
		prev_proj_mats_.resize(num_motion_frames);

		// ���ù۲����Ĳ���
		this->ViewParams(float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));

		// ����Ͷ�����Ĳ���
		this->ProjParams(PI / 4, 1, 1, 1000);
	}

	// ����������Ĺ۲����
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ViewParams(float3 const & eye_pos, float3 const & look_at)
	{
		this->ViewParams(eye_pos, look_at, float3(0, 1, 0));
	}

	void Camera::ViewParams(float3 const & eye_pos, float3 const & look_at,
										float3 const & up_vec)
	{
		// ���ù۲����Ĳ���
		eye_pos_	= eye_pos;
		look_at_	= look_at;
		up_vec_		= up_vec;

		view_vec_ = MathLib::normalize(look_at_ - eye_pos_);
		view_mat_ = MathLib::look_at_lh(eye_pos_, look_at_, up_vec);
		frustum_dirty_ = true;
	}

	// �����������Ͷ�����
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ProjParams(float fov, float aspect, float near_plane, float far_plane)
	{
		// ����Ͷ�����Ĳ���
		fov_		= fov;
		aspect_		= aspect;
		near_plane_	= near_plane;
		far_plane_	= far_plane;

		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		proj_mat_ = MathLib::perspective_fov_lh(fov, aspect, near_plane, far_plane);
		re.AdjustPerspectiveMatrix(proj_mat_);
		frustum_dirty_ = true;
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

		prev_view_mats_.push_back(view_mat_);
		prev_proj_mats_.push_back(proj_mat_);
	}

	float4x4 const & Camera::ViewMatrix() const
	{
		return view_mat_;
	}

	float4x4 const & Camera::ProjMatrix() const
	{
		return proj_mat_;
	}

	float4x4 const & Camera::PrevViewMatrix() const
	{
		return prev_view_mats_.front();
	}
	
	float4x4 const & Camera::PrevProjMatrix() const
	{
		return prev_proj_mats_.front();
	}

	Frustum const & Camera::ViewFrustum() const
	{
		if (frustum_dirty_)
		{
			frustum_.ClipMatrix(view_mat_ * proj_mat_);
			frustum_dirty_ = false;
		}
		return frustum_;
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
