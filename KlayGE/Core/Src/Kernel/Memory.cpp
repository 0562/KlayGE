// Memory.cpp
// KlayGE �ڴ溯���� ʵ���ļ�
// Ver 1.4.8.5
// ��Ȩ����(C) ������, 2001--2003
// Homepage: http://www.enginedev.com
//
// 1.2.8.10
// �Ľ���MemFuncsInstance (2002.10.24)
//
// 1.3.8.2
// �ı��˳�ʼ������ (2002.12.28)
//
// 1.4.8.5
// ������MemoryInterface (2003.4.20)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Cpu.hpp>
#include <KlayGE/Memory.hpp>

#include "MemoryImpl.hpp"

/*
�㷨:
void* MemSet(void* pDest, int c, size_t Count)
{
	while (0 != Count)
	{
		*pDest = c;
		++ pDest;
		-- Count;
	}

	return pDest;
}

bool MemCmp(const void* pBuf1, const void* pBuf2, size_t Count)
{
	while ((0 != Count) && (*pBuf1 == *pBuf2))
	{
		++ pBuf1;
		++ pBuf2
		-- Count;
	}

	return (0 == (*pBuf1 - *pBuf2));
}

void* MemCpy(void* pDest, const void* pSrc, size_t Count)
{
	while (0 != Count)
	{
		*pDest = *pSrc;
		++ pDest;
		++ pSrc;
		-- Count;
	}

	return pDest;
}
*/

namespace KlayGE
{
	MemoryLib* MemoryLib::Create(CPUOptimal cpu)
	{
		switch (cpu)
		{
		case CPU_Standard:
			return new KlayGEImpl::MemStandardLib;

		case CPU_MMX:
			return new KlayGEImpl::MemMMXLib;

		case CPU_AMD3DNowEx:
			return new KlayGEImpl::Mem3DNowExLib;
		}

		return NULL;
	}
};
