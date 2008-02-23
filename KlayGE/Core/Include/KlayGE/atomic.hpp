// atomic.hpp
// KlayGE ԭ������ ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ���ν��� (2008.2.23)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _ATOMIC_HPP
#define _ATOMIC_HPP

#ifdef KLAYGE_COMPILER_MSVC
#include <windows.h>
#include <intrin.h>
#endif

namespace KlayGE
{
	template <typename T>
	class atomic
	{
	public:
		atomic(T const & rhs);

		operator T();
		atomic& operator=(T const & rhs);

		atomic const & operator++();
		atomic const & operator--();
		atomic operator++(int);
		atomic operator--(int);

		atomic& operator+=(T const & rhs);
		atomic& operator-=(T const & rhs);
		atomic& operator&=(T const & rhs);
		atomic& operator|=(T const & rhs);
		atomic& operator^=(T const & rhs);

		void swap(T const & rhs);
		bool compare_swap(T const & old_val, T const & new_val);
	};

#ifdef KLAYGE_COMPILER_MSVC
	template <>
	class atomic<int32_t>
	{
	public:
		atomic()
		{
		}

		explicit atomic(int32_t rhs)
		{
			InterlockedExchange(&value_, rhs);
		}

		int32_t value() const
		{
			return value_;
		}

		atomic& operator=(int32_t const & rhs)
		{
			InterlockedExchange(&value_, rhs);
			return *this;
		}
		atomic& operator=(atomic const & rhs)
		{
			InterlockedExchange(&value_, rhs.value_);
			return *this;
		}

		bool compare_swap(int32_t const & old_val, int32_t const & new_val)
		{
			return old_val == InterlockedCompareExchange(&value_, new_val, old_val);
		}

		bool operator<(int32_t const & rhs) const
		{
			return value_ < rhs;
		}
		bool operator<(atomic const & rhs) const
		{
			return value_ < rhs.value_;
		}
		bool operator<=(int32_t const & rhs) const
		{
			return value_ <= rhs;
		}
		bool operator<=(atomic const & rhs) const
		{
			return value_ <= rhs.value_;
		}
		bool operator>(int32_t const & rhs) const
		{
			return value_ > rhs;
		}
		bool operator>(atomic const & rhs) const
		{
			return value_ > rhs.value_;
		}
		bool operator>=(int32_t const & rhs) const
		{
			return value_ >= rhs;
		}
		bool operator>=(atomic const & rhs) const
		{
			return value_ >= rhs.value_;
		}

		bool operator==(int32_t const & rhs) const
		{
			return value_ == rhs;
		}
		bool operator==(atomic const & rhs) const
		{
			return value_ == rhs.value_;
		}

		bool operator!=(int32_t const & rhs) const
		{
			return value_ != rhs;
		}
		bool operator!=(atomic const & rhs) const
		{
			return value_ != rhs.value_;
		}

		atomic& operator+=(int32_t const & rhs)
		{
			InterlockedExchangeAdd(&value_, rhs);
			return *this;
		}
		atomic& operator+=(atomic const & rhs)
		{
			InterlockedExchangeAdd(&value_, rhs.value_);
			return *this;
		}

		atomic& operator-=(int32_t const & rhs)
		{
			InterlockedExchangeAdd(&value_, -rhs);
			return *this;
		}

		atomic& operator*=(int32_t const & rhs)
		{
			int32_t comperand;
			int32_t exchange;
			do
			{
				comperand = value_;  
				exchange = comperand * rhs;
			}
			while (comperand != InterlockedCompareExchange(&value_, exchange, comperand));
			return *this;
		}

		atomic& operator/=(int32_t const & rhs)
		{
			int32_t comperand;
			int32_t exchange;
			do
			{
				comperand = value_;  
				exchange = comperand / rhs;
			}
			while (comperand != InterlockedCompareExchange(&value_, exchange, comperand));
			return *this;
		}

		atomic& operator%=(int32_t const & rhs)
		{
			int32_t comperand;
			int32_t exchange;
			do
			{
				comperand = value_;  
				exchange = comperand % rhs;
			}
			while (comperand != InterlockedCompareExchange(&value_, exchange, comperand));
			return *this;
		}

		atomic& operator&=(int32_t const & rhs)
		{
			_InterlockedAnd(&value_, rhs);
			return *this;
		}

		atomic& operator|=(int32_t const & rhs)
		{
			_InterlockedOr(&value_, rhs);
			return *this;
		}

		atomic& operator^=(int32_t const & rhs)
		{
			_InterlockedXor(&value_, rhs);
			return *this;
		}

		atomic const & operator++()
		{
			InterlockedIncrement(&value_);
			return *this;
		}

		atomic const & operator--()
		{
			InterlockedDecrement(&value_);
			return *this;
		}

		atomic operator++(int)
		{
			atomic tmp = *this;
			++ *this;
			return tmp;
		}

		atomic operator--(int)
		{
			atomic tmp = *this;
			-- *this;
			return tmp;
		}

	private:
		int32_t value_;
	};
#endif
}

#endif		// _ATOMIC_HPP
