// DInputDeviceImpl.cpp
// KlayGE DInput�豸ʵ���� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ���ν��� (2004.9.5)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputDeviceImpl.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DInputDeviceImpl::DInputDeviceImpl(REFGUID guid, InputEngine& inputEng)
	{
		DInputEngine& dinputEng(static_cast<DInputEngine&>(inputEng));

		IDirectInputDevice8W* device;
		dinputEng.DInput()->CreateDevice(guid, &device, NULL);
		device_ = MakeCOMPtr(device);
	}

	// ��ȡ�豸
	/////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::Acquire()
	{
		TIF(device_->Acquire());
	}

	// �ͷ��豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::Unacquire()
	{
		TIF(device_->Unacquire());
	}

	// �������ݸ�ʽ
	//////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::DataFormat(const DIDATAFORMAT& df)
	{
		device_->SetDataFormat(&df);
	}

	// ����Э������
	//////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::CooperativeLevel(HWND hwnd, DWORD flags)
	{
		device_->SetCooperativeLevel(hwnd, flags);
	}

	// �����豸����
	//////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::Property(REFGUID rguidProp, const DIPROPHEADER& diph)
	{
		TIF(device_->SetProperty(rguidProp, &diph));
	}

	// ��ѭ�豸
	//////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::Poll()
	{
		TIF(device_->Poll());
	}

	// ��ȡ�豸״̬
	//////////////////////////////////////////////////////////////////////////////////
	void DInputDeviceImpl::DeviceState(void* data, size_t size)
	{
		bool done;
		do
		{
			HRESULT hr = device_->GetDeviceState(size, data);
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
	}
}
