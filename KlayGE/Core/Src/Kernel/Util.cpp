// Util.cpp
// KlayGE ʵ�ú����� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// �����˱��غ������ʽ��ת������ (2004.6.2)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>

#ifdef WIN32
	#include <windows.h>
#endif

#include <cassert>
#include <vector>
#include <algorithm>

#include <KlayGE/Util.hpp>

namespace KlayGE
{
	// ��һ��WStringת��ΪString
	/////////////////////////////////////////////////////////////////////////////////
	std::string& Convert(std::string& dest, std::wstring const & src)
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
	std::string& Convert(std::string& dest, std::string const & src)
	{
		dest = src;

		return dest;
	}

	// ��һ��Stringת��ΪWString
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring& Convert(std::wstring& dest, std::string const & src)
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
	std::wstring& Convert(std::wstring& dest, std::wstring const & src)
	{
		dest = src;

		return dest;
	}

	// ��ͣ������
	/////////////////////////////////////////////////////////////////////////////////
	void Sleep(U32 ms)
	{
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC);
		xt.nsec += ms;
		boost::thread::sleep(xt);
	}

	// U32���ظ�ʽ�������ʽ
	/////////////////////////////////////////////////////////////////////////////////
	U32 ToNet(U32 host)
	{
		union
		{
			U8 byte[sizeof(U32) / sizeof(U8)];
			U32 net;
		} ret;

		ret.byte[0] = static_cast<U8>((host & 0xFF000000) >> 24);
		ret.byte[1] = static_cast<U8>((host & 0x00FF0000) >> 16);
		ret.byte[2] = static_cast<U8>((host & 0x0000FF00) >> 8);
		ret.byte[3] = static_cast<U8>((host & 0x000000FF) >> 0);

		return ret.net;
	}

	// U16���ظ�ʽ�������ʽ
	/////////////////////////////////////////////////////////////////////////////////
	U16 ToNet(U16 host)
	{
		union
		{
			U8 byte[sizeof(U16) / sizeof(U8)];
			U16 net;
		} ret;

		ret.byte[0] = static_cast<U8>((host & 0xFF00) >> 8);
		ret.byte[1] = static_cast<U8>((host & 0x00FF) >> 0);

		return ret.net;
	}

	// U32�����ʽ�����ظ�ʽ
	/////////////////////////////////////////////////////////////////////////////////
	U32 ToHost(U32 net)
	{
		return ToNet(net);
	}

	// U16�����ʽ�����ظ�ʽ
	/////////////////////////////////////////////////////////////////////////////////
	U16 ToHost(U16 net)
	{
		return ToNet(net);
	}
}
