// Mouse.cpp
// KlayGE �������� ʵ���ļ�
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
	// X��
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::X() const
	{
		return pos_.x();
	}

	// Y��
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::Y() const
	{
		return pos_.y();
	}

	// Z�ᣬҲ���ǹ���
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::Z() const
	{
		return pos_.z();
	}

	// ���
	//////////////////////////////////////////////////////////////////////////////////
	bool InputMouse::LeftButton() const
	{
		return this->Button(0);
	}

	// �Ҽ�
	//////////////////////////////////////////////////////////////////////////////////
	bool InputMouse::RightButton() const
	{
		return this->Button(1);
	}

	// �м�
	//////////////////////////////////////////////////////////////////////////////////
	bool InputMouse::MiddleButton() const
	{
		return this->Button(2);
	}

	// ��ȡĳ��ť�Ƿ���
	//////////////////////////////////////////////////////////////////////////////////
	bool InputMouse::Button(size_t index) const
	{
		assert(index < buttons_.size());

		return buttons_[index];
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputMouse::DoActionMap(const InputActionMap& actionMap)
	{
		if (actionMap.HasAction(MS_X))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(MS_X), MS_X));
		}
		if (actionMap_.HasAction(MS_Y))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(MS_Y), MS_Y));
		}
		if (actionMap_.HasAction(MS_Z))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(MS_Z), MS_Z));
		}
		for (U16 i = 0; i < static_cast<U16>(buttons_.size()); ++ i)
		{
			if (actionMap_.HasAction(MS_Button0 + i))
			{
				actionMap_.AddAction(InputAction(actionMap.Action(MS_Button0 + i), MS_Button0 + i));
			}
		}
	}

	// ������궯��
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputMouse::Update()
	{
		this->UpdateKeys();

		InputActionsType ret;

		if (this->X() != 0)
		{
			actionMap_.UpdateInputActions(ret, MS_X, this->X());
		}
		if (this->Y() != 0)
		{
			actionMap_.UpdateInputActions(ret, MS_Y, this->Y());
		}
		if (this->Z() != 0)
		{
			actionMap_.UpdateInputActions(ret, MS_Z, this->Z());
		}
		for (U16 i = 0; i < static_cast<U16>(buttons_.size()); ++ i)
		{
			if (this->Button(i))
			{
				actionMap_.UpdateInputActions(ret, MS_Button0 + i);
			}
		}

		return ret;
	}
}
