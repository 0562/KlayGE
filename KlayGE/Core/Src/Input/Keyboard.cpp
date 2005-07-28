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

#include <boost/assert.hpp>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// ��ȡĳ���Ƿ���
	//////////////////////////////////////////////////////////////////////////////////
	bool InputKeyboard::Key(size_t n) const
	{
		BOOST_ASSERT(n < keys_.size());

		return keys_[n];
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputKeyboard::DoActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		InputActionMap& iam = actionMaps_[id];

		for (uint16_t i = 0; i < static_cast<uint16_t>(keys_.size()); ++ i)
		{
			if (actionMap.HasAction(i))
			{
				iam.AddAction(InputAction(actionMap.Action(i), i));
			}
		}
	}

	// ���¼��̶���
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputKeyboard::DoUpdate(uint32_t id)
	{
		InputActionsType ret;

		InputActionMap& iam = actionMaps_[id];

		for (uint16_t i = 0; i < static_cast<uint16_t>(keys_.size()); ++ i)
		{
			if (this->Key(i))
			{
				iam.UpdateInputActions(ret, i);
			}
		}

		return ret;
	}
}
