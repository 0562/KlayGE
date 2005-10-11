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
	}

	// ���ö�����ʽ
	//////////////////////////////////////////////////////////////////////////////////
	void InputEngine::ActionMap(InputActionMap const & actionMap,
		action_handler_t handler, bool reenumerate)
	{
		// �����µĶ�����ʽ
		action_handlers_.push_back(std::make_pair(actionMap, handler));

		// ֻ�е�����ʱָ��Ҫ����ö��ʱ�����ٲ���ö���豸
		// �豸�б��п�����ѭ����ʹ�ã������ʱö���豸�п����������
		if (reenumerate)
		{
			// �����ǰö�ٵ��豸
			devices_.clear();

			// ����ö�ٺ��ʵ��豸
			this->EnumDevices();
		}

		// �Ե�ǰ�豸Ӧ���µĶ���ӳ��
		for (uint32_t id = 0; id < action_handlers_.size(); ++ id)
		{
			for (InputDevicesType::iterator iter = devices_.begin();
				iter != devices_.end(); ++ iter)
			{
				(*iter)->ActionMap(id, action_handlers_[id].first);
			}
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
	void InputEngine::Update()
	{
		for (uint32_t id = 0; id < action_handlers_.size(); ++ id)
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

						// ������
						action_handlers_[id].second(*i);
					}
				}
			}
		}
	}

	// ��ȡ�豸�ӿ�
	//////////////////////////////////////////////////////////////////////////////////
	InputDevicePtr InputEngine::Device(size_t index) const
	{
		BOOST_ASSERT(index < this->NumDevice());

		return devices_[index];
	}
}
