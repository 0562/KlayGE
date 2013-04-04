// DIMouse.cpp
// KlayGE DInput�������� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
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
#include <KFL/ThrowErr.hpp>
#include <KFL/Util.hpp>
#include <KlayGE/InputFactory.hpp>
#include <KlayGE/Context.hpp>

#include <algorithm>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDevice.hpp>

static DIOBJECTDATAFORMAT dfDIMouse2[] =
{
	{ &GUID_XAxis,  DIMOFS_X, DIDFT_ANYINSTANCE | DIDFT_AXIS, 0 },
	{ &GUID_YAxis,  DIMOFS_Y, DIDFT_ANYINSTANCE | DIDFT_AXIS, 0 },
	{ &GUID_ZAxis,  DIMOFS_Z, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_AXIS, 0 },
	{ &GUID_Button, DIMOFS_BUTTON0, DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON1, DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON2, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON3, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON4, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON5, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON6, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
	{ &GUID_Button, DIMOFS_BUTTON7, DIDFT_OPTIONAL | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 }
};

const DIDATAFORMAT c_dfDIMouse2 =
{
	sizeof(DIDATAFORMAT),
	sizeof(DIOBJECTDATAFORMAT),
	DIDF_RELAXIS,
	sizeof(DIMOUSESTATE2),
	sizeof(dfDIMouse2) / sizeof(dfDIMouse2[0]),
	dfDIMouse2
};

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputMouse::DInputMouse(REFGUID guid, InputEngine const & inputEng)
					: DInputDevice(guid, inputEng)
	{
		this->DataFormat(c_dfDIMouse2);
		this->CooperativeLevel(checked_cast<DInputEngine const *>(&inputEng)->HWnd(), DISCL_NONEXCLUSIVE);

		// ��������Ϊ���ģʽ
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize		= sizeof(dipdw);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwObj		= 0;
		dipdw.diph.dwHow		= DIPH_DEVICE;
		dipdw.dwData			= DIPROPAXISMODE_REL;
		this->Property(DIPROP_AXISMODE, dipdw.diph);
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
		DIMOUSESTATE2 diMouseState;
		this->DeviceState(&diMouseState, sizeof(diMouseState));

		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(checked_cast<DInputEngine const *>(&Context::Instance().InputFactoryInstance().InputEngineInstance())->HWnd(), &pt);
		abs_pos_ = int2(pt.x, pt.y);
		offset_ = int3(diMouseState.lX, diMouseState.lY, diMouseState.lZ);

		index_ = !index_;
		for (size_t i = 0; i < buttons_[index_].size(); ++ i)
		{
			buttons_[index_][i] = (diMouseState.rgbButtons[i] & 0x80) ? true : false;
		}
	}
}
