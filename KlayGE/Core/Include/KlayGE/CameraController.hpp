// CameraController.hpp
// KlayGE ������������� ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2005-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// FirstPersonCameraControllerȥ�������ǵ����� (2008.1.25)
//
// 2.8.0
// ������timer (2005.8.2)
//
// 2.4.0
// ���ν��� (2005.3.12)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _CAMERACONTROLLER_HPP
#define _CAMERACONTROLLER_HPP

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/Math.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// �����������
	//////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API CameraController
	{
	public:
		CameraController();
		virtual ~CameraController();

		void Scalers(float rotationScaler, float moveScaler);

		virtual void AttachCamera(Camera& camera);
		void DetachCamera();

	protected:
		float		rotationScaler_;	// Scaler for rotation
		float		moveScaler_;		// Scaler for movement

		Camera*		camera_;
	};

	class KLAYGE_CORE_API FirstPersonCameraController : public CameraController
	{
	public:
		FirstPersonCameraController();

		void AttachCamera(Camera& camera);

		void Move(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);

	private:
		float2		rot_x_;
		float2		rot_y_;
		float2		rot_z_;
		Quaternion	inv_rot_;

		enum
		{
			TurnLeftRight,
			TurnUpDown,
			RollLeft,
			RollRight,

			Forward,
			Backward,
			MoveLeft,
			MoveRight
		};

	private:
		void InputHandler(InputEngine const & sender, InputAction const & action);
	};

	class KLAYGE_CORE_API TrackballCameraController : public CameraController
	{
	public:
		TrackballCameraController();

		void AttachCamera(Camera& camera);

	private:
		bool reverse_target_;
		float3 target_;
		float3 right_;

		enum
		{
			Turn
		};

	private:
		void InputHandler(InputEngine const & sender, InputAction const & action);
	};
}

#endif		// _CAMERACONTROLLER_HPP
