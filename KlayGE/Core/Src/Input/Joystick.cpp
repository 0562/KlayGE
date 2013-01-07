// Joystick.cpp
// KlayGE ��Ϸ�˹����� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
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
	InputJoystick::InputJoystick()
		: index_(false)
	{
		buttons_[0].fill(false);
		buttons_[1].fill(false);
	}

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

	// ��ȡ���˵�����
	//////////////////////////////////////////////////////////////////////////////////
	size_t InputJoystick::NumSliders() const
	{
		return slider_.size();
	}
	
	// ��ȡ���˵�ֵ
	/////////////////////////////////////////////////////////////////////////////////
	long InputJoystick::Slider(size_t index) const
	{
		BOOST_ASSERT(index < slider_.size());

		return slider_[index];
	}

	// ��ȡ��������
	//////////////////////////////////////////////////////////////////////////////////
	size_t InputJoystick::NumButtons() const
	{
		return buttons_[index_].size();
	}
	
	// ��ȡָ�����Ƿ���
	/////////////////////////////////////////////////////////////////////////////////
	bool InputJoystick::Button(size_t n) const
	{
		BOOST_ASSERT(n < buttons_[index_].size());

		return buttons_[index_][n];
	}

	bool InputJoystick::ButtonDown(size_t n) const
	{
		BOOST_ASSERT(n < buttons_[index_].size());

		return (buttons_[index_][n] && !buttons_[!index_][n]);
	}

	bool InputJoystick::ButtonUp(size_t n) const
	{
		BOOST_ASSERT(n < buttons_[index_].size());

		return (!buttons_[index_][n] && buttons_[!index_][n]);
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputJoystick::ActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		InputActionMap& iam = actionMaps_[id];

		for (uint16_t i = JS_XPos; i < JS_Button0 + this->NumButtons(); ++ i)
		{
			if (actionMap.HasAction(i))
			{
				iam.AddAction(InputActionDefine(actionMap.Action(i), i));
			}
		}
	}

	// ������Ϸ�˶���
	/////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputJoystick::UpdateActionMap(uint32_t id)
	{
		InputActionsType ret;

		actionMaps_[id].UpdateInputActions(ret, JS_XPos, this->XPos());
		actionMaps_[id].UpdateInputActions(ret, JS_YPos, this->YPos());
		actionMaps_[id].UpdateInputActions(ret, JS_ZPos, this->ZPos());
		actionMaps_[id].UpdateInputActions(ret, JS_XRot, this->XRot());
		actionMaps_[id].UpdateInputActions(ret, JS_YRot, this->YRot());
		actionMaps_[id].UpdateInputActions(ret, JS_ZRot, this->ZRot());

		for (uint16_t i = 0; i < this->NumSliders(); ++ i)
		{
			actionMaps_[id].UpdateInputActions(ret, static_cast<uint16_t>(JS_Slider0 + i), this->Slider(i));
		}
		for (uint16_t i = 0; i < this->NumButtons(); ++ i)
		{
			if (this->Button(i))
			{
				actionMaps_[id].UpdateInputActions(ret, static_cast<uint16_t>(JS_Button0 + i));
			}
		}

		return ret;
	}
}
