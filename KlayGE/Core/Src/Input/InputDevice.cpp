// InputDevice.cpp
// Clay! �����豸���� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	InputDevice::~InputDevice()
	{
	}

	// ���ö���ӳ��
	/////////////////////////////////////////////////////////////////////////////////
	void InputDevice::ActionMap(const InputActionMap& actionMap)
	{
		actionMap_ = actionMap;

		this->DoActionMap(actionMap_);
	}

	// ͨ������ֵ���ɶ���
	/////////////////////////////////////////////////////////////////////////////////
	U32 InputDevice::MakeAction(U16 key, long value)
	{
		return ((value & 0xFFFF) << 16) | actionMap_.Action(key);
	}
}
