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
#include <KlayGE/Engine.hpp>

#include <KlayGE/Camera.hpp>

namespace KlayGE
{
	// ���캯��
	//////////////////////////////////////////////////////////////////////////////////
	Camera::Camera()
	{
		// ���ù۲����Ĳ���
		this->ViewParams(MakeVector(0.0f, 0.0f, 0.0f), MakeVector(0.0f, 0.0f, 1.0f), MakeVector(0.0f, 1.0f, 0.0f));

		// ����Ͷ�����Ĳ���
		this->ProjParams(PI / 4, 1, 1, 1000);
	}

	// ����������Ĺ۲����
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ViewParams(const Vector3& eyePt, const Vector3& lookatPt,
										const Vector3& upVec)
	{
		// ���ù۲����Ĳ���
		eyePt_		= eyePt;
		lookatPt_	= lookatPt;
		upVec_		= upVec;
		
		Engine::MathInstance().Normalize(viewVec_, lookatPt_ - eyePt_);
		Engine::MathInstance().LookAtLH(viewMat_, eyePt_, lookatPt_, upVec);

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

		Engine::MathInstance().PerspectiveFovLH(projMat_, FOV, aspect, nearPlane, farPlane);
	}

	// �����Ƽ�������Ҫ�ľ���
	//////////////////////////////////////////////////////////////////////////////////
	const Matrix4& Camera::BillboardMatrix()
	{
		if (reEvalBillboard_)
		{
			Engine::MathInstance().Inverse(billboardMat_, viewMat_);

			billboardMat_(3, 0) = 0;
			billboardMat_(3, 1) = 0;
			billboardMat_(3, 2) = 0;
		}

		return billboardMat_;
	}
}
