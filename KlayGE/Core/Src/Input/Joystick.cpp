// Joystick.cpp
// KlayGE ��Ϸ�˹����� ʵ���ļ�
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
	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	InputJoystick::~InputJoystick()
	{
	}

	// X��λ��
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::XPos() const
	{
		return pos_.x();
	}

	// Y��λ��
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::YPos() const
	{
		return pos_.y();
	}
	
	// Z��λ��
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::ZPos() const
	{
		return pos_.z();
	}

	// X����ת
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::XRot() const
	{
		return rot_.x();
	}

	// Y����ת
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::YRot() const
	{
		return rot_.y();
	}
	
	// Z����ת
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::ZRot() const
	{
		return rot_.z();
	}

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
	void InputJoystick::DoActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		if (actionMap.HasAction(JS_XPos))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(JS_XPos), JS_XPos));
		}
		if (actionMap.HasAction(JS_XPos))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(JS_YPos), JS_YPos));
		}
		if (actionMap.HasAction(JS_ZPos))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(JS_ZPos), JS_ZPos));
		}
		if (actionMap.HasAction(JS_XRot))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(JS_XRot), JS_XRot));
		}
		if (actionMap.HasAction(JS_XRot))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(JS_YRot), JS_YRot));
		}
		if (actionMap.HasAction(JS_ZRot))
		{
			actionMaps_[id].AddAction(InputAction(actionMap.Action(JS_ZRot), JS_ZRot));
		}

		for (uint16_t i = 0; i < static_cast<uint16_t>(slider_.size()); ++ i)
		{
			uint16_t const slider(static_cast<uint16_t>(JS_Slider0 + i));
			if (actionMap.HasAction(slider))
			{
				actionMaps_[id].AddAction(InputAction(actionMap.Action(slider), slider));
			}
		}
		for (uint16_t i = 0; i < static_cast<uint16_t>(buttons_.size()); ++ i)
		{
			uint16_t const button(static_cast<uint16_t>(JS_Button0 + i));
			if (actionMap.HasAction(button))
			{
				actionMaps_[id].AddAction(InputAction(actionMap.Action(button), button));
			}
		}
	}

	// ������Ϸ�˶���
	/////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputJoystick::DoUpdate(uint32_t id)
	{
		InputActionsType ret;

		actionMaps_[id].UpdateInputActions(ret, JS_XPos, this->XPos());
		actionMaps_[id].UpdateInputActions(ret, JS_YPos, this->YPos());
		actionMaps_[id].UpdateInputActions(ret, JS_ZPos, this->ZPos());
		actionMaps_[id].UpdateInputActions(ret, JS_XRot, this->XRot());
		actionMaps_[id].UpdateInputActions(ret, JS_YRot, this->YRot());
		actionMaps_[id].UpdateInputActions(ret, JS_ZRot, this->ZRot());

		for (uint16_t i = 0; i < static_cast<uint16_t>(slider_.size()); ++ i)
		{
			actionMaps_[id].UpdateInputActions(ret, static_cast<uint16_t>(JS_Slider0 + i), this->Slider(i));
		}
		for (uint16_t i = 0; i < static_cast<uint16_t>(buttons_.size()); ++ i)
		{
			if (this->Button(i))
			{
				actionMaps_[id].UpdateInputActions(ret, static_cast<uint16_t>(JS_Button0 + i));
			}
		}

		return ret;
	}
}
