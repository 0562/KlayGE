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
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/Memory.hpp>

#include <algorithm>
#include <ctime>

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

	Lobby::PlayerAddrsIter Lobby::ID(const SOCKADDR_IN& addr)
	{
		for (PlayerAddrsIter iter = this->players_.begin(); iter != this->players_.end(); ++ iter)
		{
			if (0 == MemoryLib::Compare(&addr, &(iter->Addr), sizeof(addr)))
			{
				return iter;
			}
		}

		return this->players_.end();
	}

	// ������Ϸ����
	/////////////////////////////////////////////////////////////////////////////////
	void Lobby::Create(const std::string& Name, char maxPlayers, U16 port, const Processer& pro)
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
			if (iter->ID != 0)
			{
				++ n;
			}
		}

		return n;
	}

	// ���ô�������
	/////////////////////////////////////////////////////////////////////////////////
	void Lobby::LobbyName(const std::string& name)
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
	const std::string& Lobby::LobbyName() const
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
			iter->ID = 0;
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
	int Lobby::Send(const void* buf, int maxSize, const SOCKADDR_IN& to)
	{
		return this->socket_.SendTo(buf, maxSize, to);
	}


	void Lobby::OnJoin(char* revBuf, char* sendBuf, int& numSend,
							SOCKADDR_IN& from, const Processer& pro)
	{
		// �����ʽ:
		//			Player����		16 �ֽ�

		char id(1);
		PlayerAddrsIter iter(this->players_.begin());
		for (; iter != this->players_.end(); ++ iter, ++ id)
		{
			if (0 == iter->ID)
			{
				size_t i(0);
				while (revBuf[i] != 0)
				{
					++ i;
				}
				std::string name(&revBuf[0], i);
				iter->ID	= id;
				iter->Name	= name;
				iter->Addr	= from;

				pro.OnJoin(iter->ID);
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
							int& numSend, const Processer& pro)
	{
		if (iter != this->players_.end())
		{
			this->players_[iter->ID].ID = 0;
			sendBuf[0] = 0;
			pro.OnQuit(iter->ID);
		}
		else
		{
			sendBuf[0] = 1;
		}

		numSend = 1;
	}

	void Lobby::OnGetLobbyInfo(char* sendBuf, int& numSend, const Processer& /*pro*/)
	{
		// ���ظ�ʽ:
		//			��ǰPlayers��	1 �ֽ�
		//			���Players��	1 �ֽ�
		//			Lobby����		16 �ֽ�

		MemoryLib::Zero(sendBuf, 18);
		sendBuf[0] = this->NumPlayer();
		sendBuf[1] = this->MaxPlayers();
		MemoryLib::Copy(&sendBuf[2], this->LobbyName().c_str(), this->LobbyName().length());
		numSend = 18;
	}

	void Lobby::OnNop(PlayerAddrsIter iter)
	{
		if (iter != this->players_.end())
		{
			iter->Time = static_cast<U32>(std::time(NULL));
		}
	}
}
