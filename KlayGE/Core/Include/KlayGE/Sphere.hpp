// Sphere.hpp
// KlayGE �߿����� ͷ�ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ��Ϊģ�� (2005.4.12)
//
// 2.1.1
// ���ν��� (2004.4.30)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _SPHERE_HPP
#define _SPHERE_HPP

#include <boost/operators.hpp>

#include <KlayGE/Bound.hpp>

#pragma once

namespace KlayGE
{
	template <typename T>
	class Sphere_T : boost::addable2<Sphere_T<T>, Vector_T<T, 3>,
						boost::subtractable2<Sphere_T<T>, Vector_T<T, 3>,
						boost::andable<Sphere_T<T>,
						boost::orable<Sphere_T<T>,
						boost::equality_comparable<Sphere_T<T> > > > > >,
				public Bound_T<T>
	{
	public:
		Sphere_T()
		{
		}
		Sphere_T(Vector_T<T, 3> const & center, T const & radius)
			: center_(center),
				radius_(radius)
		{
		}

		// ��ֵ������
		Sphere_T& operator+=(Vector_T<T, 3> const & rhs)
		{
			this->Center() += rhs;
			return *this;
		}
		Sphere_T& operator-=(Vector_T<T, 3> const & rhs)
		{
			this->Center() -= rhs;
			return *this;
		}
		Sphere_T& operator*=(T const & rhs)
		{
			this->Radius() *= rhs;
			return *this;
		}
		Sphere_T& operator/=(T const & rhs)
		{
			return this->operator*=(1.0f / rhs);
		}

		Sphere_T& operator=(Sphere_T const & rhs)
		{
			if (this != &rhs)
			{
				this->Center() = rhs.Center();
				this->Radius() = rhs.Radius();
			}
			return *this;
		}

		// һԪ������
		Sphere_T const & operator+() const
		{
			return *this;
		}
		Sphere_T const & operator-() const
		{
			return *this;
		}

		// ����
		Vector_T<T, 3>& Center()
		{
			return center_;
		}
		Vector_T<T, 3> const & Center() const
		{
			return center_;
		}
		T& Radius()
		{
			return radius_;
		}
		T Radius() const
		{
			return radius_;
		}

		bool IsEmpty() const
		{
			return MathLib::equal(radius_, 0.0f);
		}

		bool VecInBound(Vector_T<T, 3> const & v) const
		{
			return MathLib::vec_in_sphere(*this, v);
		}
		T MaxRadiusSq() const
		{
			return this->Radius() * this->Radius();
		}

		bool operator==(Sphere_T const & rhs)
		{
			return (center_ == rhs.center_) && (radius_ == rhs.radius_);
		}

	private:
		Vector_T<T, 3> center_;
		T radius_;
	};

	typedef Sphere_T<float> Sphere;
}

#endif			// _SPHERE_HPP
