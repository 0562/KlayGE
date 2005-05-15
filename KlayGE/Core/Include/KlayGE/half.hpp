// half.hpp
// KlayGE �뾫�ȸ�������ͷ�ļ�
// Ver 2.5.1
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.1
// ���ν��� (2005.5.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

namespace KlayGE
{
	// 1s5e10m
	class half : boost::addable<half,
						boost::subtractable<half,
						boost::multipliable<half,
						boost::dividable<half> > > >
	{
	public:
		half()
		{
		}
		explicit half(float f)
		{
			uint32_t u = *reinterpret_cast<boost::uint32_t*>(&f);

			value_ = (u & 0x80000000) << 15;
			value_ |= ((((u & 0x7F800000) >> 23) - 127 + 15) & 0x1F) << 10;
			value_ |= (u & 0x007FFFFF) >> 13;
		}

		operator float() const
		{
			boost::uint32_t ret;
			
			ret = (value_ & 0x8000) << 31;
			ret |= (((value_ & 0x7C00) >> 10) - 15 + 127) << 23;
			ret |= (value_ & 0x03FF) << 13;

			return *reinterpret_cast<float*>(&ret);
		}

		// ��ֵ������
		half const & operator+=(half const & rhs)
		{
			*this = half(float(*this) + float(rhs));
			return *this;
		}
		half const & operator-=(half const & rhs)
		{
			*this = half(float(*this) - float(rhs));
			return *this;
		}
		half const & operator*=(half const & rhs)
		{
			*this = half(float(*this) * float(rhs));
			return *this;
		}
		half const & operator/=(half const & rhs)
		{
			*this = half(float(*this) / float(rhs));
			return *this;
		}

		half& operator=(half const & rhs)
		{
			if (this != &rhs)
			{
				value_ = rhs.value_;
			}
			return *this;
		}

		// һԪ������
		half const operator+() const
			{ return *this; }
		half const operator-() const
		{
			half temp(*this);
			temp.value_ = -temp.value_;
			return temp;
		}

		friend bool
		operator==(half const & lhs, half const & rhs)
		{
			return lhs.value_ == rhs.value_;
		}

	private:
		int16_t value_;
	};

	inline bool
	operator!=(half const & lhs, half const & rhs)
	{
		return !(lhs == rhs);
	}
}
