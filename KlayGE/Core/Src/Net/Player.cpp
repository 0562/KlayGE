// Player.cpp
// KlayGE ��� ʵ���ļ�
// Ver 1.4.8.4
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 1.4.8.3
// ���ν��� (2003.3.8)
//
// 1.4.8.4
// �����˶��߳̽��յ����� (2003.4.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Lobby.hpp>
#include <KlayGE/Memory.hpp>

#include <ctime>

#include <KlayGE/NetMsg.hpp>
#include <KlayGE/Player.hpp>

namespace
{
	class ReceiveThreadFunc
	{
	public:
		ReceiveThreadFunc(KlayGE::Player* player)
			: player_(player)
			{ }

		void operator()()
			{ player_->ReceiveFunc(); }

	private:
		KlayGE::Player* player_;
	};
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	Player::Player()
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	Player::~Player()
	{
		this->Destroy();
	}

	// ��Ϣ���ܺ���
	/////////////////////////////////////////////////////////////////////////////////
	void Player::ReceiveFunc()
	{
		static time_t lastTime = std::time(NULL);

		for (;;)
		{
			if (std::time(NULL) - lastTime >= 10 * 1000)
			{
				char msg(MSG_NOP);
				socket_.Send(&msg, sizeof(msg));
				lastTime = std::time(NULL);
			}

			if (!sendQueue_.empty())
			{
				// ���Ͷ��������Ϣ
				for (SendQueueType::iterator iter = sendQueue_.begin();
					iter != sendQueue_.end(); ++ iter)
				{
					std::vector<char>& msg = *iter;
					socket_.Send(&msg[0], msg.size());
				}
			}

			char revBuf[Max_Buffer];
			MemoryLib::Zero(revBuf, sizeof(revBuf));
			if (socket_.Receive(revBuf, sizeof(revBuf)) != -1)
			{
				U32 ID;
				memcpy(&ID, &revBuf[1], 4);

				// ɾ���ѷ��͵���Ϣ
				for (SendQueueType::iterator iter = sendQueue_.begin();
					iter != sendQueue_.end();)
				{
					std::vector<char>& msg = *iter;

					U32 sendID;
					memcpy(&sendID, &msg[1], 4);
					if (sendID == ID)
					{
						iter = sendQueue_.erase(iter);
					}
					else
					{
						++ iter;
					}
				}

				if (MSG_QUIT == revBuf[0])
				{
					break;
				}
			}
		}
	}

	// ���������
	/////////////////////////////////////////////////////////////////////////////////
	bool Player::Join(SOCKADDR_IN const & lobbyAddr)
	{
		socket_.Close();
		socket_.Create(SOCK_DGRAM);
		socket_.Connect(lobbyAddr);

		socket_.TimeOut(2000);

		char buf[Max_Buffer];
		MemoryLib::Zero(buf, sizeof(buf));

		buf[0] = MSG_JOIN;
		name_.copy(&buf[1], this->name_.length());

		socket_.Send(buf, sizeof(buf));

		socket_.Receive(&playerID_, sizeof(playerID_));
		if (0 == playerID_)
		{
			return false;
		}

		receiveLoop_ = true;
		receiveThread_ = boost::shared_ptr<boost::thread>(new boost::thread(ReceiveThreadFunc(this)));

		return true;
	}

	// �˳�������
	/////////////////////////////////////////////////////////////////////////////////
	void Player::Quit()
	{
		if (receiveLoop_)
		{
			char msg(MSG_QUIT);
			socket_.Send(&msg, sizeof(msg));

			receiveLoop_ = false;
			receiveThread_->join();
			receiveThread_.reset();
		}
	}

	// �������
	/////////////////////////////////////////////////////////////////////////////////
	void Player::Destroy()
	{
		this->Quit();
		socket_.Close();
	}

	LobbyDes Player::LobbyInfo()
	{
		LobbyDes lobbydes;
		lobbydes.numPlayer = 0;
		lobbydes.maxPlayers = 0;

		char msg(MSG_GETLOBBYINFO);
		socket_.Send(&msg, sizeof(msg));

		char buf[18];
		socket_.Receive(buf, sizeof(buf));
		if (MSG_GETLOBBYINFO == buf[0])
		{
			lobbydes.numPlayer = buf[1];
			lobbydes.maxPlayers = buf[2];
			size_t i(0);
			while (buf[3 + i] != 0)
			{
				++ i;
			}
			lobbydes.name = std::string(&buf[3], i);
		}

		return lobbydes;
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	void Player::Name(std::string const & name)
	{
		if (name.length() > 16)
		{
			name_ = name.substr(0, 16);
		}
		else
		{
			name_ = name;
		}
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	int Player::Receive(void* buf, int maxSize, SOCKADDR_IN& from)
	{
		return socket_.ReceiveFrom(buf, maxSize, from);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	int Player::Send(void const * buf, int size)
	{
		return socket_.Send(buf, size);
	}
}
