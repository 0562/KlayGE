// Camera.cpp
// KlayGE ������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
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
	void Camera::ViewParams(Vector3 const & eyePos, Vector3 const & lookat,
										Vector3 const & upVec)
	{
		// ���ù۲����Ĳ���
		eyePos_		= eyePos;
		lookat_		= lookat;
		upVec_		= upVec;
		
		viewVec_ = MathLib::Normalize(lookat_ - eyePos_);
		viewMat_ = MathLib::LookAtLH(eyePos_, lookat_, upVec);

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

		projMat_ = MathLib::PerspectiveFovLH(FOV, aspect, nearPlane, farPlane);
	}

	// �����Ƽ�������Ҫ�ľ���
	//////////////////////////////////////////////////////////////////////////////////
	Matrix4 const & Camera::BillboardMatrix()
	{
		if (reEvalBillboard_)
		{
			billboardMat_ = MathLib::Inverse(viewMat_);

			billboardMat_(3, 0) = 0;
			billboardMat_(3, 1) = 0;
			billboardMat_(3, 2) = 0;
		}

		return billboardMat_;
	}
}
