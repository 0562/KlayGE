// Socket.cpp
// KlayGE �׽��� ʵ���ļ�
// Ver 1.4.8.4
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 1.4.8.1
// ���ν��� (2003.1.23)
//
// 1.4.8.4
// TransAddr֧�������ֽ��� (2003.4.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <cassert>
#include <cstring>

#include <KlayGE/Socket.hpp>

#pragma comment(lib, "wsock32.lib")

// ��ʼ��Winsock
/////////////////////////////////////////////////////////////////////////////////
class WSAIniter
{
public:
	WSAIniter()
	{
		WSADATA wsaData;

		WSAStartup(MAKEWORD(2, 0), &wsaData);
	}

	~WSAIniter()
	{
		WSACleanup();
	}
} wsaInit;

namespace KlayGE
{
	// ���������ַ
	/////////////////////////////////////////////////////////////////////////////////
	SOCKADDR_IN TransAddr(std::string const & address, uint16_t port)
	{
		SOCKADDR_IN sockAddr_in;
		std::memset(&sockAddr_in, 0, sizeof(sockAddr_in));

		if (address.empty())
		{
			sockAddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			sockAddr_in.sin_addr.s_addr = inet_addr(address.c_str());
		}

		if (INADDR_NONE == sockAddr_in.sin_addr.s_addr)
		{
			LPHOSTENT pHostEnt = gethostbyname(address.c_str());
			if (pHostEnt != NULL)
			{
				std::memcpy(&sockAddr_in.sin_addr.s_addr,
					pHostEnt->h_addr_list[0], pHostEnt->h_length);
			}
			else
			{
				THR(E_FAIL);
			}
		}

		sockAddr_in.sin_family = AF_INET;
		sockAddr_in.sin_port = htons(port);

		return sockAddr_in;
	}

	std::string TransAddr(SOCKADDR_IN const & sockAddr, uint16_t& port)
	{
		port = ntohs(sockAddr.sin_port);
		return std::string(inet_ntoa(sockAddr.sin_addr));
	}

	// ��ȡ������ַ
	/////////////////////////////////////////////////////////////////////////////////
	IN_ADDR Host()
	{
		IN_ADDR addr;
		memset(&addr, 0, sizeof(addr));

		char host[256];
		if (0 == gethostname(host, sizeof(host)))
		{
			HOSTENT* pHostEnt = gethostbyname(host);
			std::memcpy(&addr.S_un.S_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);
		}

		return addr;
	}


	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	Socket::Socket()
			: socket_(INVALID_SOCKET)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	Socket::~Socket()
	{
		this->Close();
	}

	// �����׽���
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::Create(int socketType, int protocolType, int addressFormat)
	{
		this->Close();

		this->socket_ = socket(addressFormat, socketType, protocolType);
		assert(this->socket_ != INVALID_SOCKET);
	}

	// �ر��׽���
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::Close()
	{
		if (this->socket_ != INVALID_SOCKET)
		{
	#ifdef _WIN32
			closesocket(this->socket_);
	#else
			close(this->socket_);
	#endif
			this->socket_ = INVALID_SOCKET;
		}
	}

	// �����Ӧ��
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::Accept(Socket& connectedSocket, SOCKADDR_IN& sockAddr)
	{
		connectedSocket.Close();

		int len(sizeof(sockAddr));
		connectedSocket.socket_ = accept(this->socket_,
			reinterpret_cast<SOCKADDR*>(&sockAddr), &len);
	}

	void Socket::Accept(Socket& connectedSocket)
	{
		connectedSocket.Close();

		connectedSocket.socket_ = accept(this->socket_, NULL, NULL);
	}

