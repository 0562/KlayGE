// BSTR.cpp
// KlayGE ���ϵͳBSTR ʵ���ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://www.klayge.org
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#ifdef KLAYGE_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <cstring>

#include "BSTR.hpp"

namespace KlayGE
{
	BSTR AllocBSTR(std::wstring const & sz)
	{
#ifdef KLAYGE_PLATFORM_WINDOWS
		return ::SysAllocString(sz.c_str());
#else
		if (sz.empty())
		{
			return NULL;
		}

		uint32_t strLen = sz.size();
		size_t len = (strLen + 1) * sizeof(sz[0]);
		uint8_t* p = new uint8_t[len + sizeof(uint32_t)];
		std::memcpy(p, &strLen, sizeof(strLen));
		BSTR bstr = reinterpret_cast<BSTR>(p + sizeof(strLen));
		std::memmove(bstr, sz.c_str(), len);
		return bstr;
#endif
	}

	void FreeBSTR(BSTR bstr)
	{
#ifdef KLAYGE_PLATFORM_WINDOWS
		::SysFreeString(bstr);
#else
		if (bstr)
		{
			uint8_t* p = reinterpret_cast<uint8_t*>(bstr);
			p -= sizeof(uint32_t);
			delete[] p;
		}
#endif
	}
}
