// Event.hpp
// KlayGE �¼�ģ�� ͷ�ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// ���ν��� (2005.6.14)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _EVENT_HPP
#define _EVENT_HPP

#include <list>

#pragma warning(push)
#pragma warning(disable: 4127 4189)
#include <boost/function.hpp>
#pragma warning(pop)

namespace KlayGE
{
	template <typename Sender, typename EventArg>
	class Event
	{
	public:
		typedef boost::function<void (Sender const &, EventArg const &)> event_handler;
		typedef std::list<event_handler> event_handles_type;

	public:
		Event(Sender const & sender)
			: sender_(&sender)
		{
		}

		void operator+=(event_handler const & handler)
		{
			handlers_.push_back(handler);
		}

		void operator()(EventArg const & arg)
		{
			for (typename event_handles_type::iterator iter = handlers_.begin();
					iter != handlers_.end(); ++ iter)
			{
				(*iter)(*sender_, arg);
			}
		}

	private:
		Sender const * sender_;
		event_handles_type handlers_;
	};
}

#endif			// _EVENT_HPP
