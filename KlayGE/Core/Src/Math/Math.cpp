// Math.hpp
// KlayGE ��ѧ������ ʵ���ļ�
// Ver 2.1.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.0
// ȥ���˻����� (2004.4.18)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <cstdlib>

#include <KlayGE/Math.hpp>

namespace KlayGE
{
	namespace MathLib
	{
		float Abs(float x)
		{
			return std::fabsf(x);
		}

		float Sqrt(float x)
		{
			return std::sqrtf(x);
		}

		float RecipSqrt(float x)
		{
			return 1.0f / Sqrt(x);
		}

		float Pow(float x, float y)
		{
			return std::powf(x, y);
		}

		float Exp(float x)
		{
			return std::expf(x);
		}

		float Log(float x)
		{
			return std::logf(x);
		}

		float Log10(float x)
		{
			return std::log10f(x);
		}

		float Sin(float x)
		{
			return std::sinf(x);
		}

		float Cos(float x)
		{
			return std::cosf(x);
		}

		void SinCos(float x, float& s, float& c)
		{
			s = Sin(x);
			c = Cos(x);
		}

		float Tan(float x)
		{
			return std::tanf(x);
		}

		float ASin(float x)
		{
			return std::asinf(x);
		}

		float ACos(float x)
		{
			return std::acosf(x);
		}

		float ATan(float x)
		{
			return std::atanf(x);
		}

		float Sinh(float x)
		{
			return std::sinhf(x);
		}

		float Cosh(float x)
		{
			return std::coshf(x);
		}

		float Tanh(float x)
		{
			return std::tanhf(x);
		}


		// ��ɫ
		///////////////////////////////////////////////////////////////////////////////
		Color& Negative(Color& out, const Color& rhs)
		{
			out = Color(1 - rhs.r(), 1 - rhs.g(), 1 - rhs.b(), rhs.a());
			return out;
		}

		Color& Modulate(Color& out, const Color& lhs, const Color& rhs)
		{
			out = Color(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b(), lhs.a() * rhs.a());
			return out;
		}


		// ��Χ
		///////////////////////////////////////////////////////////////////////////////
		bool VecInSphere(const Sphere& sphere, const Vector3& v)
		{
			if (Length(v - sphere.Center()) < sphere.Radius())
			{
				return true;
			}
			return false;
		}

		bool BoundProbe(const Sphere& sphere, const Vector3& orig, const Vector3& dir)
		{
			const float a = LengthSq(dir);
			const float b = 2 * Dot(dir, orig - sphere.Center());
			const float c = LengthSq(orig - sphere.Center()) - sphere.Radius() * sphere.Radius();

			if (b * b - 4 * a * c < 0)
			{
				return false;
			}
			return true;
		}

		bool VecInBox(const Box& box, const Vector3& v)
		{
			return (InBound(v.x(), box.Min().x(), box.Max().x()))
				&& (InBound(v.y(), box.Min().y(), box.Max().y()))
				&& (InBound(v.z(), box.Min().z(), box.Max().z()));
		}

		bool BoundProbe(const Box& box, const Vector3& orig, const Vector3& dir)
		{
			const Vector3 leftBottomNear(box.LeftBottomNear());
			const Vector3 leftTopNear(box.LeftTopNear());
			const Vector3 rightTopNear(box.RightTopNear());
			const Vector3 leftTopFar(box.LeftTopFar());

			Plane pNear;
			FromPoints(pNear, leftBottomNear, leftTopNear, rightTopNear);
			Plane pTop;
			FromPoints(pTop, leftTopNear, leftTopFar, rightTopNear);
			Plane pLeft;
			FromPoints(pLeft, leftTopFar, leftTopNear, leftBottomNear);

			Vector3 vec;
			if (IntersectLine(vec, pNear, orig, dir))
			{
				if ((!InBound(vec.x(), leftBottomNear.x(), rightTopNear.x()))
					|| (!InBound(vec.y(), leftBottomNear.y(), leftTopNear.y())))
				{
					return false;
				}
			}

			if (IntersectLine(vec, pTop, orig, dir))
			{
				if ((!InBound(vec.x(), leftTopNear.x(), rightTopNear.x()))
					|| (!InBound(vec.z(), leftTopNear.z(), leftTopFar.z())))
				{
					return false;
				}
			}

			if (IntersectLine(vec, pLeft, orig, dir))
			{
				if ((!InBound(vec.y(), leftBottomNear.y(), leftTopNear.y()))
					|| (!InBound(vec.z(), leftBottomNear.z(), leftTopFar.z())))
				{
					return false;
				}
			}

			return true;
		}
	}
}
