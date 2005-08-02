// CameraController.cpp
// KlayGE ������������� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������timer (2005.8.2)
//
// 2.5.0
// AttachCamera��������Update (2005.3.30)
// �Զ��������� (2005.4.3)
//
// 2.4.0
// ���ν��� (2005.3.12)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Input.hpp>
#include <KlayGE/InputFactory.hpp>

#pragma warning(disable: 4512)

#include <boost/assign.hpp>
#include <boost/assign/std/vector.hpp>

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
		timer_.restart();
		camera_ = &camera;
		this->Update();
	}

	void CameraController::DetachCamera()
	{
		camera_ = NULL;
	}

	void CameraController::Update()
	{
	}


	FirstPersonCameraController::FirstPersonCameraController()
	{
		using namespace boost::assign;

		std::vector<InputAction> actions;
		actions += InputAction(TurnLeftRight, MS_X),
					InputAction(TurnUpDown, MS_Y),
					InputAction(Forward, KS_W),
					InputAction(Backward, KS_S),
					InputAction(MoveLeft, KS_A),
					InputAction(MoveRight, KS_D),
					InputAction(Forward, KS_UpArrow),
					InputAction(Backward, KS_DownArrow),
					InputAction(MoveLeft, KS_LeftArrow),
					InputAction(MoveRight, KS_RightArrow);

		InputEngine& inputEngine(Context::Instance().InputFactoryInstance().InputEngineInstance());
		KlayGE::InputActionMap actionMap;
		actionMap.AddActions(actions.begin(), actions.end());
		action_map_id_ = inputEngine.ActionMap(actionMap, true);
	}

	void FirstPersonCameraController::Update()
	{
		BOOST_ASSERT(camera_ != NULL);

		world_ = MathLib::Inverse(camera_->ViewMatrix());

		float const elapsed_time = static_cast<float>(timer_.elapsed());
		if (elapsed_time > 0.01f)
		{
			float const scaler = elapsed_time * 10;

			InputEngine& inputEngine(Context::Instance().InputFactoryInstance().InputEngineInstance());
			InputActionsType actions(inputEngine.Update(action_map_id_));
			for (InputActionsType::iterator iter = actions.begin(); iter != actions.end(); ++ iter)
			{
				switch (iter->first)
				{
				case TurnLeftRight:
					this->Rotate(iter->second * scaler, 0, 0);
					break;

				case TurnUpDown:
					this->Rotate(0, iter->second * scaler, 0);
					break;

				case Forward:
					this->Move(0, 0, scaler);
					break;

				case Backward:
					this->Move(0, 0, -scaler);
					break;

				case MoveLeft:
					this->Move(-scaler, 0, 0);
					break;

				case MoveRight:
					this->Move(scaler, 0, 0);
					break;
				}
			}
		}
		timer_.restart();
	}

	void FirstPersonCameraController::Move(float x, float y, float z)
	{
		BOOST_ASSERT(camera_ != NULL);

		Vector3 movement(x, y, z);
		movement *= moveScaler_;

		Vector3 eyePos = camera_->EyePos();
		Vector3 viewVec = camera_->ViewVec();

		eyePos = MathLib::TransformCoord(movement, world_);

		camera_->ViewParams(eyePos, eyePos + viewVec, camera_->UpVec());
	}

	void FirstPersonCameraController::Rotate(float yaw, float pitch, float roll)
	{
		BOOST_ASSERT(camera_ != NULL);

		Matrix4 rot(MathLib::RotationMatrixYawPitchRoll(yaw * rotationScaler_,
			pitch * rotationScaler_, roll * rotationScaler_));

		Vector3 viewVec = MathLib::TransformCoord(Vector3(0, 0, 1), rot);
		viewVec = MathLib::TransformCoord(viewVec, world_);

		camera_->ViewParams(camera_->EyePos(), viewVec, camera_->UpVec());
	}
}
