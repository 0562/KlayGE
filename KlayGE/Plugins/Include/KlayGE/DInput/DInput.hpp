// DInput.hpp
// KlayGE DirectInput���������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ��Ϊ��̳нṹ (2005.8.11)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DINPUT_HPP
#define _DINPUT_HPP

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

#include <KlayGE/Input.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

#include <KlayGE/DInput/DInputDevice.hpp>

namespace KlayGE
{
	// ���������豸
	/////////////////////////////////////////////////////////////////////////////////
	class DInputEngine : boost::noncopyable, public InputEngine
	{
	public:
		DInputEngine();
		~DInputEngine();

		std::wstring const & Name() const;
		void EnumDevices();

		boost::shared_ptr<IDirectInput8W> const & DInput() const;
		HWND HWnd() const;

	private:
		boost::shared_ptr<IDirectInput8W> dinput_;
		HWND hwnd_;

	private:
		static BOOL CALLBACK EnumDevicesCB(LPCDIDEVICEINSTANCEW didi, void* pvRef);
	};

	class DInputKeyboard : public InputKeyboard, public DInputDevice
	{
	public:
		DInputKeyboard(REFGUID guid, InputEngine const & inputEng);

		std::wstring const & Name() const;

		void Acquire();
		void Unacquire();

	private:
		void UpdateInputs();
	};

	class DInputMouse : public InputMouse, public DInputDevice
	{
	public:
		DInputMouse(REFGUID guid, InputEngine const & inputEng);

		std::wstring const & Name() const;

		void Acquire();
		void Unacquire();

	private:
		void UpdateInputs();
	};

	class DInputJoystick : public InputJoystick, public DInputDevice
	{
	public:
		DInputJoystick(REFGUID guid, InputEngine const & inputEng);

		std::wstring const & Name() const;

		void Acquire();
		void Unacquire();

	private:
		void UpdateInputs();
	};
}

#endif		// _DINPUT_HPP
