// Box.hpp
// KlayGE �߿�� ͷ�ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.1
// ���ν��� (2004.4.22)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _BOX_HPP
#define _BOX_HPP

#include <boost/operators.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Bound.hpp>

namespace KlayGE
{
	class Box : boost::addable2<Box, Vector3, 
						boost::subtractable2<Box, Vector3,
						boost::andable<Box,
						boost::orable<Box > > > >,
				public Bound
	{
	public:
		Box()
		{
		}
		Box(Vector3 const & vMin, Vector3 const & vMax)
		{
			MathLib::Minimize(min_, vMin, vMax);
			MathLib::Maximize(max_, vMin, vMax);
		}

		// ��ֵ������
		Box const & operator+=(Vector3 const & rhs)
		{
			min_ += rhs;
			max_ += rhs;
			return *this;
		}
		Box const & operator-=(Vector3 const & rhs)
		{
			min_ -= rhs;
			max_ -= rhs;
			return *this;
		}
		Box const & operator*=(float rhs)
		{
			this->Min() *= rhs;
			this->Max() *= rhs;
			return *this;
		}
		Box const & operator/=(float rhs)
		{
			return this->operator*=(1.0f / rhs);
		}
		Box const & operator&=(Box const & rhs)
		{
			MathLib::Maximize(this->Min(), this->Min(), rhs.Min());
			MathLib::Minimize(this->Max(), this->Max(), rhs.Max());
			return *this;
		}
		Box& operator|=(Box const & rhs)
		{
			MathLib::Minimize(this->Min(), this->Min(), rhs.Min());
			MathLib::Maximize(this->Max(), this->Max(), rhs.Max());
			return *this;
		}

		Box& operator=(Box const & rhs)
		{
			if (this != &rhs)
			{
				this->Min() = rhs.Min();
				this->Max() = rhs.Max();
			}
			return *this;
		}

		// һԪ������
		Box const operator+() const
		{
			return *this;
		}
		Box const operator-() const
		{
			return Box(-this->Min(), -this->Max());
		}

		// ����
		float Width() const
		{
			return this->Max().x() - this->Min().x();
		}
		float Height() const
		{
			return this->Max().y() - this->Min().y();
		}
		float Depth() const
		{
			return this->Max().z() - this->Min().z();
		}
		bool IsEmpty() const
		{
			return this->Min() == this->Max();
		}

		Vector3 const LeftBottomNear() const
		{
			return this->Min();
		}
		Vector3 const LeftTopNear() const
		{
			return Vector3(this->Min().x(), this->Max().y(), this->Min().z());
		}
		Vector3 const RightBottomNear() const
		{
			return Vector3(this->Max().x(), this->Min().y(), this->Min().z());
		}
		Vector3 const RightTopNear() const
		{
			return Vector3(this->Max().x(), this->Max().y(), this->Min().z());
		}
		Vector3 const LeftBottomFar() const
		{
			return Vector3(this->Min().x(), this->Min().y(), this->Max().z());
		}
		Vector3 const LeftTopFar() const
		{
			return Vector3(this->Min().x(), this->Max().y(), this->Max().z());
		}
		Vector3 const RightBottomFar() const
		{
			return Vector3(this->Max().x(), this->Min().y(), this->Max().z());
		}
		Vector3 const RightTopFar() const
		{
			return this->Max();
		}

		Vector3& Min()
		{
			return min_;
		}
		Vector3 const & Min() const
		{
			return min_;
		}
		Vector3& Max()
		{
			return max_;
		}
		Vector3 const & Max() const
		{
			return max_;
		}

		bool VecInBound(Vector3 const & v) const
		{
			return MathLib::VecInBox(*this, v);
		}
		float MaxRadiusSq() const
		{
			return std::max(MathLib::LengthSq(this->Max()), MathLib::LengthSq(this->Min()));
		}

	private:
		Vector3 min_, max_;
	};

	inline bool
	operator==(Box const & lhs, Box const & rhs)
	{
		return (lhs.Min() == rhs.Min()) && (lhs.Max() == rhs.Max());
	}
	inline bool
	operator!=(Box const & lhs, Box const & rhs)
	{
		return !(lhs == rhs);
	}
}

#endif			// _BOX_HPP
