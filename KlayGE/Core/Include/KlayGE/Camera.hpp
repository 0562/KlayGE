// Camera.hpp
// KlayGE ������� ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2003-2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ֧��Motion blur (2010.2.22)
// ֧��Stereo (2010.4.2)
//
// 2.0.0
// ���ν��� (2003.5.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 6011)
#endif
#include <boost/circular_buffer.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif
#include <boost/function.hpp>

#include <KlayGE/Frustum.hpp>
#include <KlayGE/Vector.hpp>
#include <KlayGE/Matrix.hpp>

namespace KlayGE
{
	// 3D���������
	//////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API Camera
	{
	public:
		Camera();

		float3 const & EyePos() const
			{ return eyePos_; }
		float3 const & LookAt() const
			{ return lookat_; }
		float3 const & UpVec() const
			{ return upVec_; }
		float3 const & ViewVec() const
			{ return viewVec_; }

		float FOV() const
			{ return FOV_; }
		float Aspect() const
			{ return aspect_; }
		float NearPlane() const
			{ return nearPlane_; }
		float FarPlane() const
			{ return farPlane_; }

		void ViewParams(float3 const & eyePos, float3 const & lookat,
			float3 const & upVec = float3(0, 1, 0));
		void ProjParams(float FOV, float aspect, float nearPlane, float farPlane);

		void BindUpdateFunc(boost::function<void(Camera&, float, float)> const & update_func);

		void Update(float app_time, float elapsed_time);

		float4x4 const & ViewMatrix() const;
		float4x4 const & ProjMatrix() const;
		float4x4 const & PrevViewMatrix() const;
		float4x4 const & PrevProjMatrix() const;

		Frustum const & ViewFrustum() const;

		bool StereoMode() const;
		void StereoMode(bool stereo);

		bool OmniDirectionalMode() const;
		void OmniDirectionalMode(bool omni);

	private:
		float3		eyePos_;		// �۲���������
		float3		lookat_;
		float3		upVec_;
		float3		viewVec_;
		float4x4	viewMat_[2];

		float		FOV_;			// Ͷ����������
		float		aspect_;
		float		nearPlane_;
		float		farPlane_;
		float4x4	projMat_[2];

		boost::circular_buffer<float4x4> prev_view_mats_[2];
		boost::circular_buffer<float4x4> prev_proj_mats_[2];

		mutable Frustum	frustum_[2];
		mutable bool	frustum_dirty_;

		uint32_t	mode_;

		boost::function<void(Camera&, float, float)> update_func_;
	};
}

#endif		// _CAMERA_HPP
