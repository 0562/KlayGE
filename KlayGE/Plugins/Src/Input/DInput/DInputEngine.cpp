// DInputEngine.cpp
// KlayGE DirectInput���������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/COMPtr.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/App3D.hpp>
#include <KlayGE/Window.hpp>

#include <KlayGE/DInput/DInput.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "ole32.lib")
#endif

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputEngine::DInputEngine()
	{
		// ���� DirectInput ����
		IDirectInput8W* di;
		DirectInput8Create(::GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
			IID_IDirectInput8W, reinterpret_cast<void**>(&di), NULL);
		dinput_ = MakeCOMPtr(di);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DInputEngine::~DInputEngine()
	{
		devices_.clear();
	}

	// ��ȡDirectInput�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	boost::shared_ptr<IDirectInput8W> const & DInputEngine::DInput() const
	{
		return dinput_;
	}

	// ��ȡ���ھ��
	/////////////////////////////////////////////////////////////////////////////////
	HWND DInputEngine::HWnd() const
	{
		return hwnd_;
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring const & DInputEngine::Name() const
	{
		static std::wstring const name(L"DirectInput Input Engine");
		return name;
	}

	// ö���豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputEngine::EnumDevices()
	{
		TIF(dinput_->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCB, this, DIEDFL_ALLDEVICES));
		hwnd_ = Context::Instance().AppInstance().MainWnd()->HWnd();
	}

	// ö���豸�Ļص�����
	//////////////////////////////////////////////////////////////////////////////////
	BOOL CALLBACK DInputEngine::EnumDevicesCB(LPCDIDEVICEINSTANCEW didi, void* pvRef)
	{
		DInputEngine& inputEng(*(reinterpret_cast<DInputEngine*>(pvRef)));

		InputDevicePtr device;

		switch (GET_DIDEVICE_TYPE(didi->dwDevType))
		{
		case DI8DEVTYPE_KEYBOARD:
			device = InputDevicePtr(new DInputKeyboard(didi->guidInstance, inputEng));
			break;

		case DI8DEVTYPE_MOUSE:
			device = InputDevicePtr(new DInputMouse(didi->guidInstance, inputEng));
			break;

		case DI8DEVTYPE_JOYSTICK:
			device = InputDevicePtr(new DInputJoystick(didi->guidInstance, inputEng));
			break;
		}

		if (device)
		{
			inputEng.devices_.push_back(device);
		}

		return DIENUM_CONTINUE;
	}
}
