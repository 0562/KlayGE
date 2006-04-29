// Camera.hpp
// KlayGE ������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.5.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <KlayGE/Vector.hpp>
#include <KlayGE/Matrix.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// 3D���������
	//////////////////////////////////////////////////////////////////////////////////
	class Camera
	{
	public:
		Vector3 const & EyePos() const
			{ return eyePos_; }
		Vector3 const & LookAt() const
			{ return lookat_; }
		Vector3 const & UpVec() const
			{ return upVec_; }
		Vector3 const & ViewVec() const
			{ return viewVec_; }

		float FOV() const
			{ return FOV_; }
		float Aspect() const
			{ return aspect_; }
		float NearPlane() const
			{ return nearPlane_; }
		float FarPlane() const
			{ return farPlane_; }

		Matrix4 const & ViewMatrix() const
			{ return this->viewMat_; }
		Matrix4 const & BillboardMatrix();
		Matrix4 const & ProjMatrix() const
			{ return this->projMat_; }

		void ViewParams(Vector3 const & eyePos, Vector3 const & lookat,
			Vector3 const & upVec = Vector3(0, 1, 0));
		void ProjParams(float FOV, float aspect, float nearPlane, float farPlane);

		Camera();

	private:
		Vector3		eyePos_;			// �۲���������
		Vector3		lookat_;
		Vector3		upVec_;
		Vector3		viewVec_;
		Matrix4		viewMat_;

		bool		reEvalBillboard_;
		Matrix4		billboardMat_;

		float		FOV_;			// Ͷ����������
		float		aspect_;
		float		nearPlane_;
		float		farPlane_;
		Matrix4		projMat_;
	};
}

#endif		// _CAMERA_HPP
