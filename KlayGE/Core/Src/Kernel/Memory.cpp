// Memory.cpp
// KlayGE �ڴ溯���� ʵ���ļ�
// Ver 2.1.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.0
// ȥ���˻����� (2004.4.20)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Memory.hpp>

#include <memory.h>

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
	namespace MemoryLib
	{
		// ��destָ��Ĵ�СΪcount���ڴ����Ϊc
		/////////////////////////////////////////////////////////////////////
		void* Set(void* dest, int c, size_t count)
		{
			return memset(dest, c, count);
		}

		// ��buf1��buf2�����ݱȽϣ���ͬ����true
		/////////////////////////////////////////////////////////////////////
		bool Compare(const void* dest, const void* src, size_t count)
		{
			return (0 == memcmp(dest, src, count));
		}

		// ��srcָ��Ĵ�СΪcount���ڴ濽����dest
		//////////////////////////////////////////////////////////////////////////
		void* Copy(void* dest, const void* src, size_t count)
		{
			return memcpy(dest, src, count);
		}
	}
};
