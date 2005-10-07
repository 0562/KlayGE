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
		BOOST_ASSERT(index < buttons_.size());

		return buttons_[index];
	}

	// ʵ�ֶ���ӳ��
	//////////////////////////////////////////////////////////////////////////////////
	void InputMouse::ActionMap(uint32_t id, InputActionMap const & actionMap)
	{
		InputActionMap& iam = actionMaps_[id];

		for (uint16_t i = MS_X; i < MS_Button3; ++ i)
		{
			if (actionMap.HasAction(i))
			{
				iam.AddAction(InputActionDefine(actionMap.Action(i), i));
			}
		}
	}

	// ������궯��
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputMouse::DoUpdate(uint32_t id)
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
		for (uint16_t i = 0; i < static_cast<uint16_t>(buttons_.size()); ++ i)
		{
			if (this->Button(i))
			{
				iam.UpdateInputActions(ret, static_cast<uint16_t>(MS_Button0 + i));
			}
		}

		return ret;
	}
}
