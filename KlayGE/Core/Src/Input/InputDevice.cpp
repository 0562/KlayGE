// InputDevice.cpp
// KlayGE �����豸���� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
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
	void InputDevice::ActionMap(InputActionMap const & actionMap)
	{
		actionMap_ = actionMap;

		this->DoActionMap(actionMap_);
	}
}
