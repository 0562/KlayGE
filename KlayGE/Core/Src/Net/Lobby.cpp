// Lobby.cpp
// KlayGE ��Ϸ���� ʵ���ļ�
// Ver 1.4.8.3
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 1.4.8.3
// ���ν��� (2003.3.8)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Player.hpp>

#include <algorithm>
#include <ctime>
#include <cstring>

#include <KlayGE/NetMsg.hpp>
#include <KlayGE/Lobby.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	Lobby::Lobby()
	{
		this->socket_.Create(SOCK_DGRAM);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	Lobby::~Lobby()
	{
		Close();
	}

	Lobby::PlayerAddrsIter Lobby::ID(SOCKADDR_IN const & addr)
	{
		for (PlayerAddrsIter iter = this->players_.begin(); iter != this->players_.end(); ++ iter)
		{
			if (0 == std::memcmp(&addr, &(iter->second.addr), sizeof(addr)))
			{
				return iter;
			}
		}

		return this->players_.end();
	}

	// ������Ϸ����
	/////////////////////////////////////////////////////////////////////////////////
	void Lobby::Create(std::string const & Name, char maxPlayers, uint16_t port, Processer const & pro)
	{
		this->LobbyName(Name);

		this->MaxPlayers(maxPlayers);

		this->socket_.Bind(TransAddr("", port));

		SOCKADDR_IN from;
		char revBuf[Max_Buffer];
		char sendBuf[Max_Buffer];
		int numSend = 0;
		for (;;)
		{
			this->Receive(revBuf, sizeof(revBuf), from);

			// ÿ����Ϣǰ�涼����1�ֽڵ���Ϣ����
			char* revPtr(&revBuf[1]);
			char* sendPtr(&sendBuf[1]);
			sendBuf[0] = revBuf[0];

			switch (revBuf[0])
			{
			case MSG_JOIN:
				this->OnJoin(revPtr, sendPtr, numSend, from, pro);
				break;

			case MSG_QUIT:
				this->OnQuit(this->ID(from), sendPtr, numSend, pro);
				break;

			case MSG_GETLOBBYINFO:
				this->OnGetLobbyInfo(sendPtr, numSend, pro);
				break;

			case MSG_NOP:
				this->OnNop(this->ID(from));
				break;

			default:
				pro.OnDefault(revBuf, sizeof(revBuf), sendBuf, numSend, from);
				break;
			}

			if (numSend != 0)
			{
				this->Send(sendBuf, numSend + 1, from);
			}

			// ������Ϣ
			for (PlayerAddrs::iterator iter = players_.begin(); iter != players_.end(); ++ iter)
			{
				SendQueueType& msgs = iter->second.msgs;
				for (SendQueueType::iterator msgiter = msgs.begin();
					msgiter != msgs.end(); ++ msgiter)
				{
					std::vector<char>& msg = *msgiter;

					socket_.SendTo(&msg[0], static_cast<int>(msg.size()), iter->second.addr);
				}
			}

			// ����Ƿ��������û���ʱ
			for (PlayerAddrs::iterator iter = players_.begin(); iter != players_.end();)
			{
				// ����20��
				if (std::time(NULL) - iter->second.time >= 20 * 1000)
				{
					iter = players_.erase(iter);
				}
				else
				{
					++ iter;
				}
			}
		}
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	char Lobby::NumPlayer() const
	{
		char n = 0;
		for (PlayerAddrs::const_iterator iter = this->players_.begin();
			iter != this->players_.end(); ++ iter)
		{
			if (iter->first != 0)
			{
				++ n;
			}
		}

		return n;
	}

	// ���ô�������
	/////////////////////////////////////////////////////////////////////////////////
	void Lobby::LobbyName(std::string const & name)
	{
		if (name.length() > 16)
		{
			this->name_ = name.substr(0, 16);
		}
		else
		{
			this->name_ = name;
		}
	}

	// ��ȡ��������
	/////////////////////////////////////////////////////////////////////////////////
	std::string const & Lobby::LobbyName() const
	{
		return this->name_;
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	void Lobby::MaxPlayers(char maxPlayers)
	{
		this->players_.resize(maxPlayers);
		PlayerAddrs(this->players_).swap(this->players_);

		for (PlayerAddrsIter iter = this->players_.begin();
						iter != this->players_.end(); ++ iter)
		{
			iter->first = 0;
		}
	}

	// ��ȡ�������
	/////////////////////////////////////////////////////////////////////////////////
	char Lobby::MaxPlayers() const
	{
		return static_cast<char>(this->players_.size());
	}

	// �ر���Ϸ����
	/////////////////////////////////////////////////////////////////////////////////
	void Lobby::Close()
	{
		this->socket_.Close();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	int Lobby::Receive(void* buf, int maxSize, SOCKADDR_IN& from)
	{
		return this->socket_.ReceiveFrom(buf, maxSize, from);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	int Lobby::Send(void const * buf, int maxSize, SOCKADDR_IN const & to)
	{
		return this->socket_.SendTo(buf, maxSize, to);
	}


	void Lobby::OnJoin(char* revBuf, char* sendBuf, int& numSend,
							SOCKADDR_IN& from, Processer const & pro)
	{
		// �����ʽ:
		//			Player����		16 �ֽ�

		char id(1);
		PlayerAddrsIter iter(this->players_.begin());
		for (; iter != this->players_.end(); ++ iter, ++ id)
		{
			if (0 == iter->first)
			{
				size_t i(0);
				while (revBuf[i] != 0)
				{
					++ i;
				}
				std::string name(&revBuf[0], i);
				iter->first			= id;
				iter->second.name	= name;
				iter->second.addr	= from;

				pro.OnJoin(iter->first);
				break;
			}
		}

		// ���ظ�ʽ:
		//			Player ID		1 �ֽ�

		// �Ѿ�����
		if (iter == this->players_.end())
		{
			sendBuf[0] = 1;
		}
		else
		{
			sendBuf[0] = 0;
		}

		numSend = 1;
	}

	void Lobby::OnQuit(PlayerAddrsIter iter, char* sendBuf,
							int& numSend, Processer const & pro)
	{
		if (iter != this->players_.end())
		{
			pro.OnQuit(iter->first);
			iter->first = 0;
			sendBuf[0] = 0;
		}
		else
		{
			sendBuf[0] = 1;
		}

		numSend = 1;
	}

	void Lobby::OnGetLobbyInfo(char* sendBuf, int& numSend, Processer const & /*pro*/)
	{
		// ���ظ�ʽ:
		//			��ǰPlayers��	1 �ֽ�
		//			���Players��	1 �ֽ�
		//			Lobby����		16 �ֽ�

		std::fill_n(sendBuf, 18, 0);
		sendBuf[0] = this->NumPlayer();
		sendBuf[1] = this->MaxPlayers();
		this->LobbyName().copy(&sendBuf[2], this->LobbyName().length());
		numSend = 18;
	}

	void Lobby::OnNop(PlayerAddrsIter iter)
	{
		if (iter != this->players_.end())
		{
			iter->second.time = static_cast<uint32_t>(std::time(NULL));
		}
	}
}
