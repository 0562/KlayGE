// DInput.hpp
// KlayGE DirectInput���������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
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

#pragma comment(lib, "KlayGE_InputEngine_DInput.lib")

namespace KlayGE
{
	// ���������豸
	/////////////////////////////////////////////////////////////////////////////////
	class DInputEngine : boost::noncopyable, public InputEngine
	{
	public:
		DInputEngine();
		~DInputEngine();

		const std::wstring& Name() const;
		void EnumDevices();

		const boost::shared_ptr<IDirectInput8W>& DInput() const;

	private:
		boost::shared_ptr<IDirectInput8W> dinput_;

	private:
		static BOOL CALLBACK EnumDevicesCB(LPCDIDEVICEINSTANCEW didi, void* pvRef);
	};

	class DInputKeyboard : public InputKeyboard
	{
	public:
		DInputKeyboard(REFGUID guid, InputEngine& inputEng);
		~DInputKeyboard();

		const std::wstring& Name() const;

		void Acquire();
		void Unacquire();

	private:
		void UpdateKeys();

		boost::shared_ptr<IDirectInputDevice8W> device_;
	};

	class DInputMouse : public InputMouse
	{
	public:
		DInputMouse(REFGUID guid, InputEngine& inputEng);
		~DInputMouse();

		const std::wstring& Name() const;

		void Acquire();
		void Unacquire();

	private:
		void UpdateKeys();

		boost::shared_ptr<IDirectInputDevice8W> device_;
	};

	class DInputJoystick : public InputJoystick
	{
	public:
		DInputJoystick(REFGUID guid, InputEngine& inputEng);
		~DInputJoystick();

		const std::wstring& Name() const;

		void Acquire();
		void Unacquire();

	private:
		void UpdateKeys();

		boost::shared_ptr<IDirectInputDevice8W> device_;
	};

	boost::shared_ptr<IDirectInputDevice8W> CreateDevice(REFGUID guid, InputEngine& inputEng);
}

#endif		// _DINPUT_HPP
