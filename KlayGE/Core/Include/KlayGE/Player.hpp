// Player.cpp
// KlayGE ��Ϸ�� ͷ�ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// �����˷��Ͷ��� (2004.5.28)
//
// 1.4.8.3
// ���ν��� (2003.3.8)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <list>
#include <vector>

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <KlayGE/Socket.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	struct LobbyDes
	{
		char			numPlayer;
		char			maxPlayers;
		std::string		name;
		SOCKADDR_IN		addr;
	};

	class Player
	{
	public:
		Player();
		~Player();

		bool Join(SOCKADDR_IN const & lobbyAddr);
		void Quit();
		void Destroy();
		LobbyDes LobbyInfo();

		void Name(std::string const & name);
		std::string const & Name()
			{ return this->name_; }

		int Receive(void* buf, int maxSize, SOCKADDR_IN& from);
		int Send(void const * buf, int size);

		void ReceiveFunc();

	private:
		Socket		socket_;

		char		playerID_;
		std::string	name_;

		boost::shared_ptr<boost::thread>	receiveThread_;
		bool		receiveLoop_;

		typedef std::list<std::vector<char> > SendQueueType;
		SendQueueType	sendQueue_;
	};
}

#endif			// _PLAYER_HPP
