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
		Box(const Vector3& vMin, const Vector3& vMax)
		{
			MathLib::Minimize(min_, vMin, vMax);
			MathLib::Maximize(max_, vMin, vMax);
		}

		// ��ֵ������
		Box& operator+=(const Vector3& rhs)
		{
			min_ += rhs;
			max_ += rhs;
			return *this;
		}
		Box& operator-=(const Vector3& rhs)
		{
			min_ -= rhs;
			max_ -= rhs;
			return *this;
		}
		Box& operator*=(float rhs)
		{
			this->Min() *= rhs;
			this->Max() *= rhs;
			return *this;
		}
		Box& operator/=(float rhs)
		{
			return operator*=(1.0f / rhs);
		}
		Box& operator&=(const Box& rhs)
		{
			MathLib::Maximize(this->Min(), this->Min(), rhs.Min());
			MathLib::Minimize(this->Max(), this->Max(), rhs.Max());
			return *this;
		}
		Box& operator|=(const Box& rhs)
		{
			MathLib::Minimize(this->Min(), this->Min(), rhs.Min());
			MathLib::Maximize(this->Max(), this->Max(), rhs.Max());
			return *this;
		}

		Box& operator=(const Box& rhs)
		{
			if (this != &rhs)
			{
				this->Min() = rhs.Min();
				this->Max() = rhs.Max();
			}
			return *this;
		}

		// һԪ������
		const Box operator+() const
		{
			return *this;
		}
		const Box operator-() const
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

		const Vector3 LeftBottomNear() const
		{
			return this->Min();
		}
		const Vector3 LeftTopNear() const
		{
			return Vector3(this->Min().x(), this->Max().y(), this->Min().z());
		}
		const Vector3 RightBottomNear() const
		{
			return Vector3(this->Max().x(), this->Min().y(), this->Min().z());
		}
		const Vector3 RightTopNear() const
		{
			return Vector3(this->Max().x(), this->Max().y(), this->Min().z());
		}
		const Vector3 LeftBottomFar() const
		{
			return Vector3(this->Min().x(), this->Min().y(), this->Max().z());
		}
		const Vector3 LeftTopFar() const
		{
			return Vector3(this->Min().x(), this->Max().y(), this->Max().z());
		}
		const Vector3 RightBottomFar() const
		{
			return Vector3(this->Max().x(), this->Min().y(), this->Max().z());
		}
		const Vector3 RightTopFar() const
		{
			return this->Max();
		}

		Vector3& Min()
		{
			return min_;
		}
		const Vector3& Min() const
		{
			return min_;
		}
		Vector3& Max()
		{
			return max_;
		}
		const Vector3& Max() const
		{
			return max_;
		}

		bool VecInBound(const Vector3& v) const
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
	operator==(const Box& lhs, const Box& rhs)
	{
		return (lhs.Min() == rhs.Min()) && (lhs.Max() == rhs.Max());
	}
	inline bool
	operator!=(const Box& lhs, const Box& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif			// _BOX_HPP
