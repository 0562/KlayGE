// CameraController.hpp
// KlayGE ������������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
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

#include <KlayGE/Math.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/Input.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// �����������
	//////////////////////////////////////////////////////////////////////////////////
	class CameraController
	{
	public:
		CameraController();
		virtual ~CameraController();

		void Scalers(float rotationScaler, float moveScaler);

		void AttachCamera(Camera& camera);
		void DetachCamera();

		virtual void Update();

	protected:
		float		rotationScaler_;	// Scaler for rotation
		float		moveScaler_;		// Scaler for movement

		Camera*		camera_;

		Timer timer_;
	};

	class FirstPersonCameraController : public CameraController
	{
	public:
		FirstPersonCameraController();

		void Update();

		void Move(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);

	private:
		Matrix4		world_;				// World matrix of the camera (inverse of the view matrix)
		float elapsed_time_;

		enum
		{
			TurnLeftRight,
			TurnUpDown,

			Forward,
			Backward,
			MoveLeft,
			MoveRight,
		};

	private:
		void InputHandler(InputEngine const & sender, InputAction const & action);
	};
}

#endif		// _CAMERACONTROLLER_HPP
