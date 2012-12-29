// DInputDevice.cpp
// KlayGE DInput�豸ʵ���� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://www.klayge.org
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
#include <KFL/ThrowErr.hpp>
#include <KFL/Util.hpp>
#include <KFL/COMPtr.hpp>

#include <boost/assert.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDevice.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputDevice::DInputDevice(REFGUID guid, InputEngine const & inputEng)
	{
		DInputEngine const & dinputEng = *checked_cast<DInputEngine const *>(&inputEng);

		IDirectInputDevice8W* device;
		dinputEng.DInput()->CreateDevice(guid, &device, nullptr);
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
		if (DIERR_OTHERAPPHASPRIO == device_->Acquire())
		{
			device_->Acquire();
		}
	}

	// �ͷ��豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::Unacquire()
	{
		HRESULT hr = device_->Unacquire();
		if ((hr != DI_OK) && (hr != DI_NOEFFECT))
		{
			device_->Unacquire();
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
		HRESULT hr = device_->Poll();
		if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
		{
			this->Acquire();

			device_->Poll();
		}
	}

	// ��ȡ�豸״̬
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::DeviceState(void* data, size_t size)
	{
		HRESULT hr = device_->GetDeviceState(static_cast<DWORD>(size), data);
		if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
		{
			this->Acquire();

			device_->GetDeviceState(static_cast<DWORD>(size), data);
		}
	}

	// ��ȡ�豸���ݣ�����״̬
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDevice::DeviceData(size_t size, DIDEVICEOBJECTDATA* rgdod, uint32_t& num_elements)
	{
		HRESULT hr = device_->GetDeviceData(static_cast<DWORD>(size), rgdod,
						reinterpret_cast<DWORD*>(&num_elements), 0);
		if ((DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr))
		{
			this->Acquire();

			device_->GetDeviceData(static_cast<DWORD>(size), rgdod,
						reinterpret_cast<DWORD*>(&num_elements), 0);
		}
	}
}
