// DIJoystick.cpp
// KlayGE DInput��Ϸ�˹����� ʵ���ļ�
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

#include <algorithm>
#include <boost/lambda/lambda.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDeviceImpl.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputJoystick::DInputJoystick(REFGUID guid, InputEngine& inputEng)
	{
		boost::shared_ptr<DInputDeviceImpl> didImpl(new DInputDeviceImpl(guid, inputEng));
		impl_ = didImpl;

		didImpl->DataFormat(c_dfDIJoystick);
		didImpl->CooperativeLevel(::GetForegroundWindow(), DISCL_EXCLUSIVE | DISCL_BACKGROUND);

		// Set the X-axis range (-1000 to +1000)
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		didImpl->Property(DIPROP_RANGE, diprg.diph);

		// And again for Y-axis range
		diprg.diph.dwObj = DIJOFS_Y;
		didImpl->Property(DIPROP_RANGE, diprg.diph);


		// Set X axis dead zone to 10%
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = DIJOFS_X;
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 1000;
		didImpl->Property(DIPROP_DEADZONE, dipdw.diph);

		// Set Y axis dead zone to 10%
		dipdw.diph.dwObj = DIJOFS_Y;
		didImpl->Property(DIPROP_DEADZONE, dipdw.diph);

		this->Acquire();
	}

	// �豸����
	//////////////////////////////////////////////////////////////////////////////////
	const std::wstring& DInputJoystick::Name() const
	{
		static const std::wstring name(L"DirectInput Joystick");
		return name;
	}

	// ������Ϸ��״̬
	/////////////////////////////////////////////////////////////////////////////////
	void DInputJoystick::UpdateInputs()
	{
		DInputDeviceImpl* didImpl = static_cast<DInputDeviceImpl*>(impl_.get());

		didImpl->Poll();

		DIJOYSTATE diJoyState;
		didImpl->DeviceState(&diJoyState, sizeof(diJoyState));

		pos_ = Vector_T<long, 3>(diJoyState.lX, diJoyState.lY, diJoyState.lZ);
		rot_ = Vector_T<long, 3>(diJoyState.lRx, diJoyState.lRy, diJoyState.lRz);

		std::copy(diJoyState.rglSlider, diJoyState.rglSlider + slider_.size(), slider_.begin());
		std::transform(diJoyState.rgbButtons, diJoyState.rgbButtons + buttons_.size(),
			buttons_.begin(), boost::lambda::_1 != 0);
	}
}
