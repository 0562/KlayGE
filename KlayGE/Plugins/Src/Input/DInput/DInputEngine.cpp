// DInputEngine.cpp
// KlayGE DirectInput���������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.klayge.org
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#define INITGUID
#include <KFL/Util.hpp>
#include <KFL/ThrowErr.hpp>
#include <KFL/COMPtr.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/App3D.hpp>
#include <KlayGE/Window.hpp>

#include <KlayGE/DInput/DInput.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputEngine::DInputEngine()
	{
		mod_dinput8_ = ::LoadLibraryW(L"dinput8.dll");
		if (nullptr == mod_dinput8_)
		{
			::MessageBoxW(nullptr, L"Can't load dinput8.dll", L"Error", MB_OK);
		}

		if (mod_dinput8_ != nullptr)
		{
			DynamicDirectInput8Create_ = reinterpret_cast<DirectInput8CreateFunc>(::GetProcAddress(mod_dinput8_, "DirectInput8Create"));
		}

		// Create DirectInput object
		IDirectInput8W* di;
		DynamicDirectInput8Create_(::GetModuleHandle(nullptr), DIRECTINPUT_VERSION, 
			IID_IDirectInput8W, reinterpret_cast<void**>(&di), nullptr);
		dinput_ = MakeCOMPtr(di);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DInputEngine::~DInputEngine()
	{
		devices_.clear();

		// Some other resources may still alive, so don't free them
		//::FreeLibrary(mod_dinput8_);
	}

	// ��ȡDirectInput�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	shared_ptr<IDirectInput8W> const & DInputEngine::DInput() const
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
			device = MakeSharedPtr<DInputKeyboard>(didi->guidInstance, inputEng);
			break;

		case DI8DEVTYPE_MOUSE:
			device = MakeSharedPtr<DInputMouse>(didi->guidInstance, inputEng);
			break;

		case DI8DEVTYPE_JOYSTICK:
			device = MakeSharedPtr<DInputJoystick>(didi->guidInstance, inputEng);
			break;
		}

		if (device)
		{
			inputEng.devices_.push_back(device);
		}

		return DIENUM_CONTINUE;
	}
}
