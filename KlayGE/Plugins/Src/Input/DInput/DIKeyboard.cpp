// DIKeyboard.cpp
// KlayGE DInput���̹����� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
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
#include <KlayGE/Util.hpp>

#include <boost/assert.hpp>
#include <boost/array.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4512)
#endif
#include <boost/lambda/lambda.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDevice.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputKeyboard::DInputKeyboard(REFGUID guid, InputEngine const & inputEng)
						: DInputDevice(guid, inputEng)
	{
		this->DataFormat(c_dfDIKeyboard);
		this->CooperativeLevel(checked_cast<DInputEngine const *>(&inputEng)->HWnd(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	// �豸����
	//////////////////////////////////////////////////////////////////////////////////
	std::wstring const & DInputKeyboard::Name() const
	{
		static std::wstring const name(L"DirectInput Keyboard");
		return name;
	}

	// ��ȡ�豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::Acquire()
	{
		DInputDevice::Acquire();
	}

	// �ͷ��豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::Unacquire()
	{
		DInputDevice::Unacquire();
	}

	// ���¼���״̬
	//////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::UpdateInputs()
	{
		boost::array<uint8_t, 256> keys;
		this->DeviceState(&keys[0], keys.size());

		std::transform(keys.begin(), keys.end(), keys_.begin(),
			(boost::lambda::_1 & 0x80) != 0);
	}
}
