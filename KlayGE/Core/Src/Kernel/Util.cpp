// Util.cpp
// KlayGE ʵ�ú����� ʵ���ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://www.klayge.org
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
#include <KlayGE/ResLoader.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4127 4244 4512 6328 6330)
#endif
#include <boost/date_time.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4244)
#endif
#include <boost/thread/thread.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef KLAYGE_PLATFORM_WINDOWS
	#include <windows.h>
#else
	#include <cerrno>
#endif

#include <cstdlib>
#include <cwchar>
#include <clocale>
#include <vector>
#include <algorithm>
#include <boost/assert.hpp>

#include <KlayGE/Util.hpp>

namespace KlayGE
{
	// ��һ��wstringת��Ϊstring
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, std::wstring const & src)
	{
#ifdef KLAYGE_PLATFORM_WINDOWS
		int const mbs_len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), nullptr, 0, nullptr, nullptr);
		std::vector<char> tmp(mbs_len + 1);
		WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], mbs_len, nullptr, nullptr);
#else
		std::setlocale(LC_CTYPE, "");

		size_t const mbs_len = wcstombs(nullptr, src.c_str(), 0);
		std::vector<char> tmp(mbs_len + 1);
		wcstombs(&tmp[0], src.c_str(), tmp.size());
#endif

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
#ifdef KLAYGE_PLATFORM_WINDOWS
		int const wcs_len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), nullptr, 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], wcs_len);
#else
		std::setlocale(LC_CTYPE, "");

		size_t const wcs_len = mbstowcs(nullptr, src.c_str(), 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		mbstowcs(&tmp[0], src.c_str(), src.size());
#endif

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
		boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
	}

	// Endian���л�
	/////////////////////////////////////////////////////////////////////////////////
	template <>
	void EndianSwitch<2>(void* p)
	{
		uint8_t* bytes = static_cast<uint8_t*>(p);
		std::swap(bytes[0], bytes[1]);
	}

	template <>
	void EndianSwitch<4>(void* p)
	{
		uint8_t* bytes = static_cast<uint8_t*>(p);
		std::swap(bytes[0], bytes[3]);
		std::swap(bytes[1], bytes[2]);
	}

	template <>
	void EndianSwitch<8>(void* p)
	{
		uint8_t* bytes = static_cast<uint8_t*>(p);
		std::swap(bytes[0], bytes[7]);
		std::swap(bytes[1], bytes[6]);
		std::swap(bytes[2], bytes[5]);
		std::swap(bytes[3], bytes[4]);
	}

	// ��ȡ��һ���������
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t LastError()
	{
#ifdef KLAYGE_PLATFORM_WINDOWS
		return ::GetLastError();
#else
		return errno;
#endif
	}

	std::string ReadShortString(ResIdentifierPtr const & res)
	{
		uint8_t len;
		res->read(&len, sizeof(len));

		std::string tmp;
		if (len > 0)
		{
			tmp.resize(len);
			res->read(&tmp[0], len * sizeof(tmp[0]));
		}

		return tmp;
	}

	void WriteShortString(std::ostream& os, std::string const & str)
	{
		uint8_t len = static_cast<uint8_t>(std::min(str.size(), static_cast<size_t>(255)));
		os.write(reinterpret_cast<char*>(&len), sizeof(len));

		if (len > 0)
		{
			os.write(&str[0], len * sizeof(str[0]));
		}
	}
}
