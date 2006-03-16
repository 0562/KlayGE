// DInputDevice.cpp
// KlayGE DInput�豸ʵ���� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ����ΪDInputDevice (2005.8.11)
//
// 2.1.2
// ���ν��� (2004.9.5)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>

#include <boost/assert.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDevice.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputDevice::DInputDevice(REFGUID guid, InputEngine const & inputEng)
	{
		DInputEngine const & dinputEng(*checked_cast<DInputEngine const *>(&inputEng));

		IDirectInputDevice8W* device;
		dinputEng.DInput()->CreateDevice(guid, &device, NULL);
		device_ = MakeCOMPtr(device);

		this->Unacquire();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DInputDevice::~DInputDevice()
	{
		this->Unacquire();
	}

	// ��ȡ�豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::Acquire()
	{
		while (DIERR_OTHERAPPHASPRIO == device_->Acquire())
		{
			Sleep(1);
		}
	}

	// �ͷ��豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::Unacquire()
	{
		for (;;)
		{
			HRESULT hr = device_->Unacquire();
			if ((DI_OK == hr) || (DI_NOEFFECT == hr))
			{
				break;
			}
			else
			{
				Sleep(1);
			}
		}
	}

	// �������ݸ�ʽ
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::DataFormat(const DIDATAFORMAT& df)
	{
		device_->SetDataFormat(&df);
	}

	// ����Э������
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::CooperativeLevel(HWND hwnd, DWORD flags)
	{
		device_->SetCooperativeLevel(hwnd, flags);
	}

	// �����豸����
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::Property(REFGUID rguidProp, const DIPROPHEADER& diph)
	{
		TIF(device_->SetProperty(rguidProp, &diph));
	}

	// ��ѭ�豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::Poll()
	{
		for (;;)
		{
			HRESULT hr = device_->Poll();
			if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
			{
				this->Acquire();
			}
			else
			{
				BOOST_ASSERT(DIERR_NOTINITIALIZED != hr);
				break;
			}
		}
	}

	// ��ȡ�豸״̬
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::DeviceState(void* data, size_t size)
	{
		for (;;)
		{
			HRESULT hr = device_->GetDeviceState(static_cast<DWORD>(size), data);
			if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
			{
				this->Acquire();
			}
			else
			{
				BOOST_ASSERT(DIERR_NOTINITIALIZED != hr);
				break;
			}
		}
	}

	// ��ȡ�豸���ݣ�����״̬
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::DeviceData(size_t size, DIDEVICEOBJECTDATA* rgdod, uint32_t& num_elements)
	{
		for (;;)
		{
			HRESULT hr = device_->GetDeviceData(size, rgdod, &num_elements, 0);
			if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
			{
				this->Acquire();
			}
			else
			{
				BOOST_ASSERT(DIERR_NOTINITIALIZED != hr);
				break;
			}
		}
	}
}
