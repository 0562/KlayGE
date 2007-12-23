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

#include <boost/assert.hpp>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	InputMouse::InputMouse()
		: index_(false)
	{
		buttons_[0].assign(false);
		buttons_[1].assign(false);
	}

	InputMouse::~InputMouse()
	{
	}

	// ���������X
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::AbsX() const
	{
		return abs_pos_.x();
	}

	// ���������Y
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::AbsY() const
	{
		return abs_pos_.y();
	}

	// X��
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::X() const
	{
		return offset_.x();
	}

	// Y��
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::Y() const
	{
		return offset_.y();
	}

	// Z�ᣬҲ���ǹ���
	//////////////////////////////////////////////////////////////////////////////////
	long InputMouse::Z() const
	{
		return offset_.z();
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

	// ��ȡ��������
	//////////////////////////////////////////////////////////////////////////////////
	size_t InputMouse::NumButtons() const
	{
		return buttons_[index_].size();
	}

	// ��ȡĳ��ť�Ƿ���
	//////////////////////////////////////////////////////////////////////////////////
	bool InputMouse::Button(size_t n) const
	{
		BOOST_ASSERT(n < buttons_[index_].size());

		return buttons_[index_][n];
	}

	bool InputMouse::ButtonDown(size_t n) const
	{
		BOOST_ASSERT(n < buttons_[index_].size());

		return (buttons_[index_][n] && !buttons_[!index_][n]);
	}

	bool InputMouse::ButtonUp(size_t n) const
	{
		BOOST_ASSERT(n < buttons_[index_].size());

		return (!buttons_[index_][n] && buttons_[!index_][n]);
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputMouse::ActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		InputActionMap& iam = actionMaps_[id];

		for (uint16_t i = MS_X; i < MS_Button0 + this->NumButtons(); ++ i)
		{
			if (actionMap.HasAction(i))
			{
				iam.AddAction(InputActionDefine(actionMap.Action(i), i));
			}
		}
	}

	// ������궯��
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputMouse::UpdateActionMap(uint32_t id)
	{
		InputActionsType ret;

		InputActionMap& iam = actionMaps_[id];

		if (this->X() != 0)
		{
			iam.UpdateInputActions(ret, MS_X, this->X());
		}
		if (this->Y() != 0)
		{
			iam.UpdateInputActions(ret, MS_Y, this->Y());
		}
		if (this->Z() != 0)
		{
			iam.UpdateInputActions(ret, MS_Z, this->Z());
		}
		for (uint16_t i = 0; i < this->NumButtons(); ++ i)
		{
			if (this->Button(i))
			{
				iam.UpdateInputActions(ret, static_cast<uint16_t>(MS_Button0 + i));
			}
		}

		return ret;
	}
}
