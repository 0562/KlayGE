// InputActionMap.cpp
// KlayGE ���붯��ӳ���� ʵ���ļ�
// Ver 2.0.5
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.5
// ���ν��� (2004.4.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <cassert>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// ���Ӷ���
	//////////////////////////////////////////////////////////////////////////////////
	void InputActionMap::AddAction(InputAction const & inputAction)
	{
		actionMap_.insert(std::make_pair(inputAction.second, inputAction.first));
	}

	// �������붯��
	//////////////////////////////////////////////////////////////////////////////////
	void InputActionMap::UpdateInputActions(InputActionsType& actions, U16 key, long value)
	{
		if (this->HasAction(key))
		{
			actions.push_back(std::make_pair(this->Action(key), value));
		}
	}

	// ӳ���д������key
	//////////////////////////////////////////////////////////////////////////////////
	bool InputActionMap::HasAction(U16 key) const
	{
		return (actionMap_.find(key) != actionMap_.end());
	}

	// ��key��ȡ����
	//////////////////////////////////////////////////////////////////////////////////
	U16 InputActionMap::Action(U16 key) const
	{
		return actionMap_.find(key)->second;
	}
}