// DIKeyboard.cpp
// KlayGE DInput���̹����� ʵ���ļ�
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
	DInputKeyboard::DInputKeyboard(REFGUID guid, InputEngine& inputEng)
	{
		DInputEngine& dinputEng(reinterpret_cast<DInputEngine&>(inputEng));

		IDirectInputDevice8W* device;
		dinputEng.DInput()->CreateDevice(guid, &device, NULL);
		device_ = COMPtr<IDirectInputDevice8W>(device);

		device_->SetDataFormat(&c_dfDIKeyboard);
		device_->SetCooperativeLevel(::GetActiveWindow(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		this->Acquire();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DInputKeyboard::~DInputKeyboard()
	{
		this->Unacquire();
	}

	// �豸����
	//////////////////////////////////////////////////////////////////////////////////
	const WString& DInputKeyboard::Name() const
	{
		static WString name(L"DirectInput Keyboard");
		return name;
	}

	// ��ȡ�豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::Acquire()
	{
		TIF(device_->Acquire());
	}

	// �ͷ��豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::Unacquire()
	{
		TIF(device_->Unacquire());
	}

	// ���¼���״̬
	//////////////////////////////////////////////////////////////////////////////////
	void DInputKeyboard::UpdateKeys()
	{
		U8 keys[256];

		bool done;
		do
		{
			HRESULT hr(device_->GetDeviceState(sizeof(keys), &keys));
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

		for (size_t i = 0; i < sizeof(keys); ++ i)
		{
			keys_[i] = ((keys[i] & 0x80) != 0);
		}
	}
}
