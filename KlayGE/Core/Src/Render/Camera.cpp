// Camera.cpp
// KlayGE ������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν���(2003.10.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>

#include <KlayGE/Camera.hpp>

namespace KlayGE
{
	// ���캯��
	//////////////////////////////////////////////////////////////////////////////////
	Camera::Camera()
	{
		// ���ù۲����Ĳ���
		this->ViewParams(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));

		// ����Ͷ�����Ĳ���
		this->ProjParams(PI / 4, 1, 1, 1000);
	}

	// ����������Ĺ۲����
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ViewParams(const Vector3& eyePos, const Vector3& lookat,
										const Vector3& upVec)
	{
		// ���ù۲����Ĳ���
		eyePos_		= eyePos;
		lookat_		= lookat;
		upVec_		= upVec;
		
		MathLib::Normalize(viewVec_, lookat_ - eyePos_);
		MathLib::LookAtLH(viewMat_, eyePos_, lookat_, upVec);

		reEvalBillboard_ = true;
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

		MathLib::PerspectiveFovLH(projMat_, FOV, aspect, nearPlane, farPlane);
	}

	// �����Ƽ�������Ҫ�ľ���
	//////////////////////////////////////////////////////////////////////////////////
	const Matrix4& Camera::BillboardMatrix()
	{
		if (reEvalBillboard_)
		{
			MathLib::Inverse(billboardMat_, viewMat_);

			billboardMat_(3, 0) = 0;
			billboardMat_(3, 1) = 0;
			billboardMat_(3, 2) = 0;
		}

		return billboardMat_;
	}
}
