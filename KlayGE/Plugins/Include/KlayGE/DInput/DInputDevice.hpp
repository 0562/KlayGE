// DInputDevice.hpp
// KlayGE DirectInput���������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
//
// 2.8.0
// ��Ϊ���� (2005.8.11)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DINPUTDEVICE_HPP
#define _DINPUTDEVICE_HPP

#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	class DInputDevice
	{
	public:
		DInputDevice(REFGUID guid, InputEngine const & inputEng);
		virtual ~DInputDevice();

		void Acquire();
		void Unacquire();

		void DataFormat(DIDATAFORMAT const & df);
		void CooperativeLevel(HWND hwnd, DWORD flags);
		void Property(REFGUID rguidProp, DIPROPHEADER const & diph);

		void Poll();
		void DeviceState(void* data, size_t size);
		void DeviceData(size_t size, DIDEVICEOBJECTDATA* rgdod, uint32_t& num_elements);

	protected:
		shared_ptr<IDirectInputDevice8W> device_;
	};
}

#endif		// _DINPUTDEVICE_HPP
