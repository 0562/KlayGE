// Math.hpp
// KlayGE ��ѧ������ ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.18)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _MATH_HPP
#define _MATH_HPP

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/MathTypes.hpp>

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	const float PI		= 3.141592f;			// PI
	const float PI2		= 6.283185f;			// PI * 2
	const float PIdiv2	= 1.570796f;			// PI / 2

	const float DEG90	= 1.570796f;			// 90 ��
	const float DEG270	= -1.570796f;			// 270 ��
	const float DEG45	= 0.7853981f;			// 45 ��
	const float DEG5	= 0.0872664f;			// 5 ��
	const float DEG10	= 0.1745329f;			// 10 ��
	const float DEG20	= 0.3490658f;			// 20 �� 
	const float DEG30	= 0.5235987f;			// 30 ��
	const float DEG60	= 1.047197f;			// 60 ��
	const float DEG120	= 2.094395f;			// 120 ��

	const float DEG40	= 0.6981317f;			// 40 ��
	const float DEG80	= 1.396263f;			// 80 ��
	const float DEG140	= 2.443460f;			// 140 ��
	const float DEG160	= 2.792526f;			// 160 ��

	const float SQRT2	= 1.414213f;			// ��2
	const float SQRT_2	= 0.7071068f;			// 1 / SQRT2
	const float SQRT3	= 1.732050f;			// ��3

	const float DEG2RAD	= 0.01745329f;			// �ǶȻ���������
	const float RAD2DEG	= 57.29577f;			// ���Ȼ��Ƕ�����

	class MathLib
	{
	public:
		static MathLib* Create(CPUOptimal cpu);

		virtual ~MathLib()
			{ }

	public:
		// �����ֵ
		template <typename T>
		T Abs(const T& x) const
			{ return x < T(0) ? -x : x; }

		// ȡ����
		template <typename T>
		T Sgn(const T& x) const
			{ return x < T(0) ? T(-1) : (x > T(0) ? T(1) : T(0)); }
		
		// ƽ��
		template <typename T>
		T Sqr(const T& x) const
			{ return x * x; }
		// ����
		template <typename T>
		T Cube(const T& x) const
			{ return this->Sqr(x) * x; }

		// �ǶȻ�����
		template <typename T>
		T Deg2Rad(const T& x) const
			{ return static_cast<T>(x * DEG2RAD); }
		// ���Ȼ��Ƕ�
		template <typename T>
		T Rad2Deg(const T& x) const
			{ return static_cast<T>(x * RAD2DEG); }

		// ��������
		template <typename T>
		T Round(const T& x) const
		{
			return (x > 0) ? static_cast<T>(static_cast<int>(0.5f + x)) :
					-static_cast<T>(static_cast<int>(0.5f - x));
		}
		// ȡ��
		template <typename T>
		T Trunc(const T& x) const
			{ return static_cast<T>(static_cast<int>(x)); }

		// ȡ������С��
		template <typename T>
		const T& Min3(const T& a, const T& b, const T& c) const
			{ return std::min(std::min(a, b), c); }
		// ȡ�����д��
		template <typename T>
		const T& Max3(const T& a, const T& b, const T& c) const
			{ return std::max(std::max(a, b), c); }

		// ����
		template <typename T>
		T Mod(const T& x, const T& y) const
			{ return x % y; }
		// ����汾
		template <>
		float Mod<float>(const float& x, const float& y) const
			{ return std::fmodf(x, y); }
		template <>
		double Mod<double>(const double& x, const double& y) const
			{ return std::fmod(x, y); }

		// ���
		template <typename InputIterator, typename T>
		T Sum(InputIterator first, InputIterator last) const
		{
			T sum(0);
			for (InputIterator i = first; i != last; ++ i)
			{
				sum += (*i);
			}
			return sum;
		}

		// ƽ����
		template <typename InputIterator, typename T>
		T Avg(InputIterator first, InputIterator last) const
		{
			return this->Sum() / (last - first);
		}

		// ��ֵ
		template <typename InputIterator, typename T>
		T Median(InputIterator first, InputIterator last) const
		{
			size_t countBelow, coundAbove;

			for (InputIterator i = first; i != last; ++ i)
			{
				countBelow = countAbove = 0;

				for (InputIterator j = first; j != last; ++ j)
				{
					if (j == i)
					{
						continue;
					}
					if (*i <= *j)
					{
						++ countAbove;
					}
					if (*i >= *j)
					{
						++ countBelow;
					}
				}

				if (this->IsOdd(last - first))
				{
					if (countBelow == countAbove)
					{
						return *i;
					}
				}
				else
				{
					if ((countBelow + 1 >= countAbove) && (countBelow <= countAbove + 1))
					{
						return *i;
					}
				}
			}

			return *first;
		}

		// ���� val �� low �� high ֮��
		template <typename T>
		const T& Limit(const T& val, const T& low, const T& high) const
		{
			if (val < low)
			{
				return low;
			}
			else
			{
				if (val > high)
				{
					return high;
				}
				else
				{
					return val;
				}
			}
		}

		// ���ƴ���
		template <typename T>
		T Surround(const T& val, const T& low, const T& high) const
		{
			T ret(val);
			T rang(high - low);

			while (ret >= high)
			{
				ret -= rang;
			}
			while (ret < low)
			{
				ret += rang;
			}

			return ret;
		}


		// ��ʼ�������������
		void Randomize() const
			{ std::srand(static_cast<unsigned int>(time(NULL))); }

		// ���������
		int Random() const
			{ return std::rand(); }

		// С��x�������
		template <typename T>
		T Random(const T& x) const
			{ return this->Mod<T>(static_cast<T>(this->Random()), x); }

		// ��min��max֮��������
		template <typename T>
		T Random(const T& minv, const T& maxv) const
			{ return minv + this->Random(maxv - minv); }


		// �����򷵻�true
		template <typename T>
		bool IsOdd(const T& x) const
			{ return this->Mod(x, 2) != 0; }
		// ż���򷵻�true
		template <typename T>
		bool IsEven(const T& x) const
			{ return !this->IsOdd(x); }

		// �ж� val �Ƿ��� low �� high ֮��
		template <typename T>
		bool InBound(const T& val, const T& low, const T& high) const
		{
			if ((val >= low) && (val <= high))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		// �ж��������Ƿ����
		template <typename T>
		bool Eq(const T& lhs, const T& rhs) const
		{
			return lhs == rhs;
		}
		// ����汾
		template <>
		bool Eq(const float& lhs, const float& rhs) const
		{
			if (this->Abs(lhs - rhs) <= std::numeric_limits<float>::epsilon())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		template <>
		bool Eq(const double& lhs, const double& rhs) const
		{
			if (this->Abs(lhs - rhs) <= std::numeric_limits<double>::epsilon())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	public:
		// ������ѧ����
		///////////////////////////////////////////////////////////////////////////////
		virtual float Abs(float x) const = 0;
		virtual float Sqrt(float x) const = 0;
		virtual float RecipSqrt(float x) const = 0;

		virtual float Pow(float x, float y) const = 0;
		virtual float Exp(float x) const = 0;

		virtual float Log(float x) const = 0;
		virtual float Log10(float x) const = 0;

		virtual float Sin(float x) const = 0;
		virtual float Cos(float x) const = 0;
		virtual void SinCos(float x, float& s, float& c) const = 0;
		virtual float Tan(float x) const = 0;

		virtual float ASin(float x) const = 0;
		virtual float ACos(float x) const = 0;
		virtual float ATan(float x) const = 0;

		virtual float Sinh(float x) const = 0;
		virtual float Cosh(float x) const = 0;
		virtual float Tanh(float x) const = 0;


	public:

		// �������͵�Dot
		template <typename T>
		typename T::value_type Dot(const T& lhs, const T& rhs) const
		{
			T::value_type ret(0);
			for (int i = 0; i < lhs.End() - lhs.Begin(); ++ i)
			{
				ret += lhs[i] * rhs[i];
			}

			return ret;
		}

		// �������͵�LengthSq
		template <typename T>
		typename T::value_type LengthSq(const T& rhs) const
		{
			T::value_type ret(0);
			for (T::const_iterator citer = rhs.Begin(); citer != rhs.End(); ++ citer)
			{
				ret += (*citer) * (*citer);
			}

			return ret;
		}

		// �������͵�Length
		template <typename T>
		float Length(const T& rhs) const
		{
			return this->Sqrt(this->LengthSq(rhs));
		}

		// �������͵�Lerp
		template <typename T>
		T& Lerp(T& out, const T& lhs, const T& rhs, float s) const
		{
			out = lhs + (rhs - lhs) * s;
			return out;
		}

		template <int N, typename T>
		Vector_T<N, T>& Maximize(Vector_T<N, T>& out, const Vector_T<N, T>& lhs, const Vector_T<N, T>& rhs) const
		{
			for (int i = 0; i < N; ++ i)
			{
				out[i] = std::max(lhs[i], rhs[i]);
			}

			return out;
		}

		template <int N, typename T>
		Vector_T<N, T>& Minimize(Vector_T<N, T>& out, const Vector_T<N, T>& lhs, const Vector_T<N, T>& rhs) const
		{
			for (int i = 0; i < N; ++ i)
			{
				out[i] = std::min(lhs[i], rhs[i]);
			}

			return out;
		}
		

		// 2D ����
		///////////////////////////////////////////////////////////////////////////////
		float CCW(const Vector2& lhs, const Vector2& rhs) const
			{ return lhs.x() * rhs.y() - lhs.y() * rhs.x(); }

		virtual Vector2& BaryCentric(Vector2& out, const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) const = 0;
		virtual Vector2& Normalize(Vector2& out, const Vector2& rhs) const = 0;
		virtual Vector4& Transform(Vector4& out, const Vector2& v, const Matrix4& mat) const = 0;
		virtual Vector2& TransformCoord(Vector2& out, const Vector2& v, const Matrix4& mat) const = 0;
		virtual Vector2& TransformNormal(Vector2& out, const Vector2& v, const Matrix4& mat) const = 0;


		// 3D ����
		///////////////////////////////////////////////////////////////////////////////
		virtual float Angle(const Vector3& lhs, const Vector3& rhs) const = 0;
		virtual Vector3& BaryCentric(Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) const = 0;
		virtual Vector3& Normalize(Vector3& out, const Vector3& rhs) const = 0;
		virtual Vector3& Cross(Vector3& out, const Vector3& lhs, const Vector3& rhs) const = 0;
		virtual Vector4& Transform(Vector4& out, const Vector3& v, const Matrix4& mat) const = 0;
		virtual Vector3& TransformCoord(Vector3& out, const Vector3& v, const Matrix4& mat) const = 0;
		virtual Vector3& TransformNormal(Vector3& out, const Vector3& v, const Matrix4& mat) const = 0;
		virtual Vector3& TransQuat(Vector3& out, const Vector3& v, const Quaternion& quat) const = 0;
		virtual Vector3& Project(Vector3& out, const Vector3& vec,
			const Matrix4& world, const Matrix4& view, const Matrix4& proj,
			const int viewport[4], float near, float far) const = 0;
		virtual Vector3& UnProject(Vector3& out, const Vector3& winVec, float clipW,
			const Matrix4& world, const Matrix4& view, const Matrix4& proj,
			const int viewport[4], float near, float far) const = 0;


		// 4D ����
		///////////////////////////////////////////////////////////////////////////////
		virtual Vector4& BaryCentric(Vector4& out, const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) const = 0;
		virtual Vector4& Cross(Vector4& out, const Vector4& v1, const Vector4& v2, const Vector4& v3) const = 0;
		virtual Vector4& Normalize(Vector4& out, const Vector4& rhs) const = 0;
		virtual Vector4& Transform(Vector4& out, const Vector4& v, const Matrix4& mat) const = 0;


		// 4D ����
		///////////////////////////////////////////////////////////////////////////////
		virtual Matrix4& Multiply(Matrix4& out, const Matrix4& lhs, const Matrix4& rhs) const = 0;
		virtual float Determinant(const Matrix4& m) const = 0;
		virtual float Inverse(Matrix4& out, const Matrix4& m) const = 0;
		virtual Matrix4& LookAtLH(Matrix4& out, const Vector3& vEye, const Vector3& vAt, const Vector3& vUp) const = 0;
		virtual Matrix4& LookAtRH(Matrix4& out, const Vector3& vEye, const Vector3& vAt, const Vector3& vUp) const = 0;
		virtual Matrix4& OrthoLH(Matrix4& out, float w, float h, float fNear, float fFar) const = 0;
		virtual Matrix4& OrthoOffCenterLH(Matrix4& out, float l, float r, float b, float t, float fNear, float fFar) const = 0;
		virtual Matrix4& PerspectiveLH(Matrix4& out, float w, float h, float fNear, float fFar) const = 0;
		virtual Matrix4& PerspectiveFovLH(Matrix4& out, float fFOV, float fAspect, float fNear, float fFar) const = 0;
		virtual Matrix4& PerspectiveOffCenterLH(Matrix4& out, float l, float r, float b, float t,
			float fNear, float fFar) const = 0;
		virtual Matrix4& Reflect(Matrix4& out, const Plane& p) const = 0;
		virtual Matrix4& RotationX(Matrix4& out, float x) const = 0;
		virtual Matrix4& RotationY(Matrix4& out, float y) const = 0;
		virtual Matrix4& RotationZ(Matrix4& out, float z) const = 0;
		virtual Matrix4& Rotation(Matrix4& out, float angle, float x, float y, float z) const = 0;
		virtual Matrix4& Scaling(Matrix4& out, float x, float y, float z) const = 0;
		virtual Matrix4& Shadow(Matrix4& out, const Vector4& v, const Plane& p) const = 0;
		virtual Matrix4& ToMatrix(Matrix4& out, const Quaternion& quat) const = 0;
		virtual Matrix4& Translation(Matrix4& out, float x, float y, float z) const = 0;
		virtual Matrix4& Transpose(Matrix4& out, const Matrix4& m) const = 0;
		virtual Matrix4& AffineTransformation(Matrix4& out, float f, const Vector3& vRotationCenter = MakeVector(0.0f, 0.0f, 0.0f),
			const Quaternion& qRotation = Quaternion(0, 0, 0, 1), const Vector3& vTranslation = MakeVector(0.0f, 0.0f, 0.0f)) const = 0;

		virtual Matrix4& LHToRH(Matrix4& out, const Matrix4& rhs) const = 0;

		virtual Matrix4& Scaling(Matrix4& out, const Vector3& vPos) const;
		virtual Matrix4& Translation(Matrix4& out, const Vector3& vPos) const;
		virtual Matrix4& LookAtLH(Matrix4& out, const Vector3& vEye, const Vector3& vAt) const;
		virtual Matrix4& LookAtRH(Matrix4& out, const Vector3& vEye, const Vector3& vAt) const;
		virtual Matrix4& OrthoRH(Matrix4& out, float w, float h, float fNear, float fFar) const;
		virtual Matrix4& OrthoOffCenterRH(Matrix4& out, float l, float r, float b, float t, 
			float fNear, float fFar) const;
		virtual Matrix4& PerspectiveRH(Matrix4& out, float w, float h, float fNear, float fFar) const;
		virtual Matrix4& PerspectiveFovRH(Matrix4& out, float fFOV, float fAspect, float fNear, float fFar) const;
		virtual Matrix4& PerspectiveOffCenterRH(Matrix4& out, float l, float r, float b, float t, 
			float fNear, float fFar) const;

		virtual Matrix4& RHToLH(Matrix4& out, const Matrix4& rhs) const;


		// ��Ԫ��
		///////////////////////////////////////////////////////////////////////////////
		Quaternion& Conjugate(Quaternion& out, const Quaternion& rhs) const
		{
			out = Quaternion(-rhs.x(), -rhs.y(), -rhs.z(), rhs.w());
			return out;
		}

		virtual Quaternion& AxisToAxis(Quaternion& out, const Vector3& vFrom, const Vector3& vTo) const = 0;
		virtual Quaternion& BaryCentric(Quaternion& out, const Quaternion& q1, const Quaternion& q2,
			const Quaternion& q3, float f, float g) const = 0;
		virtual Quaternion& Exp(Quaternion& out, const Quaternion& rhs) const = 0;
		virtual Quaternion& Inverse(Quaternion& out, const Quaternion& q) const = 0;
		virtual Quaternion& Normalize(Quaternion& out, const Quaternion& q) const = 0;
		virtual Quaternion& Ln(Quaternion& out, const Quaternion& rhs) const = 0;
		virtual Quaternion& Multiply(Quaternion& out, const Quaternion& lhs, const Quaternion& rhs) const = 0;
		virtual Quaternion& RotationYawPitchRoll(Quaternion& out, float fYaw, float fPitch, float fRoll) const = 0;
		virtual void ToAxisAngle(Vector3& vec, float& ang, const Quaternion& quat) const = 0;
		virtual Quaternion& ToQuaternion(Quaternion& out, const Matrix4& m) const = 0;
		virtual Quaternion& RotationAxis(Quaternion& out, const Vector3& v, float s) const = 0;
		virtual Quaternion& Slerp(Quaternion& out, const Quaternion& lhs, const Quaternion& rhs, float s) const = 0;
		virtual Quaternion& UnitAxisToUnitAxis2(Quaternion& out, const Vector3& vFrom, const Vector3& vTo) const = 0;

		virtual Quaternion& RotationYawPitchRoll(Quaternion& out, const Vector3& vAng) const;


		// ƽ��
		///////////////////////////////////////////////////////////////////////////////
		float Dot(const Plane& lhs, const Vector4& rhs) const
			{ return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.c() * rhs.z() + lhs.d() * rhs.w(); }

		float DotCoord(const Plane& lhs, const Vector3& rhs) const
			{ return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.c() * rhs.z() + lhs.d(); }

		float DotNormal(const Plane& lhs, const Vector3& rhs) const
			{ return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.c() * rhs.z(); }

		virtual Plane& Normalize(Plane& out, const Plane& p) const = 0;
		virtual Plane& FromPointNormal(Plane& out, const Vector3& vPoint, const Vector3& vNor) const = 0;
		virtual Plane& FromPoints(Plane& out, const Vector3& v1, const Vector3& v2, const Vector3& v3) const = 0;
		virtual Plane& Transform(Plane& out, const Plane& p, const Matrix4& mat) const = 0;
		virtual bool IntersectLine(Vector3& out, const Plane& p, const Vector3& vStart, const Vector3& vEnd) const = 0;


		// ��ɫ
		///////////////////////////////////////////////////////////////////////////////
		Color& Negative(Color& out, const Color& rhs) const
		{
			out = Color(1 - rhs.r(), 1 - rhs.g(), 1 - rhs.b(), rhs.a());
			return out;
		}

		Color& Modulate(Color& out, const Color& lhs, const Color& rhs) const
		{
			out = Color(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b(), lhs.a() * rhs.a());
			return out;
		}

		// ��Χ
		///////////////////////////////////////////////////////////////////////////////
		virtual bool VecInBox(const Box& box, const Vector3& v) const = 0;
		virtual bool BoundProbe(const Box& box, const Vector3& vPos, const Vector3& vDir) const = 0;
	};
}

#endif		// _MATH_HPP
