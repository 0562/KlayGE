// DInput.hpp
// KlayGE DirectInput���������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DINPUTDEVICEIMPL_HPP
#define _DINPUTDEVICEIMPL_HPP

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

#include <KlayGE/Input.hpp>
#include <boost/smart_ptr.hpp>

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "KlayGE_InputEngine_DInput_d.lib")
#else
	#pragma comment(lib, "KlayGE_InputEngine_DInput.lib")
#endif

namespace KlayGE
{
	class DInputDeviceImpl : public InputDeviceImpl
	{
	public:
		DInputDeviceImpl(REFGUID guid, InputEngine& inputEng);

		void Acquire();
		void Unacquire();

		void DataFormat(DIDATAFORMAT const & df);
		void CooperativeLevel(HWND hwnd, DWORD flags);
		void Property(REFGUID rguidProp, DIPROPHEADER const & diph);

		void Poll();
		void DeviceState(void* data, size_t size);

	private:
		boost::shared_ptr<IDirectInputDevice8W> device_;
	};
}

#endif		// _DINPUTDEVICEIMPL_HPP
