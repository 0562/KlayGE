// Mouse.cpp
// KlayGE �������� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ������Action map id (2005.4.3)
//
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
	void InputMouse::DoActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		if (actionMap.HasAction(MS_X))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(MS_X), MS_X));
		}
		if (actionMap.HasAction(MS_Y))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(MS_Y), MS_Y));
		}
		if (actionMap.HasAction(MS_Z))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(MS_Z), MS_Z));
		}
		for (uint16_t i = 0; i < static_cast<uint16_t>(buttons_.size()); ++ i)
		{
			uint16_t const button(static_cast<uint16_t>(MS_Button0 + i));
			if (actionMaps_[id].HasAction(button))
			{
				actionMaps_[id].AddAction(InputAction(actionMap.Action(button), button));
			}
		}
	}

	// ������궯��
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputMouse::DoUpdate(uint32_t id)
	{
		InputActionsType ret;

		if (this->X() != 0)
		{
			actionMaps_[id].UpdateInputActions(ret, MS_X, this->X());
		}
		if (this->Y() != 0)
		{
			actionMaps_[id].UpdateInputActions(ret, MS_Y, this->Y());
		}
		if (this->Z() != 0)
		{
			actionMaps_[id].UpdateInputActions(ret, MS_Z, this->Z());
		}
		for (uint16_t i = 0; i < static_cast<uint16_t>(buttons_.size()); ++ i)
		{
			if (this->Button(i))
			{
				actionMaps_[id].UpdateInputActions(ret, static_cast<uint16_t>(MS_Button0 + i));
			}
		}

		return ret;
	}
}
