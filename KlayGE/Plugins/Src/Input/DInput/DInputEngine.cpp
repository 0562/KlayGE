// DInputEngine.cpp
// KlayGE DirectInput���������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Engine.hpp>

#include <cassert>

#include <KlayGE/DInput/DInput.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "ole32.lib")

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputEngine::DInputEngine()
	{
		// ���� DirectInput ����
		IDirectInput8W* di;
		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
			IID_PPV_ARG(IDirectInput8W, di), NULL);
		dinput_ = COMPtr<IDirectInput8W>(di);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DInputEngine::~DInputEngine()
	{
		devices_.clear();
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	const WString& DInputEngine::Name() const
	{
		static WString name(L"DirectInput Input Engine");
		return name;
	}

	// ö���豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputEngine::EnumDevices()
	{
		TIF(dinput_->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCB, this, DIEDFL_ALLDEVICES));
	}

	// ö���豸�Ļص�����
	//////////////////////////////////////////////////////////////////////////////////
	BOOL CALLBACK DInputEngine::EnumDevicesCB(LPCDIDEVICEINSTANCEW didi, void* pvRef)
	{
		DInputEngine* inputEng(reinterpret_cast<DInputEngine*>(pvRef));

		InputDevicePtr device;

		switch (GET_DIDEVICE_TYPE(didi->dwDevType))
		{
		case DI8DEVTYPE_KEYBOARD:
			device = InputDevicePtr(new DInputKeyboard(didi->guidInstance, *inputEng));
			break;

		case DI8DEVTYPE_MOUSE:
			device = InputDevicePtr(new DInputMouse(didi->guidInstance, *inputEng));
			break;

		case DI8DEVTYPE_JOYSTICK:
			device = InputDevicePtr(new DInputJoystick(didi->guidInstance, *inputEng));
			break;
		}

		device->Unacquire();
		inputEng->devices_.push_back(device);

		return DIENUM_CONTINUE;
	}
}
