// Memory.hpp
// KlayGE �ڴ溯���� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2001--2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.1.20)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////

#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#pragma comment(lib, "KlayGE_Core.lib")

// �Ż����ڴ溯��
/////////////////////////////////////////////////////////////////////
namespace KlayGE
{
	namespace MemoryLib
	{
		void* Set(void* dest, int c, size_t count);
		bool Compare(const void* buf1, const void* buf2, size_t count);
		void* Copy(void* dest, const void* src, size_t count);

		inline void* Zero(void* dest, size_t count)
			{ return Set(dest, 0, count); }
	};
}

#endif			// _MEMORY_HPP
