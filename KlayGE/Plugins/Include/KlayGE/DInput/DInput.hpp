// DInput.hpp
// KlayGE DirectInput���������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
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

#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL          0x80000000
#endif

#include <KlayGE/Input.hpp>
#include <boost/noncopyable.hpp>

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

		HMODULE mod_dinput8_;
		typedef HRESULT (WINAPI *DirectInput8CreateFunc)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
		DirectInput8CreateFunc DynamicDirectInput8Create_;

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
