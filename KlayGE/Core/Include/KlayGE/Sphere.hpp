// Sphere.hpp
// KlayGE �߿����� ͷ�ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
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

namespace KlayGE
{
	class Sphere : boost::addable2<Sphere, Vector3, 
						boost::subtractable2<Sphere, Vector3,
						boost::andable<Sphere,
						boost::orable<Sphere > > > >,
				public Bound
	{
	public:
		Sphere()
		{
		}
		Sphere(Vector3 const & center, float radius)
			: center_(center),
				radius_(radius)
		{
		}

		// ��ֵ������
		Sphere const & operator+=(Vector3 const & rhs)
		{
			this->Center() += rhs;
			return *this;
		}
		Sphere const & operator-=(Vector3 const & rhs)
		{
			this->Center() -= rhs;
			return *this;
		}
		Sphere const & operator*=(float rhs)
		{
			this->Radius() *= rhs;
			return *this;
		}
		Sphere const & operator/=(float rhs)
		{
			return this->operator*=(1.0f / rhs);
		}

		Sphere const & operator=(Sphere const & rhs)
		{
			if (this != &rhs)
			{
				this->Center() = rhs.Center();
				this->Radius() = rhs.Radius();
			}
			return *this;
		}

		// һԪ������
		Sphere const & operator+() const
		{
			return *this;
		}
		Sphere const & operator-() const
		{
			return *this;
		}

		// ����
		Vector3& Center()
		{
			return center_;
		}
		Vector3 const & Center() const
		{
			return center_;
		}
		float& Radius()
		{
			return radius_;
		}
		float Radius() const
		{
			return radius_;
		}

		bool IsEmpty() const
		{
			return MathLib::Eq(radius_, 0.0f);
		}

		bool VecInBound(Vector3 const & v) const
		{
			return MathLib::VecInSphere(*this, v);
		}
		float MaxRadiusSq() const
		{
			return this->Radius() * this->Radius();
		}

	private:
		Vector3 center_;
		float radius_;
	};

	inline bool
	operator==(Sphere const & lhs, Sphere const & rhs)
	{
		return (lhs.Center() == rhs.Center()) && (lhs.Radius() == rhs.Radius());
	}
	inline bool
	operator!=(Sphere const & lhs, Sphere const & rhs)
	{
		return !(lhs == rhs);
	}
}

#endif			// _SPHERE_HPP
