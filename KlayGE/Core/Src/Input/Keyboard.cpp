// Keyboard.cpp
// KlayGE ���̹����� ʵ���ļ�
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

#include <cassert>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// ��ȡĳ���Ƿ���
	//////////////////////////////////////////////////////////////////////////////////
	bool InputKeyboard::Key(size_t n) const
	{
		assert(n < keys_.size());

		return keys_[n];
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputKeyboard::DoActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		for (uint16_t i = 0; i < static_cast<uint16_t>(keys_.size()); ++ i)
		{
			if (actionMap.HasAction(i))
			{
				actionMaps_[id].AddAction(InputAction(actionMap.Action(i), i));
			}
		}
	}

	// ���¼��̶���
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputKeyboard::DoUpdate(uint32_t id)
	{
		InputActionsType ret;

		for (uint16_t i = 0; i < static_cast<uint16_t>(keys_.size()); ++ i)
		{
			if (this->Key(i))
			{
				actionMaps_[id].UpdateInputActions(ret, i);
			}
		}

		return ret;
	}
}
