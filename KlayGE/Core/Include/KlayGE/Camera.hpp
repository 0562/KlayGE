// Camera.hpp
// KlayGE ������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
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

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// 3D���������
	//////////////////////////////////////////////////////////////////////////////////
	class Camera
	{
	public:
		const Vector3& EyePt() const
			{ return this->eyePt_; }
		const Vector3& LookatPt() const
			{ return this->lookatPt_; }
		const Vector3& UpVec() const
			{ return this->upVec_; }

		const Matrix4& ViewMatrix() const
			{ return this->viewMat_; }
		const Matrix4& BillboardMatrix();
		const Matrix4& ProjMatrix() const
			{ return this->projMat_; }

		void ViewParams(const Vector3& eyePt, const Vector3& lookatPt,
			const Vector3& upVec = Vector3(0, 1, 0));
		void ProjParams(float FOV, float aspect, float nearPlane, float farPlane);

		Camera();

	private:
		Vector3		eyePt_;			// �۲���������
		Vector3		lookatPt_;
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
