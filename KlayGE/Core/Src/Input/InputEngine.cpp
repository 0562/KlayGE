// InputEngine.cpp
// KlayGE ���������� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
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
#include <KlayGE/Util.hpp>
#include <KlayGE/MapVector.hpp>

#include <vector>

#include <boost/assert.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <KlayGE/Input.hpp>

namespace KlayGE
{
	class NullInputEngine : public InputEngine
	{
	public:
		std::wstring const & Name() const
		{
			static std::wstring name(L"Null Input Engine");
			return name;
		}

		void EnumDevices()
		{
		}
	};

	// ��������
	//////////////////////////////////////////////////////////////////////////////////
	InputEngine::~InputEngine()
	{
	}

	// ���ؿն���
	//////////////////////////////////////////////////////////////////////////////////
	InputEnginePtr InputEngine::NullObject()
	{
		static InputEnginePtr obj = MakeSharedPtr<NullInputEngine>();
		return obj;
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
			BOOST_FOREACH(BOOST_TYPEOF(devices_)::reference device, devices_)
			{
				device->ActionMap(id, action_handlers_[id].first);
			}
		}
	}

	// ��ȡ�����豸����
	//////////////////////////////////////////////////////////////////////////////////
	size_t InputEngine::NumDevices() const
	{
		return devices_.size();
	}

	// ˢ������״̬
	//////////////////////////////////////////////////////////////////////////////////
	void InputEngine::Update()
	{
		elapsed_time_ = static_cast<float>(timer_.elapsed());
		if (elapsed_time_ > 0.01f)
		{
			timer_.restart();

			BOOST_FOREACH(BOOST_TYPEOF(devices_)::reference device, devices_)
			{
				device->UpdateInputs();
			}

			for (uint32_t id = 0; id < action_handlers_.size(); ++ id)
			{
				MapVector<uint16_t, long> actions;

				// ���������豸
				BOOST_FOREACH(BOOST_TYPEOF(devices_)::reference device, devices_)
				{
					InputActionsType const theAction(device->UpdateActionMap(id));

					// ȥ���ظ��Ķ���
					BOOST_FOREACH(BOOST_TYPEOF(theAction)::const_reference act, theAction)
					{
						if (actions.find(act.first) == actions.end())
						{
							actions.insert(act);

							// ������
							(*action_handlers_[id].second)(*this, act);
						}
					}
				}
			}
		}
	}

	// ��ȡˢ��ʱ����
	//////////////////////////////////////////////////////////////////////////////////
	float InputEngine::ElapsedTime() const
	{
		return elapsed_time_;
	}

	// ��ȡ�豸�ӿ�
	//////////////////////////////////////////////////////////////////////////////////
	InputDevicePtr InputEngine::Device(size_t index) const
	{
		BOOST_ASSERT(index < this->NumDevices());

		return devices_[index];
	}
}
