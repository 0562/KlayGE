// Sphere.hpp
// KlayGE �߿����� ͷ�ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://www.klayge.org
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

#include <KlayGE/PreDeclare.hpp>

#include <boost/assert.hpp>
#include <boost/operators.hpp>

#include <KlayGE/Bound.hpp>

#pragma once

namespace KlayGE
{
	template <typename T>
	class Sphere_T : boost::addable2<Sphere_T<T>, Vector_T<T, 3>,
						boost::subtractable2<Sphere_T<T>, Vector_T<T, 3>,
						boost::multipliable2<Sphere_T<T>, T,
						boost::dividable2<Sphere_T<T>, T,
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
			return MathLib::intersect_point_sphere(v, *this);
		}
		T MaxRadiusSq() const
		{
			return this->Radius() * this->Radius();
		}

		bool Intersect(AABBox_T<T> const & aabb) const
		{
			return aabb.Intersect(*this);
		}
		bool Intersect(OBBox_T<T> const & obb) const
		{
			return obb.Intersect(*this);
		}
		bool Intersect(Sphere_T<T> const & sphere) const
		{
			return intersect_sphere_sphere(*this, sphere);
		}
		bool Intersect(Frustum_T<T> const & frustum) const
		{
			return intersect_sphere_frustum(*this, frustum) != BO_No;
		}

		friend bool
		operator==(Sphere_T<T> const & lhs, Sphere_T<T> const & rhs)
		{
			return (lhs.center_ == rhs.center_) && (rhs.radius_ == rhs.radius_);
		}

	private:
		Vector_T<T, 3> center_;
		T radius_;
	};

	typedef Sphere_T<float> Sphere;
}

#endif			// _SPHERE_HPP
