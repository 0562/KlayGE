// MemoryImpl.hpp
// KlayGE �ڴ溯����ʵ�� ͷ�ļ�
// Ver 1.3.8.1
// ��Ȩ����(C) ������, 2002
// Homepage: http://www.enginedev.com
//
// 1.3.8.1
// ��������������򿽱����� (2002.12.5)
//
// ����޸�: 2002.9.20
///////////////////////////////////////////////////////////////////////////////

#ifndef _MEMORYIMPL_HPP
#define _MEMORYIMPL_HPP

namespace KlayGE
{
	namespace KlayGEImpl
	{
		// �ڴ溯���⣬ʹ��x86ָ�
		///////////////////////////////////////////////////////////////////////////////
		class MemStandardLib : public MemoryLib
		{
		public:
			virtual ~MemStandardLib()
				{ }

			virtual void* Set(void* dest, int c, size_t count) const;
			virtual bool  Cmp(const void* buf1, const void* buf2, size_t count) const;
			virtual void* Cpy(void* dest, const void* src, size_t count) const;
		};

		// �ڴ溯���⣬ʹ��MMXָ�
		/////////////////////////////////////////////////////////////////////////////////
		class MemMMXLib : public MemStandardLib
		{
		public:
			virtual ~MemMMXLib()
				{ }

			virtual void* Set(void* dest, int c, size_t count) const;
			virtual bool  Cmp(const void* buf1, const void* buf2, size_t count) const;
			virtual void* Cpy(void* dest, const void* src, size_t count) const;

		private:
			void Cpy64(void* dst, const void* src) const;
		};

		// �ڴ溯���⣬ʹ��3DNowExָ�
		/////////////////////////////////////////////////////////////////////////////////
		class Mem3DNowExLib : public MemMMXLib
		{
		public:
			virtual ~Mem3DNowExLib()
				{ }

			virtual void* Set(void* dest, int c, size_t count) const;
			virtual bool  Cmp(const void* buf1, const void* buf2, size_t count) const;
			virtual void* Cpy(void* dest, const void* src, size_t count) const;

		private:
			void Cpy64(void* dst, const void* src) const;
		};
	}
}

#endif		// _MEMORYIMPL_HPP
