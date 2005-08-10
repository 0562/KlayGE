// InputDevice.cpp
// KlayGE �����豸���� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ������Action map id (2005.4.3)
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
	void InputDevice::ActionMap(action_maps_t const & ams)
	{
		actionMaps_ = ams;

		for (action_maps_t::iterator iter = actionMaps_.begin();
			iter != actionMaps_.end(); ++ iter)
		{
			this->DoActionMap(iter->first, iter->second);
		}
	}

	// ���¶���
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputDevice::Update(uint32_t id)
	{
		this->UpdateInputs();

		return this->DoUpdate(id);
	}
}
