// DIKeyboard.cpp
// KlayGE DInput���̹����� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
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

#include <boost/array.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDeviceImpl.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputKeyboard::DInputKeyboard(REFGUID guid, InputEngine& inputEng)
	{
		boost::shared_ptr<DInputDeviceImpl> didImpl(new DInputDeviceImpl(guid, inputEng));
		impl_ = didImpl;

		didImpl->DataFormat(c_dfDIKeyboard);
		didImpl->CooperativeLevel(::GetActiveWindow(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		this->Acquire();
	}

	// �豸����
	//////////////////////////////////////////////////////////////////////////////////
	const std::wstring& DInputKeyboard::Name() const
	{
		static const std::wstring name(L"DirectInput Keyboard");
		return name;
	}

	// ���¼���״̬
	//////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::UpdateInputs()
	{
		boost::array<U8, 256> keys;
		static_cast<DInputDeviceImpl*>(impl_.get())->DeviceState(&keys[0], keys.size());

		for (size_t i = 0; i < keys.size(); ++ i)
		{
			keys_[i] = ((keys[i] & 0x80) != 0);
		}
	}
}
