// alloc.hpp
// KlayGE �ڴ������� ͷ�ļ�
// Ver 2.0.2
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.2
// ���ν��� (2003.10.23)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _ALLOC_HPP
#define _ALLOC_HPP

namespace KlayGE
{
	class Malloc
	{
	public:
		static void* Allocate(size_t n)
			{ return malloc(n); }

		static void Deallocate(void* p)
			{ return free(p); }
	};

	// �ڴ�أ�inst��ʾʵ����
	template <size_t inst, typename Allocator>
	class MemoryPool
	{
	private:
		enum
		{
			ALIGN		= 8,
			MAX_BYTES	= 128,
			NFREELISTS	= 16,
		};

		union Obj
		{
			Obj*	next;
			char	client[1];
		};

		static Obj* volatile freeListHead_[NFREELISTS];

		// ����һ���ڴ�
		// allocNum��ʾ��ʵ���뵽���ڴ����
		static void* ChunkAlloc(size_t blockSize, size_t numBlock, size_t& numAlloc)
		{
			void* ret(0);

			// �Զ�������Ҫ�Ĵ�С
			for (numAlloc = numBlock; (numAlloc > 1) && (0 == ret); numAlloc /= 2)
			{
				ret = Allocator::Allocate(blockSize * numAlloc);
			}

			if (0 == ret)
			{
				// �������޷������뵽�ڴ棬�������free list�ﲦ�ռ�
				for (size_t i = 2; blockSize * i < MAX_BYTES; ++ i)
				{
					Obj* volatile& theHead = freeListHead_[FreeListIndex(blockSize * i)];

					if (theHead != 0)
					{
						ret	= theHead;
						theHead = theHead->next;

						numAlloc = i;

						break;
					}
				}

				// ������ڴ�ض����ˣ�ֻ���׳��쳣
				if (0 == ret)
				{
					throw std::bad_alloc("MemoryPool bad allocation");
				}
			}

			return ret;
		}

		static size_t FreeListIndex(size_t n)
			{ return (n + ALIGN - 1) / ALIGN - 1; }

		// ������ALIGN��������
		static size_t RoundUp(size_t n)
			{ return (n + ALIGN - 1) & (~(ALIGN - 1)); }

	public:
		static void* Allocate(size_t n)
		{
			void* ret(0);

			// ̫�󣬲��ܷ����ڴ����
			if (n > MAX_BYTES)
			{
				ret = Allocator::Allocate(n);
			}
			else
			{
				Obj* volatile& theHead = freeListHead_[FreeListIndex(n)];

				if (theHead != 0)
				{
					ret	= theHead;
					theHead	= theHead->next;
				}
				else
				{
					const size_t blockSize(RoundUp(n));
					size_t numAlloc;

					// ����һ���㹻��Ŀռ�
					char* chuck(static_cast<char*>(ChunkAlloc(blockSize, 256, numAlloc)));
					ret = chuck;

					chuck += blockSize;
					theHead = static_cast<Obj*>(static_cast<void*>(chuck));

					// �����µ�free list
					Obj* curObj(0);
					for (size_t i = 1; i < numAlloc - 1; ++ i)
					{
						curObj = static_cast<Obj*>(static_cast<void*>(chuck));
						chuck += blockSize;

						curObj->next = static_cast<Obj*>(static_cast<void*>(chuck));
					}
					curObj->next = 0;
				}
			}

			return ret;
		}

		static void Deallocate(void* p, size_t n)
		{
			if (p != 0)
			{
				if (n > MAX_BYTES)
				{
					Allocator::Deallocate(p);
				}
				else
				{
					Obj* volatile& theHead = freeListHead_[FreeListIndex(n)];

					// �ѿռ����free listͷ
					Obj* space(static_cast<Obj*>(p));
					space->next = theHead;
					theHead = space;
				}
			}
		}
	};

	// �����ڴ�ؿ��пռ�ı�
	template <size_t inst, typename Allocator>
	typename MemoryPool<inst, Allocator>::Obj* volatile
		MemoryPool<inst, Allocator>::freeListHead_[MemoryPool<inst, Allocator>::NFREELISTS] =
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	template <typename T>
	class alloc;

	template <>
	class alloc<void>
	{
	public:
		typedef void*			pointer;
		typedef const void*		const_pointer;

		typedef void			value_type;

		template <typename U>
		struct rebind
		{
			typedef alloc<U> other;
		};
	};

	template <typename T>
	class alloc
	{
	public:
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef T				value_type;

		template <typename U>
		struct rebind
		{
			typedef alloc<U> other;
		};

		alloc() throw()
			{ }
		alloc(const alloc& /*rhs*/) throw()
			{ }
		template <typename U>
		alloc(const alloc<U>& /*rhs*/) throw()
			{ }
		~alloc() throw()
			{ }

		pointer address(reference x) const
			{ return &x; }
		const_pointer address(const_reference x) const
			{ return &x; }

		pointer allocate(size_type n, alloc<void>::pointer /*hint*/ = 0)
			{ return static_cast<pointer>(MemoryPool<0, Malloc>::Allocate(n * sizeof(T))); }
		void deallocate(pointer p, size_type n)
			{ MemoryPool<0, Malloc>::Deallocate(p, n * sizeof(T)); }

		size_type max_size() const
			{ return size_t(-1) / sizeof(T); }

		void construct(pointer p, const T& val)
			{ new(p) T(val); }
		void destroy(pointer p)
			{ p->~T(); }
	};

	template <typename T1, typename T2>
	bool operator==(const alloc<T1>& /*lhs*/, const alloc<T2>& /*rhs*/) throw()
		{ return true; }
	template <typename T1, typename T2>
	bool operator!=(const alloc<T1>& /*lhs*/, const alloc<T2>& /*rhs*/) throw()
		{ return false; }
}

#endif			// _ALLOC_HPP
