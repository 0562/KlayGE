// Keyboard.cpp
// KlayGE ���̹����� ʵ���ļ�
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

#include <cassert>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// ��ȡĳ���Ƿ���
	//////////////////////////////////////////////////////////////////////////////////
	bool InputKeyboard::Key(size_t n) const
	{
		assert(n < keys_.size());

		return (keys_[n]);
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputKeyboard::DoActionMap(const InputActionMap& actionMap)
	{
		for (U16 i = 0; i < static_cast<U16>(keys_.size()); ++ i)
		{
			if (actionMap.HasAction(i))
			{
				actionMap_.AddAction(InputAction(actionMap.Action(i), i));
			}
		}
	}

	// ���¼��̶���
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputKeyboard::Update()
	{
		this->UpdateKeys();

		InputActionsType ret;

		for (U16 i = 0; i < static_cast<U16>(keys_.size()); ++ i)
		{
			if (this->Key(i))
			{
				actionMap_.UpdateInputActions(ret, i);
			}
		}

		return ret;
	}
}
