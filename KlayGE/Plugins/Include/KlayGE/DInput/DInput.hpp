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

		std::wstring const & Name() const;
		void EnumDevices();

		boost::shared_ptr<IDirectInput8W> const & DInput() const;

	private:
		boost::shared_ptr<IDirectInput8W> dinput_;

	private:
		static BOOL CALLBACK EnumDevicesCB(LPCDIDEVICEINSTANCEW didi, void* pvRef);
	};

	class DInputKeyboard : public InputKeyboard
	{
	public:
		DInputKeyboard(REFGUID guid, InputEngine& inputEng);

		std::wstring const & Name() const;

	private:
		void UpdateInputs();
	};

	class DInputMouse : public InputMouse
	{
	public:
		DInputMouse(REFGUID guid, InputEngine& inputEng);

		std::wstring const & Name() const;

	private:
		void UpdateInputs();
	};

	class DInputJoystick : public InputJoystick
	{
	public:
		DInputJoystick(REFGUID guid, InputEngine& inputEng);

		std::wstring const & Name() const;

	private:
		void UpdateInputs();
	};
}

#endif		// _DINPUT_HPP
