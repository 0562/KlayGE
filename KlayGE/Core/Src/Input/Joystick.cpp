// Joystick.cpp
// KlayGE ��Ϸ�˹����� ʵ���ļ�
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
	// ��ȡ���˵�ֵ
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::Slider(size_t index) const
	{
		assert(index < slider_.size());

		return slider_[index];
	}

	// ��ȡָ�����Ƿ���
	/////////////////////////////////////////////////////////////////////////////////
	bool InputJoystick::Button(size_t index) const
	{
		assert(index < buttons_.size());

		return buttons_[index];
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputJoystick::DoActionMap(const InputActionMap& actionMap)
	{
		if (actionMap.HasAction(JS_XPos))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(JS_XPos), JS_XPos));
		}
		if (actionMap_.HasAction(JS_XPos))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(JS_YPos), JS_YPos));
		}
		if (actionMap_.HasAction(JS_ZPos))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(JS_ZPos), JS_ZPos));
		}
		if (actionMap.HasAction(JS_XRot))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(JS_XRot), JS_XRot));
		}
		if (actionMap_.HasAction(JS_XRot))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(JS_YRot), JS_YRot));
		}
		if (actionMap_.HasAction(JS_ZRot))
		{
			actionMap_.AddAction(InputAction(actionMap.Action(JS_ZRot), JS_ZRot));
		}

		for (size_t i = 0; i < slider_.size(); ++ i)
		{
			if (actionMap_.HasAction(JS_Slider0 + i))
			{
				actionMap_.AddAction(InputAction(actionMap.Action(JS_Slider0 + i), JS_Slider0 + i));
			}
		}
		for (size_t i = 0; i < buttons_.size(); ++ i)
		{
			if (actionMap_.HasAction(JS_Button0 + i))
			{
				actionMap_.AddAction(InputAction(actionMap.Action(JS_Button0 + i), JS_Button0 + i));
			}
		}
	}

	// ������Ϸ�˶���
	/////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputJoystick::Update()
	{
		this->UpdateKeys();

		InputActionsType ret;

		if (actionMap_.HasAction(JS_XPos))
		{
			ret.push_back(this->MakeAction(JS_XPos, this->XPos()));
		}
		if (actionMap_.HasAction(JS_YPos))
		{
			ret.push_back(this->MakeAction(JS_YPos, this->YPos()));
		}
		if (actionMap_.HasAction(JS_ZPos))
		{
			ret.push_back(this->MakeAction(JS_ZPos, this->ZPos()));
		}
		if (actionMap_.HasAction(JS_XRot))
		{
			ret.push_back(this->MakeAction(JS_XRot, this->XRot()));
		}
		if (actionMap_.HasAction(JS_YRot))
		{
			ret.push_back(this->MakeAction(JS_YRot, this->YRot()));
		}
		if (actionMap_.HasAction(JS_ZRot))
		{
			ret.push_back(this->MakeAction(JS_ZRot, this->ZRot()));
		}

		for (size_t i = 0; i < slider_.size(); ++ i)
		{
			if (actionMap_.HasAction(JS_Slider0 + i))
			{
				ret.push_back(this->MakeAction(JS_Slider0 + i, this->Slider(i)));
			}
		}
		for (size_t i = 0; i < buttons_.size(); ++ i)
		{
			if (actionMap_.HasAction(JS_Button0 + i))
			{
				if (this->Button(i))
				{
					ret.push_back(this->actionMap_.Action(JS_Button0 + i));
				}
			}
		}

		return ret;
	}
}
