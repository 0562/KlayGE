// Mouse.cpp
// KlayGE �������� ʵ���ļ�
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

#include <cassert>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
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
		for (size_t i = 0; i < buttons_.size(); ++ i)
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

		if (actionMap_.HasAction(MS_X))
		{
			if (this->X() != 0)
			{
				ret.push_back(this->MakeAction(MS_X, this->X()));
			}
		}
		if (actionMap_.HasAction(MS_Y))
		{
			if (this->Y() != 0)
			{
				ret.push_back(this->MakeAction(MS_Y, this->Y()));
			}
		}
		if (actionMap_.HasAction(MS_Z))
		{
			if (this->Z() != 0)
			{
				ret.push_back(this->MakeAction(MS_Z, this->Z()));
			}
		}
		for (size_t i = 0; i < buttons_.size(); ++ i)
		{
			if (actionMap_.HasAction(MS_Button0 + i))
			{
				if (this->Button(i))
				{
					ret.push_back(actionMap_.Action(MS_Button0 + i));
				}
			}
		}

		return ret;
	}
}
