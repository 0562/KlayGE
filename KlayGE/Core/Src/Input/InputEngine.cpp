// InputEngine.cpp
// KlayGE ���������� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ������Action map id (2005.4.3)
//
// 2.1.3
// ���㷨������дѭ�� (2004.10.16)
//
// 2.0.0
// ���ν��� (2003.8.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/MapVector.hpp>

#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>

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
	uint32_t InputEngine::ActionMap(InputActionMap const & actionMap, bool reenumerate)
	{
		uint32_t id = 0;

		// �����µĶ�����ʽ
		if (!actionMaps_.empty())
		{
			id = actionMaps_.rbegin()->first + 1;
		}
		actionMaps_[id] = actionMap;

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
		std::for_each(devices_.begin(), devices_.end(),
			boost::bind(&InputDevice::ActionMap, _1, actionMaps_));

		return id;
	}

	// ��ȡ�����豸����
	//////////////////////////////////////////////////////////////////////////////////
	size_t InputEngine::NumDevice() const
	{
		return devices_.size();
	}

	// ˢ������״̬
	//////////////////////////////////////////////////////////////////////////////////
	InputActionsType InputEngine::Update(uint32_t id)
	{
		typedef MapVector<uint16_t, long> ActionSetType;
		ActionSetType actions;

		// ���������豸
		for (InputDevicesType::iterator iter = devices_.begin(); iter != devices_.end(); ++ iter)
		{
			InputActionsType const theAction((*iter)->Update(id));

			// ȥ���ظ��Ķ���
			for (InputActionsType::const_iterator i = theAction.begin(); i != theAction.end(); ++ i)
			{
				if (actions.find(i->first) == actions.end())
				{
					actions.insert(*i);
				}
			}
		}

		// ��ӵ������б�
		return InputActionsType(actions.begin(), actions.end());
	}

	// �ͷ������豸
	//////////////////////////////////////////////////////////////////////////////////
	void InputEngine::UnacquireDevices()
	{
		std::for_each(devices_.begin(), devices_.end(),
			boost::bind(&InputDevice::Unacquire, _1));
	}

	// ��ȡ�豸�ӿ�
	//////////////////////////////////////////////////////////////////////////////////
	InputDevicePtr InputEngine::Device(size_t index) const
	{
		BOOST_ASSERT(index < this->NumDevice());

		return devices_[index];
	}
}
