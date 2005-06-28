// Util.cpp
// KlayGE ʵ�ú����� ʵ���ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.3.0
// ����libc�ĺ���ʵ����Convert (2004.11.24)
//
// 2.2.0
// ��Boost��д��Sleep (2004.10.25)
//
// 2.1.2
// �����˱��غ������ʽ��ת������ (2004.6.2)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>

#ifdef WIN32
	#define NOMINMAX
	#include <windows.h>
#else
	#include <cerrno>
#endif

#include <cwchar>
#include <clocale>
#include <cassert>
#include <vector>
#include <algorithm>

#include <KlayGE/Util.hpp>

namespace KlayGE
{
	// ��һ��wstringת��Ϊstring
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, std::wstring const & src)
	{
		std::setlocale(LC_CTYPE, "");

		size_t const mbs_len = std::wcstombs(NULL, src.c_str(), 0);
		std::vector<char> tmp(mbs_len + 1);
		std::wcstombs(&tmp[0], src.c_str(), tmp.size());

		dest.assign(tmp.begin(), tmp.end() - 1);

		return dest;
	}

	// ��һ��stringת��Ϊstring
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, std::string const & src)
	{
		dest = src;

		return dest;
	}

	// ��һ��stringת��Ϊwstring
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring& Convert(std::wstring& dest, std::string const & src)
	{
		std::setlocale(LC_CTYPE, "");

		size_t const wcs_len = std::mbstowcs(NULL, src.c_str(), 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		std::mbstowcs(&tmp[0], src.c_str(), src.size());

		dest.assign(tmp.begin(), tmp.end() - 1);

		return dest;
	}

	// ��һ��wstringת��Ϊwstring
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring& Convert(std::wstring& dest, std::wstring const & src)
	{
		dest = src;

		return dest;
	}

	// ��ͣ������
	/////////////////////////////////////////////////////////////////////////////////
	void Sleep(uint32_t ms)
	{
		int const MILLISECONDS_PER_SECOND = 1000;
		int const NANOSECONDS_PER_MILLISECOND = 1000000;

		boost::xtime xt;

		boost::xtime_get(&xt, boost::TIME_UTC);
		xt.sec += ms / MILLISECONDS_PER_SECOND;
		boost::thread::sleep(xt);

		boost::xtime_get(&xt, boost::TIME_UTC);
		xt.nsec += (ms % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND;
		boost::thread::sleep(xt);
	}

	// uint32_t���ظ�ʽ�������ʽ
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t ToNet(uint32_t host)
	{
		union
		{
			uint8_t byte[sizeof(uint32_t) / sizeof(uint8_t)];
			uint32_t net;
		} ret;

		ret.byte[0] = static_cast<uint8_t>((host & 0xFF000000) >> 24);
		ret.byte[1] = static_cast<uint8_t>((host & 0x00FF0000) >> 16);
		ret.byte[2] = static_cast<uint8_t>((host & 0x0000FF00) >> 8);
		ret.byte[3] = static_cast<uint8_t>((host & 0x000000FF) >> 0);

		return ret.net;
	}

	// uint16_t���ظ�ʽ�������ʽ
	/////////////////////////////////////////////////////////////////////////////////
	uint16_t ToNet(uint16_t host)
	{
		union
		{
			uint8_t byte[sizeof(uint16_t) / sizeof(uint8_t)];
			uint16_t net;
		} ret;

		ret.byte[0] = static_cast<uint8_t>((host & 0xFF00) >> 8);
		ret.byte[1] = static_cast<uint8_t>((host & 0x00FF) >> 0);

		return ret.net;
	}

	// uint32_t�����ʽ�����ظ�ʽ
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t ToHost(uint32_t net)
	{
		return ToNet(net);
	}

	// uint16_t�����ʽ�����ظ�ʽ
	/////////////////////////////////////////////////////////////////////////////////
	uint16_t ToHost(uint16_t net)
	{
		return ToNet(net);
	}

	// ��ȡ��һ���������
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t LastError()
	{
#ifdef WIN32
		return ::GetLastError();
#else
		return errno;
#endif
	}
}