	// �󶨶˿�
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::Bind(SOCKADDR_IN const & sockAddr)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(bind(this->socket_, reinterpret_cast<SOCKADDR const *>(&sockAddr),
			sizeof(sockAddr)) != SOCKET_ERROR);
	}

	// �׽���IO����
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::IOCtl(long command, uint32_t* argument)
	{
		assert(this->socket_ != INVALID_SOCKET);

	#ifdef _WIN32
		Verify(ioctlsocket(this->socket_, command, argument) != SOCKET_ERROR);
	#else
		Verify(ioctl(this->socket_, command, argument) != SOCKET_ERROR);
	#endif
	}

	// ����˼���
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::Listen(int connectionBacklog)
	{
		assert(this->socket_ != INVALID_SOCKET);
		
		Verify(listen(this->socket_, connectionBacklog) != SOCKET_ERROR);
	}

	// �����ӵ�����·�������
	/////////////////////////////////////////////////////////////////////////////////
	int Socket::Send(void const * buf, int len, int flags)
	{
		assert(this->socket_ != INVALID_SOCKET);

		return send(this->socket_, static_cast<char const *>(buf), len, flags);
	}

	// �����ӵ�����½�������
	/////////////////////////////////////////////////////////////////////////////////
	int Socket::Receive(void* buf, int len, int flags)
	{
		assert(this->socket_ != INVALID_SOCKET);

		return recv(this->socket_, static_cast<char*>(buf), len, flags);
	}

	// ǿ�ƹر�
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::ShutDown(ShutDownMode how)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(shutdown(this->socket_, how) != SOCKET_ERROR);
	}

	// �����ӵ�����»�ȡ������
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::PeerName(SOCKADDR_IN& sockAddr, int& len)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(getpeername(this->socket_,
			reinterpret_cast<SOCKADDR*>(&sockAddr), &len) != SOCKET_ERROR);
	}

	// ��ȡ�׽�������
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::SockName(SOCKADDR_IN& sockAddr, int& len)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(getsockname(this->socket_,
			reinterpret_cast<SOCKADDR*>(&sockAddr), &len) != SOCKET_ERROR);
	}

	// �����׽��ֲ���
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::SetSockOpt(int optionName, void const * optionValue, int optionLen, int level)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(setsockopt(this->socket_, level, optionName,
			static_cast<char const *>(optionValue), optionLen) != SOCKET_ERROR);
	}

	// ��ȡ�׽��ֲ���
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::GetSockOpt(int optionName, void* optionValue, int& optionLen, int level)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(getsockopt(this->socket_, level, optionName,
			static_cast<char*>(optionValue), &optionLen) != SOCKET_ERROR);
	}

	// ����������½�������
	/////////////////////////////////////////////////////////////////////////////////
	int Socket::ReceiveFrom(void* buf, int len, SOCKADDR_IN& sockFrom, int flags)
	{
		assert(this->socket_ != INVALID_SOCKET);

		int fromLen(sizeof(sockFrom));
		return recvfrom(this->socket_, static_cast<char*>(buf), len, flags,
			reinterpret_cast<SOCKADDR*>(&sockFrom), &fromLen);
	}

	// ����������·�������
	/////////////////////////////////////////////////////////////////////////////////
	int Socket::SendTo(void const * buf, int len, SOCKADDR_IN const & sockTo, int flags)
	{
		assert(this->socket_ != INVALID_SOCKET);

		return sendto(this->socket_, static_cast<char const *>(buf), len, flags,
			reinterpret_cast<SOCKADDR const *>(&sockTo), sizeof(sockTo));
	}

	// ���ӷ����
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::Connect(SOCKADDR_IN const & sockAddr)
	{
		assert(this->socket_ != INVALID_SOCKET);

		Verify(connect(this->socket_,
			reinterpret_cast<SOCKADDR const *>(&sockAddr), sizeof(sockAddr)) != SOCKET_ERROR);
	}

	// ���ó�ʱʱ��
	/////////////////////////////////////////////////////////////////////////////////
	void Socket::TimeOut(uint32_t MicroSecs)
	{
		timeval timeOut;

		timeOut.tv_sec = MicroSecs / 1000;
		timeOut.tv_usec = MicroSecs % 1000;

		SetSockOpt(SO_RCVTIMEO, &timeOut, sizeof(timeOut));
		SetSockOpt(SO_SNDTIMEO, &timeOut, sizeof(timeOut));
	}

	// ��ȡ��ʱʱ��
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t Socket::TimeOut()
	{
		timeval timeOut;
		int len(sizeof(timeOut));

		GetSockOpt(SO_RCVTIMEO, &timeOut, len);

		return timeOut.tv_sec * 1000 + timeOut.tv_usec;
	}
}