// Rect.hpp
// KlayGE ���� ͷ�ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.1
// ���ν��� (2004.4.22)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _RECT_HPP
#define _RECT_HPP

#include <boost/operators.hpp>

#include <KlayGE/Vector.hpp>
#include <KlayGE/Size.hpp>

namespace KlayGE
{
	template <typename T>
	class Rect_T : boost::addable<Rect_T<T>,
						boost::addable2<Rect_T<T>, Vector_T<T, 2>, 
						boost::subtractable<Rect_T<T>,
						boost::subtractable2<Rect_T<T>, Vector_T<T, 2>,
						boost::andable<Rect_T<T>,
						boost::orable<Rect_T<T> > > > > > >
	{
		template <typename U>
		friend class Rect_T;

	public:
		typedef T					value_type;

		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;

		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;

		enum { elem_num = 4 };

	public:
		Rect_T()
			{ }
		explicit Rect_T(const T* rhs)
			: rect_(rhs)
			{ }
		Rect_T(const Rect_T& rhs)
			: rect_(rhs.rect_)
			{ }
		template <typename U>
		Rect_T(const Rect_T<U>& rhs)
			: rect_(rhs.rect_)
			{ }
		Rect_T(const T& left, const T& top, const T& right, const T& bottom)
		{
			this->left()	= left;
			this->top()		= top;
			this->right()	= right;
			this->bottom()	= bottom;
		}

		// ȡ����
		reference left()
			{ return rect_[0]; }
		const_reference left() const
			{ return rect_[0]; }
		reference top()
			{ return rect_[1]; }
		const_reference top() const
			{ return rect_[1]; }
		reference right()
			{ return rect_[2]; }
		const_reference right() const
			{ return rect_[2]; }
		reference bottom()
			{ return rect_[3]; }
		const_reference bottom() const
			{ return rect_[3]; }

		// ��ֵ������
		template <typename U>
		Rect_T& operator+=(const Vector_T<U, 2>& rhs)
		{
			rect_ += rhs;
			return *this;
		}
		template <typename U>
		Rect_T& operator-=(const Vector_T<U, 2>& rhs)
		{
			rect_ -= rhs;
			return *this;
		}
		template <typename U>
		Rect_T& operator+=(const Rect_T<U>& rhs)
		{
			rect_ += rhs.rect_;
			return *this;
		}
		template <typename U>
		Rect_T& operator-=(const Rect_T<U>& rhs)
		{
			rect_ -= rhs.rect_;
			return *this;
		}
		template <typename U>
		Rect_T& operator&=(const Rect_T<U>& rhs)
		{
			this->left()	= std::max(this->left(),	rhs.left());
			this->top()		= std::max(this->top(),		rhs.top());
			this->right()	= std::min(this->right(),	rhs.right());
			this->bottom()	= std::min(this->bottom(),	rhs.bottom());
			return *this;
		}
		template <typename U>
		Rect_T& operator|=(const Rect_T<U>& rhs)
		{
			this->left()	= std::min(this->left(),	rhs.left());
			this->top()		= std::min(this->top(),		rhs.top());
			this->right()	= std::max(this->right(),	rhs.right());
			this->bottom()	= std::max(this->bottom(),	rhs.bottom());
			return *this;
		}

		Rect_T& operator=(const Rect_T& rhs)
		{
			if (this != &rhs)
			{
				rect_ = rhs.rect_;
			}
			return *this;
		}
		template <typename U>
		Rect_T& operator=(const Rect_T<U>& rhs)
		{
			if (this != &rhs)
			{
				rect_ = rhs.rect_;
			}
			return *this;
		}

		// һԪ������
		const Rect_T operator+() const
			{ return *this; }
		const Rect_T operator-() const
			{ return Rect_T<T>(-this->left(), -this->top(), -this->right(), -this->bottom()); }

		// ����
		T Width() const
			{ return this->right() - this->left(); }
		T Height() const
			{ return this->bottom() - this->top(); }
		const Size_T<T> Size() const
			{ return Size_T<T>(this->Width(), this->Height()); }
		bool IsEmpty() const
			{ return (this->left() == this->right()) && (this->top() == this->bottom()); }

	private:
		Vector_T<T, elem_num> rect_;
	};

	template <typename T>
	inline bool
	operator==(const Rect_T<T>& lhs, const Rect_T<T>& rhs)
	{
		return (lhs.left() == rhs.left()) && (lhs.top() == rhs.top())
			&& (lhs.right() == rhs.right()) && (lhs.bottom() == rhs.bottom());
	}
	template <typename T>
	inline bool
	operator!=(const Rect_T<T>& lhs, const Rect_T<T>& rhs)
	{
		return !(lhs == rhs);
	}

	typedef Rect_T<float> Rect;
}

#endif			// _RECT_HPP
