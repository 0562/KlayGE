// Plane.hpp
// KlayGE ���δ�С ͷ�ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.1
// ���ν��� (2004.4.22)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _SIZE_HPP
#define _SIZE_HPP

#include <boost/operators.hpp>

#include <KlayGE/Vector.hpp>

namespace KlayGE
{
	template <typename T>
	class Size_T : boost::addable<Size_T<T>,
						boost::subtractable<Size_T<T> > >
	{
		template <typename U>
		friend class Size_T;

	public:
		typedef T					value_type;

		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;

		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;

		enum { elem_num = 2 };

	public:
		Size_T()
			{ }
		explicit Size_T(const T* rhs)
			: size_(rhs)
			{ }
		Size_T(const Size_T& rhs)
			: size_(rhs.size_)
			{ }
		template <typename U>
		Size_T(const Size_T<U>& rhs)
			: size_(rhs.size_)
			{ }
		Size_T(const T& cx, const T& cy)
		{
			this->cx() = cx;
			this->cy() = cy;
		}

		// ȡ����
		reference cx()
			{ return size_[0]; }
		const_reference cx() const
			{ return size_[0]; }
		reference cy()
			{ return size_[1]; }
		const_reference cy() const
			{ return size_[1]; }

		// ��ֵ������
		template <typename U>
		Size_T& operator+=(const Size_T<U>& rhs)
		{
			size_ += rhs.size_;
			return *this;
		}
		template <typename U>
		Size_T& operator-=(const Size_T<U>& rhs)
		{
			size_ -= rhs.size_;
			return *this;
		}

		Size_T& operator=(const Size_T& rhs)
		{
			if (this != &rhs)
			{
				size_ = rhs.size_;
			}
			return *this;
		}
		template <typename U>
		Size_T& operator=(const Size_T<U>& rhs)
		{
			if (this != &rhs)
			{
				size_ = rhs.size_;
			}
			return *this;
		}

		// һԪ������
		const Size_T<T> operator+() const
			{ return *this; }
		const Size_T<T> operator-() const
			{ return Size_T<T>(-this->cx(), -this->cy()); }

	private:
		Vector_T<T, elem_num> size_;
	};

	template <typename T>
	inline bool
	operator==(const Size_T<T>& lhs, const Size_T<T>& rhs)
	{
		return (lhs.cx() == rhs.cx()) && (lhs.cy() == rhs.cy());
	}
	template <typename T>
	inline bool
	operator!=(const Size_T<T>& lhs, const Size_T<T>& rhs)
	{
		return !(lhs == rhs);
	}

	typedef Size_T<float> Size;
}

#endif			// _SIZE_HPP
