// DIMouse.cpp
// KlayGE DInput�������� ʵ���ļ�
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

#include <KlayGE/DInput/DInput.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputMouse::DInputMouse(REFGUID guid, InputEngine& inputEng)
	{
		device_ = CreateDevice(guid, inputEng);

		device_->SetDataFormat(&c_dfDIMouse);
		device_->SetCooperativeLevel(::GetActiveWindow(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		// ��������Ϊ���ģʽ
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize		= sizeof(dipdw);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwObj		= 0;
		dipdw.diph.dwHow		= DIPH_DEVICE;
		dipdw.dwData			= DIPROPAXISMODE_REL;
		device_->SetProperty(DIPROP_AXISMODE, &(dipdw.diph));

		this->Acquire();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DInputMouse::~DInputMouse()
	{
		this->Unacquire();
	}

	// �豸����
	//////////////////////////////////////////////////////////////////////////////////
	const std::wstring& DInputMouse::Name() const
	{
		static std::wstring name(L"DirectInput Mouse");
		return name;
	}

	// ��ȡ�豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputMouse::Acquire()
	{
		TIF(device_->Acquire());
	}

	// �ͷ��豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputMouse::Unacquire()
	{
		TIF(device_->Unacquire());
	}

	// �������״̬
	//////////////////////////////////////////////////////////////////////////////////
	void DInputMouse::UpdateKeys()
	{
		DIMOUSESTATE diMouseState;
		bool done;
		do
		{
			HRESULT hr(device_->GetDeviceState(sizeof(diMouseState), &diMouseState));
			if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
			{
				this->Acquire();
				done = false;
			}
			else
			{
				done = true;
			}
		} while (!done);

		pos_ = Vector_T<long, 3>(diMouseState.lX, diMouseState.lY, diMouseState.lZ);

		for (size_t i = 0; i < buttons_.size(); ++ i)
		{
			buttons_[i] = (diMouseState.rgbButtons[i] != 0);
		}
	}
}
