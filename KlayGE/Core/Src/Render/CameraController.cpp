// CameraController.cpp
// KlayGE ������������� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// AttachCamera��������Update (2005.3.30)
//
// 2.4.0
// ���ν��� (2005.3.12)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/CameraController.hpp>

namespace KlayGE
{
	CameraController::CameraController()
		: rotationScaler_(0.05f), moveScaler_(1)
	{
	}

	CameraController::~CameraController()
	{
	}

	void CameraController::Scalers(float rotationScaler, float moveScaler)
	{
		rotationScaler_ = rotationScaler;
		moveScaler_ = moveScaler;
	}

	void CameraController::AttachCamera(Camera& camera)
	{
		camera_ = &camera;
		this->Update();
	}

	void CameraController::DeattachCamera()
	{
		camera_ = NULL;
	}

	void CameraController::Update()
	{
	}


	FirstPersonCameraController::FirstPersonCameraController()
	{
	}

	void FirstPersonCameraController::Update()
	{
		assert(camera_ != NULL);

		MathLib::Inverse(world_, camera_->ViewMatrix());
	}

	void FirstPersonCameraController::Move(float x, float y, float z)
	{
		assert(camera_ != NULL);

		Vector3 movement(x, y, z);
		movement *= moveScaler_;

		Vector3 eyePos = camera_->EyePos();
		Vector3 viewVec = camera_->ViewVec();

		MathLib::TransformCoord(eyePos, movement, world_);

		camera_->ViewParams(eyePos, eyePos + viewVec, camera_->UpVec());

		this->Update();
	}

	void FirstPersonCameraController::Rotate(float yaw, float pitch, float roll)
	{
		assert(camera_ != NULL);

		Matrix4 rot;
		MathLib::RotationYawPitchRoll(rot, yaw * rotationScaler_, pitch * rotationScaler_, roll * rotationScaler_);

		Vector3 viewVec;
		MathLib::TransformCoord(viewVec, Vector3(0, 0, 1), rot);
		MathLib::TransformCoord(viewVec, viewVec, world_);

		camera_->ViewParams(camera_->EyePos(), viewVec, camera_->UpVec());

		this->Update();
	}
}
