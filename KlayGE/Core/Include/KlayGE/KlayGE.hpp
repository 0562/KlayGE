// KlayGE.hpp
// KlayGE ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _KLAYGE_HPP
#define _KLAYGE_HPP

#define KLAYGEMAINVER		2
#define KLAYGESECVER		0
#define KLAYGERELEASEVER	0
#define KLAYGEVERSTR		"KLAYGE 2.0.0"

#define CREATOR(p)	friend class p

namespace KlayGE
{
	template <typename T>
	inline void
	SafeRelease(T& p)
	{
		if (p != 0)
		{
			p->Release();
			p = 0;
		}
	}

	template <typename T>
	inline void
	SafeDelete(T& p)
	{
		if (p != 0)
		{
			delete p;
			p = 0;
		}
	}

	template <typename T>
	inline void
	SafeDeleteArray(T& p)
	{
		if (p != 0)
		{
			delete[] p;
			p = 0;
		}
	}

	enum CPUOptimal
	{
		CPU_Standard,
		CPU_MMX,
		CPU_AMD3DNowEx,
	};
}

#include <KlayGE/Types.hpp>

#endif		// _KLAYGE_HPP