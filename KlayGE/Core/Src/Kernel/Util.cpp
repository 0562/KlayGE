// Util.cpp
// KlayGE ʵ�ú����� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/select.h>
#endif

#include <cassert>
#include <vector>
#include <algorithm>

#include <KlayGE/Util.hpp>

namespace KlayGE
{
	// ��һ��WStringת��ΪString
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, const std::wstring& src)
	{
#ifdef WIN32
		std::vector<char> vecTemp(::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1,
			NULL, 0, NULL, NULL) - 1);
		::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &vecTemp[0],
			static_cast<int>(vecTemp.size()), NULL, NULL);

		dest.assign(vecTemp.begin(), vecTemp.end());

		return dest;
#endif	// WIN32
	}

	// ��һ��Stringת��ΪString
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, const std::string& src)
	{
		dest = src;

		return dest;
	}

	// ��һ��Stringת��ΪWString
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring& Convert(std::wstring& dest, const std::string& src)
	{
#ifdef WIN32
		std::vector<wchar_t> vecTemp(::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0) - 1);
		::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &vecTemp[0],
			static_cast<int>(vecTemp.size()));

		dest.assign(vecTemp.begin(), vecTemp.end());

		return dest;
#endif		// WIN32
	}

	// ��һ��WStringת��ΪWString
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring& Convert(std::wstring& dest, const std::wstring& src)
	{
		dest = src;

		return dest;
	}

	void Sleep(U32 ms)
	{
#ifdef WIN32
		::Sleep(ms);
#else
		timeval sleeper;

		sleeper.tv_sec = ms / 1000;
		sleeper.tv_usec = (ms % 1000) * 1000;

		select(0, NULL, NULL, NULL, &sleeper);
#endif		// WIN32
	}
}
