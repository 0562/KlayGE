// InputEngine.cpp
// KlayGE ���������� ʵ���ļ�
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
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/alloc.hpp>

#include <cassert>
#include <set>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// ��������
	//////////////////////////////////////////////////////////////////////////////////
	InputEngine::~InputEngine()
	{
		this->UnacquireDevices();
	}

	// ���ö�����ʽ
	//////////////////////////////////////////////////////////////////////////////////
	void InputEngine::ActionMap(const InputActionMap& actionMap, bool reenumerate)
	{
		// �����µĶ�����ʽ
		actionMap_ = actionMap;

		// ֻ�е�����ʱָ��Ҫ��ö�ٲ����ٲ���ö���豸
		// �豸�б��п�����ѭ����ʹ�ã������ʱö���豸�п����������
		if (reenumerate)
		{
			// �����ǰö�ٵ��豸
			devices_.clear();

			// ����ö�ٺ��ʵ��豸
			this->EnumDevices();
		}
		else // Ӧ���µĶ���ӳ��
		{
			// ���µĶ���ӳ������ǰ�豸����ȫ�����ͷ�
			this->UnacquireDevices();
		}

		// �Ե�ǰ�豸Ӧ���µĶ���ӳ��
		for (InputDevicesType::iterator iter = devices_.begin(); iter != devices_.end(); ++ iter)
		{
			(*iter)->ActionMap(actionMap_);
		}
	}

	// ��ȡ�����豸����
	//////////////////////////////////////////////////////////////////////////////////
	size_t InputEngine::NumDevice() const
	{
		return devices_.size();
	}

	// ˢ������״̬
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputEngine::Update()
	{
		typedef std::set<std::pair<U16, long>, std::less<std::pair<U16, long> >,
			alloc<std::pair<U16, long> > > ActionSetType;
		ActionSetType actions;

		// ���������豸
		for (InputDevicesType::iterator iter = devices_.begin(); iter != devices_.end(); ++ iter)
		{
			InputActionsType theAction((*iter)->Update());

			for (InputActionsType::iterator it = theAction.begin(); it != theAction.end(); ++ it)
			{
				actions.insert(*it);
			}
		}

		// ��ӵ������б�
		return InputActionsType(actions.begin(), actions.end());
	}

	// �ͷ������豸
	//////////////////////////////////////////////////////////////////////////////////
	void InputEngine::UnacquireDevices()
	{
		for (InputDevicesType::iterator iter = devices_.begin(); iter != devices_.end(); ++ iter)
		{
			(*iter)->Unacquire();
		}
	}

	// ��ȡ�豸�ӿ�
	//////////////////////////////////////////////////////////////////////////////////
	InputDevicePtr InputEngine::Device(size_t index) const
	{
		assert(index < this->NumDevice());

		return devices_[index];
	}
}
