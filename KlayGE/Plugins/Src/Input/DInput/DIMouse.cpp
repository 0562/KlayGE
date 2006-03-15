// DIMouse.cpp
// KlayGE DInput�������� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ��Ϊ�Ƕ�ռģʽ (2005.7.26)
// ���ö�̳нṹ (2005.8.11)
//
// 2.1.2
// ����Bridgeģʽʵ�� (2004.9.5)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <algorithm>
#pragma warning(disable: 4512)
#include <boost/lambda/lambda.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDevice.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputMouse::DInputMouse(REFGUID guid, InputEngine const & inputEng)
					: DInputDevice(guid, inputEng)
	{
		this->DataFormat(c_dfDIMouse);
		this->CooperativeLevel(::GetActiveWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		// ��������Ϊ���ģʽ
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize		= sizeof(dipdw);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwObj		= 0;
		dipdw.diph.dwHow		= DIPH_DEVICE;
		dipdw.dwData			= DIPROPAXISMODE_REL;
		this->Property(DIPROP_AXISMODE, dipdw.diph);

		dipdw.dwData			= 16;
		this->Property(DIPROP_BUFFERSIZE, dipdw.diph);
	}

	// �豸����
	//////////////////////////////////////////////////////////////////////////////////
	std::wstring const & DInputMouse::Name() const
	{
		static std::wstring const name(L"DirectInput Mouse");
		return name;
	}

	// ��ȡ�豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputMouse::Acquire()
	{
		DInputDevice::Acquire();
	}

	// �ͷ��豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputMouse::Unacquire()
	{
		DInputDevice::Unacquire();
	}

	// �������״̬
	//////////////////////////////////////////////////////////////////////////////////
	void DInputMouse::UpdateInputs()
	{
		for (;;)
		{
			DIDEVICEOBJECTDATA od;
			uint32_t num_elements = 1;

			HRESULT hr = device_->GetDeviceData(sizeof(od), &od, &num_elements, 0);
			if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
			{
				this->Acquire();
				return;
			}

			// Unable to read data or no data available
			if (FAILED(hr))
			{
				break;
			}

			if (0 == num_elements)
			{
				break;
			}

			if ((od.dwOfs >= DIMOFS_BUTTON0) && (od.dwOfs <= DIMOFS_BUTTON7))
			{
				buttons_[od.dwOfs - DIMOFS_BUTTON0] = true;
			}
		}

		DIMOUSESTATE diMouseState;
		this->DeviceState(&diMouseState, sizeof(diMouseState));

		pos_ = Vector_T<long, 3>(diMouseState.lX, diMouseState.lY, diMouseState.lZ);

		//std::transform(diMouseState.rgbButtons, diMouseState.rgbButtons + buttons_.size(),
		//	buttons_.begin(), boost::lambda::_1 != 0);
	}
}
