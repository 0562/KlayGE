// Math.hpp
// KlayGE ��ѧ������ ͷ�ļ�
// Ver 2.6.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.6.0
// �Ľ���ComputeBoundingSphere (2005.5.23)
//
// 2.5.0
// ��Ϊͨ������ֵ���ؽ�� (2005.4.12)
//
// 2.1.1
// �޸����Զ������� (2004.4.22)
// ������������ (2004.5.18)
//
// 2.0.4
// �޸���Random�Ľӿ� (2004.3.29)
//
// 2.0.0
// ���ν��� (2003.9.18)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _MATH_HPP
#define _MATH_HPP

#include <KlayGE/PreDeclare.hpp>

#include <limits>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iterator>

#include <boost/static_assert.hpp>

#include <KlayGE/Detail/MathHelper.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	float const PI		= 3.141592f;			// PI
	float const PI2		= 6.283185f;			// PI * 2
	float const PIdiv2	= 1.570796f;			// PI / 2

	float const DEG90	= 1.570796f;			// 90 ��
	float const DEG270	= -1.570796f;			// 270 ��
	float const DEG45	= 0.7853981f;			// 45 ��
	float const DEG5	= 0.0872664f;			// 5 ��
	float const DEG10	= 0.1745329f;			// 10 ��
	float const DEG20	= 0.3490658f;			// 20 ��
	float const DEG30	= 0.5235987f;			// 30 ��
	float const DEG60	= 1.047197f;			// 60 ��
	float const DEG120	= 2.094395f;			// 120 ��

	float const DEG40	= 0.6981317f;			// 40 ��
	float const DEG80	= 1.396263f;			// 80 ��
	float const DEG140	= 2.443460f;			// 140 ��
	float const DEG160	= 2.792526f;			// 160 ��

	float const SQRT2	= 1.414213f;			// ��2
	float const SQRT_2	= 0.7071068f;			// 1 / SQRT2
	float const SQRT3	= 1.732050f;			// ��3

	float const DEG2RAD	= 0.01745329f;			// �ǶȻ���������
	float const RAD2DEG	= 57.29577f;			// ���Ȼ��Ƕ�����

	namespace MathLib
	{
		// �����ֵ
		template <typename T>
		inline T
		abs(T const & x)
		{
			return x < T(0) ? -x : x;
		}

		// ȡ����
		template <typename T>
		inline T
		sgn(T const & x)
		{
			return x < T(0) ? T(-1) : (x > T(0) ? T(1) : T(0));
		}

		// ƽ��
		template <typename T>
		inline T
		sqr(T const & x)
		{
			return x * x;
		}
		// ����
		template <typename T>
		inline T
		cube(T const & x)
		{
			return sqr(x) * x;
		}

		// �ǶȻ�����
		template <typename T>
		inline T
		deg2rad(T const & x)
		{
			return static_cast<T>(x * DEG2RAD);
		}
		// ���Ȼ��Ƕ�
		template <typename T>
		inline T
		rad2deg(T const & x)
		{
			return static_cast<T>(x * RAD2DEG);
		}

		// ȡС�ڵ���x���������
		template <typename T>
		inline T
		floor(T const & x)
		{
			return static_cast<T>(static_cast<int>(x));
		}

		// ȡx��С������
		template <typename T>
		inline T
		frac(T const & x)
		{
			return x - floor(x);
		}

		// ��������
		template <typename T>
		inline T
		round(T const & x)
		{
			return (x > 0) ? static_cast<T>(static_cast<int>(T(0.5) + x)) :
					-static_cast<T>(static_cast<int>(T(0.5) - x));
		}
		// ȡ��
		template <typename T>
		inline T
		trunc(T const & x)
		{
			return static_cast<T>(static_cast<int>(x));
		}

		// ȡ������С��
		template <typename T>
		inline T const &
		min3(T const & a, T const & b, T const & c)
		{
			return std::min(std::min(a, b), c);
		}
		// ȡ�����д��
		template <typename T>
		inline T const &
		max3(T const & a, T const & b, T const & c)
		{
			return std::max(std::max(a, b), c);
		}

		// ����
		template <typename T>
		inline T
		mod(T const & x, T const & y)
		{
			return x % y;
		}
		// ����汾
		template<>
		inline float
		mod<float>(float const & x, float const & y)
		{
			return std::fmod(x, y);
		}
		template <>
		inline double
		mod<double>(double const & x, double const & y)
		{
			return std::fmod(x, y);
		}

		// ���� val �� low �� high ֮��
		template <typename T>
		inline T const &
		clamp(T const & val, T const & low, T const & high)
		{
			return std::max(low, std::min(high, val));
		}

		// ���ƴ���
		template <typename T>
		inline T
		wrap(T const & val, T const & low, T const & high)
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

		// ������
		template <typename T>
		inline T
		mirror(T const & val, T const & low, T const & high)
		{
			T ret(val);
			T rang(high - low);

			while ((ret > high) || (ret < low))
			{
				if (ret > high)
				{
					ret = 2 * high - val;
				}
				else
				{
					if (ret < low)
					{
						ret = 2 * low - val;
					}
				}
			}

			return ret;
		}

		// �����򷵻�true
		template <typename T>
		inline bool
		is_odd(T const & x)
		{
			return mod(x, 2) != 0;
		}
		// ż���򷵻�true
		template <typename T>
		inline bool
		is_even(T const & x)
		{
			return !is_odd(x);
		}

		// �ж� val �Ƿ��� low �� high ֮��
		template <typename T>
		inline bool
		in_bound(T const & val, T const & low, T const & high)
		{
			return ((val >= low) && (val <= high));
		}

		// �ж��������Ƿ����
		template <typename T>
		inline bool
		equal(T const & lhs, T const & rhs)
		{
			return (lhs == rhs);
		}
		// ����汾
		template <>
		inline bool
		equal<float>(float const & lhs, float const & rhs)
		{
			return (abs<float>(lhs - rhs)
				<= std::numeric_limits<float>::epsilon());
		}
		template <>
		inline bool
		equal<double>(double const & lhs, double const & rhs)
		{
			return (abs<double>(lhs - rhs)
				<= std::numeric_limits<double>::epsilon());
		}


		// ������ѧ����
		///////////////////////////////////////////////////////////////////////////////
		inline float
		abs(float x)
		{
			return std::abs(x);
		}
		inline float
		sqrt(float x)
		{
			return std::sqrt(x);
		}
		inline float
		recip_sqrt(float x)
		{
			return 1.0f / sqrt(x);
		}

		inline float
		pow(float x, float y)
		{
			return std::pow(x, y);
		}
		inline float
		exp(float x)
		{
			return std::exp(x);
		}

		inline float
		log(float x)
		{
			return std::log(x);
		}
		inline float
		log10(float x)
		{
			return std::log10(x);
		}

		inline float
		sin(float x)
		{
			return std::sin(x);
		}
		inline float
		cos(float x)
		{
			return sin(x + PI / 2);
		}
		inline void
		sin_cos(float x, float& s, float& c)
		{
			s = sin(x);
			c = cos(x);
		}
		inline float
		tan(float x)
		{
			return std::tan(x);
		}

		inline float
		asin(float x)
		{
			return std::asin(x);
		}
		inline float
		acos(float x)
		{
			return std::acos(x);
		}
		inline float
		atan(float x)
		{
			return std::atan(x);
		}

		inline float
		sinh(float x)
		{
			return std::sinh(x);
		}
		inline float
		cosh(float x)
		{
			return std::cosh(x);
		}
		inline float
		tanh(float x)
		{
			return std::tanh(x);
		}


		// �������͵�Dot
		template <typename T>
		inline typename T::value_type
		dot(T const & lhs, T const & rhs)
		{
			return detail::dot_helper<typename T::value_type,
							T::elem_num>::Do(&lhs[0], &rhs[0]);
		}

		// Length��ƽ��
		template <typename T>
		inline typename T::value_type
		length_sq(T const & rhs)
		{
			return dot(rhs, rhs);
		}

		// �������͵�Length
		template <typename T>
		inline typename T::value_type
		length(T const & rhs)
		{
			return sqrt(length_sq(rhs));
		}

		// �������͵�Lerp
		template <typename T>
		inline T
		lerp(T const & lhs, T const & rhs, float s)
		{
			return lhs + (rhs - lhs) * s;
		}

		template <typename T, int N>
		inline Vector_T<T, N>
		maximize(Vector_T<T, N> const & lhs, Vector_T<T, N> const & rhs)
		{
			Vector_T<T, N> ret;
			detail::max_minimize_helper<T, N>::DoMax(&ret[0], &lhs[0], &rhs[0]);
			return ret;
		}

		template <typename T, int N>
		inline Vector_T<T, N>
		minimize(Vector_T<T, N> const & lhs, Vector_T<T, N> const & rhs)
		{
			Vector_T<T, N> ret;
			detail::max_minimize_helper<T, N>::DoMin(&ret[0], &lhs[0], &rhs[0]);
			return ret;
		}

		template <typename T, int N>
		inline Vector_T<T, 4>
		transform(Vector_T<T, N> const & v, Matrix4_T<T> const & mat)
		{
			return detail::transform_helper<T, N>::Do(v, mat);
		}

		template <typename T, int N>
		inline Vector_T<T, N>
		transform_coord(Vector_T<T, N> const & v, Matrix4_T<T> const & mat)
		{
			BOOST_STATIC_ASSERT(N < 4);

			Vector_T<T, 4> temp(detail::transform_helper<T, N>::Do(v, mat));
			Vector_T<T, N> ret = Vector_T<T, N>(&temp[0]);
			if (equal(temp.w(), T(0)))
			{
				ret = Vector_T<T, N>::Zero();
			}
			else
			{
				ret /= temp.w();
			}
			return ret;
		}

		template <typename T, int N>
		inline Vector_T<T, N>
		transform_normal(Vector_T<T, N> const & v, Matrix4_T<T> const & mat)
		{
			BOOST_STATIC_ASSERT(N < 4);

			return detail::transform_normal_helper<T, N>::Do(v, mat);
		}

		template <typename T, int N>
		inline Vector_T<T, N>
		bary_centric(Vector_T<T, N> const & v1, Vector_T<T, N> const & v2, Vector_T<T, N> const & v3,
			T const & f, T const & g)
		{
			return (1 - f - g) * v1 + f * v2 + g * v3;
		}

		template <typename T>
		inline T
		normalize(T const & rhs)
		{
			return rhs * recip_sqrt(length_sq(rhs));
		}


		// 2D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline T
		cross(Vector_T<T, 2> const & lhs, Vector_T<T, 2> const & rhs)
		{
			return lhs.x() * rhs.y() - lhs.y() * rhs.x();
		}


		// 3D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline T
		angle(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs)
		{
			return acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
		}

		template <typename T>
		inline Vector_T<T, 3>
		cross(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs)
		{
			return Vector_T<T, 3>(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
				lhs.z() * rhs.x() - lhs.x() * rhs.z(),
				lhs.x() * rhs.y() - lhs.y() * rhs.x());
		}

		template <typename T>
		inline Vector_T<T, 3>
		reflect(Vector_T<T, 3> const & incident, Vector_T<T, 3> const & normal)
		{
			return incident - 2 * dot(incident, normal) * normal;
		}

		template <typename T>
		inline Vector_T<T, 3>
		transform_quat(Vector_T<T, 3> const & v, Quaternion_T<T> const & quat)
		{
			// result = av + bq + c(q.v CROSS v)
			// where
			//  a = q.w()^2 - (q.v DOT q.v)
			//  b = 2 * (q.v DOT v)
			//  c = 2q.w()
			T const a(quat.w() * quat.w() - length_sq(quat.v()));
			T const b(2 * dot(quat.v(), v));
			T const c(quat.w() + quat.w());

			// Must store this, because result may alias v
			Vector_T<T, 3> cross_v(cross(quat.v(), v));		// q.v CROSS v

			return Vector_T<T, 3>(a * v.x() + b * quat.x() + c * cross_v.x(),
				a * v.y() + b * quat.y() + c * cross_v.y(),
				a * v.z() + b * quat.z() + c * cross_v.z());
		}

		template <typename T>
		inline Vector_T<T, 3>
		project(Vector_T<T, 3> const & vec,
			Matrix4_T<T> const & world, Matrix4_T<T> const & view, Matrix4_T<T> const & proj,
			int const viewport[4], T const & nearPlane, T const & farPlane)
		{
			Vector_T<T, 4> temp(transform(vec, world));
			temp = transform(temp, view);
			temp = transform(temp, proj);
			temp /= temp.w();

			Vector_T<T, 3> ret;
			ret.x() = (temp.x() + 1) * viewport[2] / 2 + viewport[0];
			ret.y() = (-temp.y() + 1) * viewport[3] / 2 + viewport[1];
			ret.z() = (temp.z() + 1) * (farPlane - nearPlane) / 2 + nearPlane;
			return ret;
		}

		template <typename T>
		inline Vector_T<T, 3>
		unproject(Vector_T<T, 3> const & winVec, const T& clipW,
			Matrix4_T<T> const & world, Matrix4_T<T> const & view, Matrix4_T<T> const & proj,
			int const viewport[4], T const & nearPlane, T const & farPlane)
		{
			Vector_T<T, 4> temp;
			temp.x() = 2 * (winVec.x() - viewport[0]) / viewport[2] - 1;
			temp.y() = -(2 * (winVec.y() - viewport[1]) / viewport[3] - 1);
			temp.z() = 2 * (winVec.z() - nearPlane) / (farPlane - nearPlane) - 1;
			temp.w() = clipW;

			Matrix4_T<T> mat(Inverse(world * view * proj));
			temp = Transform(temp, mat);

			return Vector_T<T, 3>(temp.x(), temp.y(), temp.z()) / temp.w();
		}


		// 4D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline Vector_T<T, 4>
		cross(Vector_T<T, 4> const & v1, Vector_T<T, 4> const & v2, Vector_T<T, 4> const & v3)
		{
			T const A = (v2.x() * v3.y()) - (v2.y() * v3.x());
			T const B = (v2.x() * v3.z()) - (v2.z() * v3.x());
			T const C = (v2.x() * v3.w()) - (v2.w() * v3.x());
			T const D = (v2.y() * v3.z()) - (v2.z() * v3.y());
			T const E = (v2.y() * v3.w()) - (v2.w() * v3.y());
			T const F = (v2.z() * v3.w()) - (v2.w() * v3.z());

			return Vector_T<T, 4>((v1.y() * F) - (v1.z() * E) + (v1.w() * D),
				-(v1.x() * F) + (v1.z() * C) - (v1.w() * B),
				(v1.x() * E) - (v1.y() * C) + (v1.w() * A),
				-(v1.x() * D) + (v1.y() * B) - (v1.z() * A));
		}


		// 4D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline Matrix4_T<T>
		mul(Matrix4_T<T> const & lhs, Matrix4_T<T> const & rhs)
		{
			return Matrix4_T<T>(
				lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0) + lhs(0, 3) * rhs(3, 0),
				lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1) + lhs(0, 3) * rhs(3, 1),
				lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2) + lhs(0, 3) * rhs(3, 2),
				lhs(0, 0) * rhs(0, 3) + lhs(0, 1) * rhs(1, 3) + lhs(0, 2) * rhs(2, 3) + lhs(0, 3) * rhs(3, 3),

				lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0) + lhs(1, 3) * rhs(3, 0),
				lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1) + lhs(1, 3) * rhs(3, 1),
				lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2) + lhs(1, 3) * rhs(3, 2),
				lhs(1, 0) * rhs(0, 3) + lhs(1, 1) * rhs(1, 3) + lhs(1, 2) * rhs(2, 3) + lhs(1, 3) * rhs(3, 3),

				lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0) + lhs(2, 3) * rhs(3, 0),
				lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1) + lhs(2, 3) * rhs(3, 1),
				lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2) + lhs(2, 3) * rhs(3, 2),
				lhs(2, 0) * rhs(0, 3) + lhs(2, 1) * rhs(1, 3) + lhs(2, 2) * rhs(2, 3) + lhs(2, 3) * rhs(3, 3),

				lhs(3, 0) * rhs(0, 0) + lhs(3, 1) * rhs(1, 0) + lhs(3, 2) * rhs(2, 0) + lhs(3, 3) * rhs(3, 0),
				lhs(3, 0) * rhs(0, 1) + lhs(3, 1) * rhs(1, 1) + lhs(3, 2) * rhs(2, 1) + lhs(3, 3) * rhs(3, 1),
				lhs(3, 0) * rhs(0, 2) + lhs(3, 1) * rhs(1, 2) + lhs(3, 2) * rhs(2, 2) + lhs(3, 3) * rhs(3, 2),
				lhs(3, 0) * rhs(0, 3) + lhs(3, 1) * rhs(1, 3) + lhs(3, 2) * rhs(2, 3) + lhs(3, 3) * rhs(3, 3));
		}

		template <typename T>
		inline T
		determinant(Matrix4_T<T> const & rhs)
		{
			T const _3142_3241(rhs(2, 0) * rhs(3, 1) - rhs(2, 1) * rhs(3, 0));
			T const _3143_3341(rhs(2, 0) * rhs(3, 2) - rhs(2, 2) * rhs(3, 0));
			T const _3144_3441(rhs(2, 0) * rhs(3, 3) - rhs(2, 3) * rhs(3, 0));
			T const _3243_3342(rhs(2, 1) * rhs(3, 2) - rhs(2, 2) * rhs(3, 1));
			T const _3244_3442(rhs(2, 1) * rhs(3, 3) - rhs(2, 3) * rhs(3, 1));
			T const _3344_3443(rhs(2, 2) * rhs(3, 3) - rhs(2, 3) * rhs(3, 2));

			return rhs(0, 0) * (rhs(1, 1) * _3344_3443 - rhs(1, 2) * _3244_3442 + rhs(1, 3) * _3243_3342)
				- rhs(0, 1) * (rhs(1, 0) * _3344_3443 - rhs(1, 2) * _3144_3441 + rhs(1, 3) * _3143_3341)
				+ rhs(0, 2) * (rhs(1, 0) * _3244_3442 - rhs(1, 1) * _3144_3441 + rhs(1, 3) * _3142_3241)
				- rhs(0, 3) * (rhs(1, 0) * _3243_3342 - rhs(1, 1) * _3143_3341 + rhs(1, 2) * _3142_3241);
		}

		template <typename T>
		inline Matrix4_T<T>
		inverse(Matrix4_T<T> const & rhs)
		{
			T const _2132_2231(rhs(1, 0) * rhs(2, 1) - rhs(1, 1) * rhs(2, 0));
			T const _2133_2331(rhs(1, 0) * rhs(2, 2) - rhs(1, 2) * rhs(2, 0));
			T const _2134_2431(rhs(1, 0) * rhs(2, 3) - rhs(1, 3) * rhs(2, 0));
			T const _2142_2241(rhs(1, 0) * rhs(3, 1) - rhs(1, 1) * rhs(3, 0));
			T const _2143_2341(rhs(1, 0) * rhs(3, 2) - rhs(1, 2) * rhs(3, 0));
			T const _2144_2441(rhs(1, 0) * rhs(3, 3) - rhs(1, 3) * rhs(3, 0));
			T const _2233_2332(rhs(1, 1) * rhs(2, 2) - rhs(1, 2) * rhs(2, 1));
			T const _2234_2432(rhs(1, 1) * rhs(2, 3) - rhs(1, 3) * rhs(2, 1));
			T const _2243_2342(rhs(1, 1) * rhs(3, 2) - rhs(1, 2) * rhs(3, 1));
			T const _2244_2442(rhs(1, 1) * rhs(3, 3) - rhs(1, 3) * rhs(3, 1));
			T const _2334_2433(rhs(1, 2) * rhs(2, 3) - rhs(1, 3) * rhs(2, 2));
			T const _2344_2443(rhs(1, 2) * rhs(3, 3) - rhs(1, 3) * rhs(3, 2));
			T const _3142_3241(rhs(2, 0) * rhs(3, 1) - rhs(2, 1) * rhs(3, 0));
			T const _3143_3341(rhs(2, 0) * rhs(3, 2) - rhs(2, 2) * rhs(3, 0));
			T const _3144_3441(rhs(2, 0) * rhs(3, 3) - rhs(2, 3) * rhs(3, 0));
			T const _3243_3342(rhs(2, 1) * rhs(3, 2) - rhs(2, 2) * rhs(3, 1));
			T const _3244_3442(rhs(2, 1) * rhs(3, 3) - rhs(2, 3) * rhs(3, 1));
			T const _3344_3443(rhs(2, 2) * rhs(3, 3) - rhs(2, 3) * rhs(3, 2));

			// ����ʽ��ֵ
			T const det(determinant(rhs));
			if (!equal<T>(det, 0))
			{
				T invDet(T(1) / det);

				return Matrix4_T<T>(
					+invDet * (rhs(1, 1) * _3344_3443 - rhs(1, 2) * _3244_3442 + rhs(1, 3) * _3243_3342),
					-invDet * (rhs(0, 1) * _3344_3443 - rhs(0, 2) * _3244_3442 + rhs(0, 3) * _3243_3342),
					+invDet * (rhs(0, 1) * _2344_2443 - rhs(0, 2) * _2244_2442 + rhs(0, 3) * _2243_2342),
					-invDet * (rhs(0, 1) * _2334_2433 - rhs(0, 2) * _2234_2432 + rhs(0, 3) * _2233_2332),

					-invDet * (rhs(1, 0) * _3344_3443 - rhs(1, 2) * _3144_3441 + rhs(1, 3) * _3143_3341),
					+invDet * (rhs(0, 0) * _3344_3443 - rhs(0, 2) * _3144_3441 + rhs(0, 3) * _3143_3341),
					-invDet * (rhs(0, 0) * _2344_2443 - rhs(0, 2) * _2144_2441 + rhs(0, 3) * _2143_2341),
					+invDet * (rhs(0, 0) * _2334_2433 - rhs(0, 2) * _2134_2431 + rhs(0, 3) * _2133_2331),

					+invDet * (rhs(1, 0) * _3244_3442 - rhs(1, 1) * _3144_3441 + rhs(1, 3) * _3142_3241),
					-invDet * (rhs(0, 0) * _3244_3442 - rhs(0, 1) * _3144_3441 + rhs(0, 3) * _3142_3241),
					+invDet * (rhs(0, 0) * _2244_2442 - rhs(0, 1) * _2144_2441 + rhs(0, 3) * _2142_2241),
					-invDet * (rhs(0, 0) * _2234_2432 - rhs(0, 1) * _2134_2431 + rhs(0, 3) * _2132_2231),

					-invDet * (rhs(1, 0) * _3243_3342 - rhs(1, 1) * _3143_3341 + rhs(1, 2) * _3142_3241),
					+invDet * (rhs(0, 0) * _3243_3342 - rhs(0, 1) * _3143_3341 + rhs(0, 2) * _3142_3241),
					-invDet * (rhs(0, 0) * _2243_2342 - rhs(0, 1) * _2143_2341 + rhs(0, 2) * _2142_2241),
					+invDet * (rhs(0, 0) * _2233_2332 - rhs(0, 1) * _2133_2331 + rhs(0, 2) * _2132_2231));
			}
			else
			{
				return rhs;
			}
		}

		template <typename T>
		inline Matrix4_T<T>
		look_at_lh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt,
			Vector_T<T, 3> const & vUp = Vector_T<T, 3>(0, 1, 0))
		{
			Vector_T<T, 3> zAxis(normalize(vAt - vEye));
			Vector_T<T, 3> xAxis(normalize(cross(vUp, zAxis)));
			Vector_T<T, 3> yAxis(cross(zAxis, xAxis));

			return Matrix4_T<T>(
				xAxis.x(),			yAxis.x(),			zAxis.x(),			0,
				xAxis.y(),			yAxis.y(),			zAxis.y(),			0,
				xAxis.z(),			yAxis.z(),			zAxis.z(),			0,
				-dot(xAxis, vEye),	-dot(yAxis, vEye),	-dot(zAxis, vEye),	1);
		}

		template <typename T>
		inline Matrix4_T<T>
		look_at_rh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt,
			Vector_T<T, 3> const & vUp = Vector_T<T, 3>(0, 1, 0))
		{
			Vector_T<T, 3> zAxis(normalize(vEye - vAt));
			Vector_T<T, 3> xAxis(normalize(Cross(vUp, zAxis)));
			Vector_T<T, 3> yAxis(cross(zAxis, xAxis));

			return Matrix4_T<T>(
				xAxis.x(),			yAxis.x(),			zAxis.x(),			0,
				xAxis.y(),			yAxis.y(),			zAxis.y(),			0,
				xAxis.z(),			yAxis.z(),			zAxis.z(),			0,
				-dot(xAxis, vEye),	-dot(yAxis, vEye),	-dot(zAxis, vEye),	1);
		}

		template <typename T>
		inline Matrix4_T<T>
		ortho_lh(T const & w, T const & h, T const & nearPlane, T const & farPlane)
		{
			T const w_2(w / 2);
			T const h_2(h / 2);
			return ortho_off_center_lh(-w_2, w_2, -h_2, h_2, nearPlane, farPlane);
		}
		template <typename T>
		inline Matrix4_T<T>
		ortho_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane)
		{
			T const q(T(1) / (farPlane - nearPlane));
			T const invWidth(T(1) / (right - left));
			T const invHeight(T(1) / (top - bottom));

			return Matrix4_T<T>(
				invWidth + invWidth,		0,								0,					0,
				0,							invHeight + invHeight,			0,					0,
				0,							0,								q,					0,
				-(left + right) * invWidth,	-(top + bottom) * invHeight,	-nearPlane * q,		1);
		}

		template <typename T>
		inline Matrix4_T<T>
		perspective_lh(T const & width, T const & height, T const & nearPlane, T const & farPlane)
		{
			T const q(farPlane / (farPlane - nearPlane));
			T const near2(nearPlane + nearPlane);

			return Matrix4_T<T>(
				near2 / width,	0,				0,				0,
				0,				near2 / height,	0,				0,
				0,				0,				q,				1,
				0,				0,				-nearPlane * q, 0);
		}
		template <typename T>
		inline Matrix4_T<T>
		perspective_fov_lh(T const & fov, T const & aspect, T const & nearPlane, T const & farPlane)
		{
			T const h(T(1) / tan(fov / 2));
			T const w(h / aspect);
			T const q(farPlane / (farPlane - nearPlane));

			return Matrix4_T<T>(
				w,		0,		0,				0,
				0,		h,		0,				0,
				0,		0,		q,				1,
				0,		0,		-nearPlane * q, 0);
		}
		template <typename T>
		inline Matrix4_T<T>
		perspective_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane)
		{
			T const q(farPlane / (farPlane - nearPlane));
			T const near2(nearPlane + nearPlane);
			T const invWidth(T(1) / (right - left));
			T const invHeight(T(1) / (top - bottom));

			return Matrix4_T<T>(
				near2 * invWidth,			0,								0,				0,
				0,							near2 * invHeight,				0,				0,
				-(left + right)* invWidth,	-(top + bottom) * invHeight,	q,				1,
				0,							0,								-nearPlane * q, 0);
		}

		template <typename T>
		inline Matrix4_T<T>
		reflect(Plane_T<T> const & p)
		{
			Plane_T<T> P(normalize(p));
			T const aa2(-2 * P.a() * P.a()), ab2(-2 * P.a() * P.b()), ac2(-2 * P.a() * P.c()), ad2(-2 * P.a() * P.d());
			T const bb2(-2 * P.b() * P.b()), bc2(-2 * P.b() * P.c()), bd2(-2 * P.a() * P.c());
			T const cc2(-2 * P.c() * P.c()), cd2(-2 * P.c() * P.d());

			return Matrix4_T<T>(
				aa2 + 1,	ab2,		ac2,		0,
				ab2,		bb2 + 1,	bc2,		0,
				ac2,		bc2,		cc2 + 1,	0,
				ad2,		bd2,		cd2,		1);
		}

		template <typename T>
		inline Matrix4_T<T>
		rotation_x(T const & x)
		{
			float sx, cx;
			sin_cos(x, sx, cx);

			return Matrix4_T<T>(
				1,	0,		0,		0,
				0,	cx,		sx,		0,
				0,	-sx,	cx,		0,
				0,	0,		0,		1);
		}
		template <typename T>
		inline Matrix4_T<T>
		rotation_y(T const & y)
		{
			float sy, cy;
			sin_cos(y, sy, cy);

			return Matrix4_T<T>(
				cy,		0,		-sy,	0,
				0,		1,		0,		0,
				sy,		0,		cy,		0,
				0,		0,		0,		1);
		}
		template <typename T>
		inline Matrix4_T<T>
		rotation_z(T const & z)
		{
			float sz, cz;
			sin_cos(z, sz, cz);

			return Matrix4_T<T>(
				cz,		sz,		0,		0,
				-sz,	cz,		0,		0,
				0,		0,		1,		0,
				0,		0,		0,		1);
		}
		template <typename T>
		inline Matrix4_T<T>
		rotation(T const & angle, T const & x, T const & y, T const & z)
		{
			Quaternion_T<T> quat(rotation_axis(Vector_T<T, 3>(x, y, z), angle));
			return to_matrix(quat);
		}
		template <typename T>
		inline Matrix4_T<T>
		rotation_matrix_yaw_pitch_roll(T const & yaw, T const & pitch, T const & roll)
		{
			Matrix4_T<T> rotX(rotation_x(pitch));
			Matrix4_T<T> rotY(rotation_y(yaw));
			Matrix4_T<T> rotZ(rotation_z(roll));
			return rotZ * rotX * rotY;
		}

		template <typename T>
		inline Matrix4_T<T>
		scaling(T const & sx, T const & sy, T const & sz)
		{
			return Matrix4_T<T>(
				sx,	0,	0,	0,
				0,	sy,	0,	0,
				0,	0,	sz,	0,
				0,	0,	0,	1);
		}

		template <typename T>
		inline Matrix4_T<T>
		shadow(Vector_T<T, 4> const & L, Plane_T<T> const & p)
		{
			Vector_T<T, 4> const v(-L);
			Plane_T<T> P(normalize(p));
			T const d(-dot(P, v));

			return Matrix4_T<T>(
				P.a() * v.x() + d,	P.a() * v.y(),		P.a() * v.z(),		P.a() * v.w(),
				P.b() * v.x(),		P.b() * v.y() + d,	P.b() * v.z(),		P.b() * v.w(),
				P.c() * v.x(),		P.c() * v.y(),		P.c() * v.z() + d,	P.c() * v.w(),
				P.d() * v.x(),		P.d() * v.y(),		P.d() * v.z(),		P.d() * v.w() + d);
		}

		template <typename T>
		inline Matrix4_T<T>
		to_matrix(Quaternion_T<T> const & quat)
		{
			// calculate coefficients
			T const x2(quat.x() + quat.x());
			T const y2(quat.y() + quat.y());
			T const z2(quat.z() + quat.z());

			T const xx2(quat.x() * x2), xy2(quat.x() * y2), xz2(quat.x() * z2);
			T const yy2(quat.y() * y2), yz2(quat.y() * z2), zz2(quat.z() * z2);
			T const wx2(quat.w() * x2), wy2(quat.w() * y2), wz2(quat.w() * z2);

			return Matrix4_T<T>(
				1 - yy2 - zz2,	xy2 + wz2,		xz2 - wy2,		0,
				xy2 - wz2,		1 - xx2 - zz2,	yz2 + wx2,		0,
				xz2 + wy2,		yz2 - wx2,		1 - xx2 - yy2,	0,
				0,				0,				0,				1);
		}

		template <typename T>
		inline Matrix4_T<T>
		translation(T const & x, T const & y, T const & z)
		{
			return Matrix4_T<T>(
				1,	0,	0,	0,
				0,	1,	0,	0,
				0,	0,	1,	0,
				x,	y,	z,	1);
		}

		template <typename T>
		inline Matrix4_T<T>
		transpose(Matrix4_T<T> const & rhs)
		{
			return Matrix4_T<T>(
				rhs(0, 0), rhs(1, 0), rhs(2, 0), rhs(3, 0),
				rhs(0, 1), rhs(1, 1), rhs(2, 1), rhs(3, 1),
				rhs(0, 2), rhs(1, 2), rhs(2, 2), rhs(3, 2),
				rhs(0, 3), rhs(1, 3), rhs(2, 3), rhs(3, 3));
		}

		template <typename T>
		inline Matrix4_T<T>
		lh_to_rh(Matrix4_T<T> const & rhs)
		{
			Matrix4_T<T> ret = rhs;
			ret(2, 0) = -ret(2, 0);
			ret(2, 1) = -ret(2, 1);
			ret(2, 2) = -ret(2, 2);
			ret(2, 3) = -ret(2, 3);
			return ret;
		}

		template <typename T>
		inline Matrix4_T<T>
		scaling(Vector_T<T, 3> const & vPos)
		{
			return scaling(vPos.x(), vPos.y(), vPos.z());
		}
		template <typename T>
		inline Matrix4_T<T>
		translation(Vector_T<T, 3> const & vPos)
		{
			return translation(vPos.x(), vPos.y(), vPos.z());
		}
		template <typename T>
		inline Matrix4_T<T>
		ortho_rh(T const & width, T const & height, T const & nearPlane, T const & farPlane)
		{
			return lh_to_rh(ortho_lh(width, height, nearPlane, farPlane));
		}
		template <typename T>
		inline Matrix4_T<T>
		ortho_off_center_rh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane)
		{
			return lh_to_rh(ortho_off_center_lh(left, right, bottom, top, nearPlane, farPlane));
		}
		template <typename T>
		inline Matrix4_T<T>
		perspective_rh(T const & width, T const & height,
			T const & nearPlane, T const & farPlane)
		{
			return lh_to_rh(perspective_lh(width, height, nearPlane, farPlane));
		}
		template <typename T>
		inline Matrix4_T<T>
		perspective_fov_rh(T const & fov, T const & aspect,
			T const & nearPlane, T const & farPlane)
		{
			return lh_to_rh(perspective_fov_lh(fov, aspect, nearPlane, farPlane));
		}
		template <typename T>
		inline Matrix4_T<T>
		perspective_off_center_rh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane)
		{
			return lh_to_rh(perspective_off_center_lh(left, right, bottom, top, nearPlane, farPlane));
		}

		template <typename T>
		inline Matrix4_T<T>
		rh_to_lh(Matrix4_T<T> const & rhs)
		{
			return lh_to_rh(rhs);
		}

		template <typename T>
		inline Matrix4_T<T>
		rotation_matrix_yaw_pitch_roll(Vector_T<T, 3> const & ang)
		{
			return rotation_matrix_yaw_pitch_roll(ang.x(), ang.y(), ang.z());
		}


		// ��Ԫ��
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline Quaternion_T<T>
		conjugate(Quaternion_T<T> const & rhs)
		{
			return Quaternion_T<T>(-rhs.x(), -rhs.y(), -rhs.z(), rhs.w());
		}

		template <typename T>
		inline Quaternion_T<T>
		axis_to_axis(Vector_T<T, 3> const & from, Vector_T<T, 3> const & to)
		{
			Vector_T<T, 3> a(normalize(from));
			Vector_T<T, 3> b(normalize(to));

			return unit_axis_to_unit_axis(a, b);
		}
		template <typename T>
		inline Quaternion_T<T>
		unit_axis_to_unit_axis(Vector_T<T, 3> const & from, Vector_T<T, 3> const & to)
		{
			T dot = Abs(dot(from, to));
			if (equal(dot, T(1)))
			{
				return Quaternion_T<T>::Identity();
			}
			else
			{
				if (equal(dot, T(-1)))
				{
					return Quaternion_T<T>(1, 0, 0, 0);
				}
				else
				{
					Vector_T<T, 3> axis = cross(from, to);

					T const cos_theta = dot(from, to);
					T const sin_theta = sqrt(1 - cos_theta * cos_theta);
					T const sin_half_theta = sqrt((1 - cos_theta) / 2);
					T const cos_half_theta = sin_theta / (2 * sin_half_theta);

					return Quaternion_T<T>(axis * sin_half_theta, cos_half_theta);
				}
			}
		}

		template <typename T>
		inline Quaternion_T<T>
		bary_centric(Quaternion_T<T> const & q1, Quaternion_T<T> const & q2,
			Quaternion_T<T> const & q3, T const & f, T const & g)
		{
			T const temp(f + g);
			Quaternion_T<T> qT1(slerp(q1, q2, temp));
			Quaternion_T<T> qT2(slerp(q1, q3, temp));

			return slerp(qT1, qT2, g / temp);
		}

		template <typename T>
		inline Quaternion_T<T>
		exp(Quaternion_T<T> const & rhs)
		{
			T const theta(length(rhs.v()));
			return Quaternion_T<T>(normalize(rhs.v()) * Sin(theta), Cos(theta));
		}
		template <typename T>
		inline Quaternion_T<T>
		ln(Quaternion_T<T> const & rhs)
		{
			T const theta_2(acos(rhs.w()));
			return Quaternion_T<T>(Normalize(rhs.v()) * (theta_2 + theta_2), 0);
		}

		template <typename T>
		inline Quaternion_T<T>
		inverse(Quaternion_T<T> const & rhs)
		{
			T const inv(T(1) / length(rhs));
			return Quaternion(-rhs.x() * inv, -rhs.y() * inv, -rhs.z() * inv, rhs.w() * inv);
		}

		template <typename T>
		inline Quaternion_T<T>
		mul(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs)
		{
			return Quaternion_T<T>(
				lhs.x() * rhs.w() - lhs.y() * rhs.z() + lhs.z() * rhs.y() + lhs.w() * rhs.x(),
				lhs.x() * rhs.z() + lhs.y() * rhs.w() - lhs.z() * rhs.x() + lhs.w() * rhs.y(),
				lhs.y() * rhs.x() - lhs.x() * rhs.y() + lhs.z() * rhs.w() + lhs.w() * rhs.z(),
				lhs.w() * rhs.w() - lhs.x() * rhs.x() - lhs.y() * rhs.y() - lhs.z() * rhs.z());
		}

		template <typename T>
		inline Quaternion_T<T>
		rotation_quat_yaw_pitch_roll(T const & yaw, T const & pitch, T const & roll)
		{
			T const angX(pitch / 2), angY(yaw / 2), angZ(roll / 2);
			T sx, sy, sz;
			T cx, cy, cz;
			sin_cos(angX, sx, cx);
			sin_cos(angY, sy, cy);
			sin_cos(angZ, sz, cz);

			return Quaternion_T<T>(
				sx * cy * cz + cx * sy * sz,
				cx * sy * cz - sx * cy * sz,
				cx * cy * sz - sx * sy * cz,
				sx * sy * sz + cx * cy * cz);
		}

		template <typename T>
		inline void
		to_axis_angle(Vector_T<T, 3>& vec, T& ang, Quaternion_T<T> const & quat)
		{
			T const tw(acos(quat.w()));
			T const stw = sin(tw);

			ang = tw + tw;
			vec = quat.v();
			if (!equal<T>(stw, 0))
			{
				vec /= stw;
			}
		}

		template <typename T>
		inline Quaternion_T<T>
		to_quaternion(Matrix4_T<T> const & mat)
		{
			Quaternion_T<T> quat;
			T s;
			T const tr(mat(0, 0) + mat(1, 1) + mat(2, 2));

			// check the diagonal
			if (tr > 0)
			{
				s = sqrt(tr + 1);
				quat.w() = s * T(0.5);
				s = T(0.5) / s;
				quat.x() = (mat(1, 2) - mat(2, 1)) * s;
				quat.y() = (mat(2, 0) - mat(0, 2)) * s;
				quat.z() = (mat(0, 1) - mat(1, 0)) * s;
			}
			else
			{
				if ((mat(1, 1) > mat(0, 0)) && (mat(2, 2) <= mat(1, 1)))
				{
					s = sqrt((mat(1, 1) - (mat(2, 2) + mat(0, 0))) + 1);

					quat.y() = s * T(0.5);

					if (!equal<T>(s, 0))
					{
						s = T(0.5) / s;
					}

					quat.w() = (mat(2, 0) - mat(0, 2)) * s;
					quat.z() = (mat(2, 1) + mat(1, 2)) * s;
					quat.x() = (mat(0, 1) + mat(1, 0)) * s;
				}
				else
				{
					if (((mat(1, 1) <= mat(0, 0)) && (mat(2, 2) > mat(0, 0))) || (mat(2, 2) > mat(1, 1)))
					{
						s = sqrt((mat(2, 2) - (mat(0, 0) + mat(1, 1))) + 1);

						quat.z() = s * T(0.5);

						if (!equal<T>(s, 0))
						{
							s = T(0.5) / s;
						}

						quat.w() = (mat(0, 1) - mat(1, 0)) * s;
						quat.x() = (mat(0, 2) + mat(2, 0)) * s;
						quat.y() = (mat(1, 2) + mat(2, 1)) * s;
					}
					else
					{
						s = sqrt((mat(0, 0) - (mat(1, 1) + mat(2, 2))) + 1);

						quat.x() = s * T(0.5);

						if (!equal<T>(s, 0))
						{
							s = T(0.5) / s;
						}

						quat.w() = (mat(1, 2) - mat(2, 1)) * s;
						quat.y() = (mat(1, 0) + mat(0, 1)) * s;
						quat.z() = (mat(2, 0) + mat(0, 2)) * s;
					}
				}
			}

			return normalize(quat);
		}

		template <typename T>
		inline Quaternion_T<T>
		rotation_axis(Vector_T<T, 3> const & v, T const & angle)
		{
			T sa, ca;
			sin_cos(angle * T(0.5), sa, ca);

			if (equal<T>(length_sq(v), 0))
			{
				return Quaternion_T<T>(sa, sa, sa, ca);
			}
			else
			{
				return Quaternion_T<T>(sa * normalize(v), ca);
			}
		}

		template <typename T>
		inline Quaternion_T<T>
		slerp(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs, T const & slerp)
		{
			T scale0, scale1;
			Quaternion_T<T> q2;

			// DOT the quats to get the cosine of the angle between them
			T const cosom(dot(lhs, rhs));

			// Two special cases:
			// Quats are exactly opposite, within DELTA?
			if (cosom > std::numeric_limits<T>::epsilon() - T(1))
			{
				// make sure they are different enough to avoid a divide by 0
				if (cosom < T(1) - std::numeric_limits<T>::epsilon())
				{
					// SLERP away
					T const omega(acos(cosom));
					T const isinom(T(1) / sin(omega));
					scale0 = sin((T(1) - slerp) * omega) * isinom;
					scale1 = sin(slerp * omega) * isinom;
				}
				else
				{
					// LERP is good enough at this distance
					scale0 = T(1)- slerp;
					scale1 = slerp;
				}

				q2 = rhs * scale1;
			}
			else
			{
				// SLERP towards a perpendicular quat
				// Set slerp parameters
				scale0 = sin((T(1) - slerp) * PIdiv2);
				scale1 = sin(slerp * PIdiv2);

				q2.x() = -rhs.y() * scale1;
				q2.y() = +rhs.x() * scale1;
				q2.z() = -rhs.w() * scale1;
				q2.w() = +rhs.z() * scale1;
			}

			// Compute the result
			return scale0 * lhs + q2;
		}

		template <typename T>
		inline Quaternion_T<T>
		rotation_quat_yaw_pitch_roll(Vector_T<T, 3> const & ang)
		{
			return rotation_quat_yaw_pitch_roll(ang.x(), ang.y(), ang.z());
		}


		// ƽ��
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline T
		dot(Plane_T<T> const & lhs, Vector_T<T, 4> const & rhs)
		{
			return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.c() * rhs.z() + lhs.d() * rhs.w();
		}
		template <typename T>
		inline T
		dot_coord(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs)
		{
			return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.c() * rhs.z() + lhs.d();
		}
		template <typename T>
		inline T
		dot_normal(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs)
		{
			return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.c() * rhs.z();
		}

		template <typename T>
		inline Plane_T<T>
		normalize(Plane_T<T> const & rhs)
		{
			T const inv(T(1) / length(rhs));
			return Plane_T<T>(rhs.a() * inv, rhs.b() * inv, rhs.c() * inv, rhs.d() * inv);
		}
		template <typename T>
		inline Plane_T<T>
		from_point_normal(Vector_T<T, 3> const & point, Vector_T<T, 3> const & normal)
		{
			return Plane(normal.x(), normal.y(), normal.z(), -Dot(point, normal));
		}
		template <typename T>
		inline Plane_T<T>
		from_points(Vector_T<T, 3> const & v0, Vector_T<T, 3> const & v1, Vector_T<T, 3> const & v2)
		{
			Vector_T<T, 3> vec(cross(v1 - v0, v2 - v0));
			return from_point_normal(v0, normalize(vec));
		}
		template <typename T>
		inline Plane_T<T>
		mul(Plane_T<T> const & p, Matrix4_T<T> const & mat)
		{
			return Plane_T<T>(
				p.a() * mat(0, 0) + p.b() * mat(1, 0) + p.c() * mat(2, 0) + p.d() * mat(3, 0),
				p.a() * mat(0, 1) + p.b() * mat(1, 1) + p.c() * mat(2, 1) + p.d() * mat(3, 1),
				p.a() * mat(0, 2) + p.b() * mat(1, 2) + p.c() * mat(2, 2) + p.d() * mat(3, 2),
				p.a() * mat(0, 3) + p.b() * mat(1, 3) + p.c() * mat(2, 3) + p.d() * mat(3, 3));
		}

		// ��ֱ�ߺ�ƽ��Ľ��㣬ֱ��orig + t * dir
		template <typename T>
		inline T
		intersect_line(Plane_T<T> const & p,
			Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir)
		{
			T deno(dot(dir, p.Normal()));
			if (equal(deno, T(0)))
			{
				deno = T(0.0001);
			}

			return -dot_coord(p, orig) / deno;
		}


		// ��ɫ
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline Color_T<T>
		negative(Color_T<T> const & rhs)
		{
			return Color_T<T>(1 - rhs.r(), 1 - rhs.g(), 1 - rhs.b(), rhs.a());
		}
		template <typename T>
		inline Color_T<T>
		modulate(Color_T<T> const & lhs, Color_T<T> const & rhs)
		{
			return Color_T<T>(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b(), lhs.a() * rhs.a());
		}


		// ��Χ
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline bool
		vec_in_sphere(Sphere_T<T> const & sphere, Vector_T<T, 3> const & v)
		{
			if (Length(v - sphere.Center()) < sphere.Radius())
			{
				return true;
			}
			return false;
		}

		template <typename T>
		inline bool
		bound_probe(Sphere_T<T> const & sphere, Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir)
		{
			T const a = length_sq(dir);
			T const b = 2 * dot(dir, orig - sphere.Center());
			T const c = length_sq(orig - sphere.Center()) - sphere.Radius() * sphere.Radius();

			if (b * b - 4 * a * c < 0)
			{
				return false;
			}
			return true;
		}

		template <typename T>
		inline bool
		vec_in_box(Box_T<T> const & box, Vector_T<T, 3> const & v)
		{
			return (in_bound(v.x(), box.Min().x(), box.Max().x()))
				&& (in_bound(v.y(), box.Min().y(), box.Max().y()))
				&& (in_bound(v.z(), box.Min().z(), box.Max().z()));
		}

		template <typename T>
		inline bool
		bound_probe(Box_T<T> const & box, Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir)
		{
			Vector_T<T, 3> const leftBottomNear(box.LeftBottomNear());
			Vector_T<T, 3> const leftTopNear(box.LeftTopNear());
			Vector_T<T, 3> const rightTopNear(box.RightTopNear());
			Vector_T<T, 3> const leftTopFar(box.LeftTopFar());

			Plane_T<T> pNear(FromPoints(leftBottomNear, leftTopNear, rightTopNear));
			Plane_T<T> pTop(FromPoints(leftTopNear, leftTopFar, rightTopNear));
			Plane_T<T> pLeft(FromPoints(leftTopFar, leftTopNear, leftBottomNear));

			T t = intersect_line(pNear, orig, dir);
			if (t >= 0)
			{
				Vector_T<T, 3> vec = orig + t * dir;
				if (!(in_bound(vec.x(), leftBottomNear.x(), rightTopNear.x())
					&& in_bound(vec.y(), leftBottomNear.y(), leftTopNear.y())))
				{
					return false;
				}
			}

			t = intersect_line(pTop, orig, dir);
			if (t >= 0)
			{
				Vector_T<T, 3> vec = orig + t * dir;
				if (!(in_bound(vec.x(), leftTopNear.x(), rightTopNear.x())
					&& in_bound(vec.z(), leftTopNear.z(), leftTopFar.z())))
				{
					return false;
				}
			}

			t = intersect_line(pLeft, orig, dir);
			if (t >= 0)
			{
				Vector_T<T, 3> vec = orig + t * dir;
				if (!(in_bound(vec.y(), leftBottomNear.y(), leftTopNear.y())
					&& in_bound(vec.z(), leftBottomNear.z(), leftTopFar.z())))
				{
					return false;
				}
			}

			return true;
		}

		// from Graphics Gems I p301
		template <typename value_type, typename Iterator>
		inline Box_T<value_type>
		compute_bounding_box(Iterator first, Iterator last)
		{
			Vector_T<value_type, 3> minVec = *first;
			Vector_T<value_type, 3> maxVec = *first;
			Iterator iter = first;
			++ iter;
			for (; iter != last; ++ iter)
			{
				Vector_T<value_type, 3> const & v = *iter;
				minVec = minimize(minVec, v);
				maxVec = maximize(maxVec, v);
			}
			return Box_T<value_type>(minVec, maxVec);
		}

		template <typename value_type, typename Iterator>
		inline Sphere_T<value_type>
		compute_bounding_sphere(Iterator first, Iterator last)
		{
			value_type const min_float = std::numeric_limits<value_type>::min();
			value_type const max_float = std::numeric_limits<value_type>::max();
			Vector_T<value_type, 3> x_min(max_float, max_float, max_float);
			Vector_T<value_type, 3> y_min(max_float, max_float, max_float);
			Vector_T<value_type, 3> z_min(max_float, max_float, max_float);
			Vector_T<value_type, 3> x_max(min_float, min_float, min_float);
			Vector_T<value_type, 3> y_max(min_float, min_float, min_float);
			Vector_T<value_type, 3> z_max(min_float, min_float, min_float);
			for (Iterator iter = first; iter != last; ++ iter)
			{
				if (x_min.x() > iter->x())
				{
					x_min = *iter;
				}
				if (y_min.y() > iter->y())
				{
					y_min = *iter;
				}
				if (z_min.z() > iter->z())
				{
					z_min = *iter;
				}

				if (x_max.x() < iter->x())
				{
					x_max = *iter;
				}
				if (y_max.y() < iter->y())
				{
					y_max = *iter;
				}
				if (z_max.z() < iter->z())
				{
					z_max = *iter;
				}
			}

			value_type x_span = length_sq(x_max - x_min);
			value_type y_span = length_sq(y_max - y_min);
			value_type z_span = length_sq(z_max - z_min);

			Vector_T<value_type, 3> dia1 = x_min;
			Vector_T<value_type, 3> dia2 = x_max;
			value_type max_span = x_span;
			if (y_span > max_span)
			{
				max_span = y_span;
				dia1 = y_min;
				dia2 = y_max;
			}
			if (z_span > max_span)
			{
				max_span = z_span;
				dia1 = z_min;
				dia2 = z_max;
			}

			Vector_T<value_type, 3> center((dia1 + dia2) / 2);
			value_type r = length(dia2 - center);

			for (Iterator iter = first; iter != last; ++ iter)
			{
				value_type d = length(*iter - center);

				if (d > r)
				{
					r = (d + r) / 2;
					center = (r * center + (d - r) * (*iter)) / d;
				}
			}

			return Sphere_T<value_type>(center, r);
		}


		// ����
		///////////////////////////////////////////////////////////////////////////////

		// ����TBN��
		template <typename T, typename TangentIterator, typename BinormIterator,
			typename IndexIterator, typename PositionIterator, typename TexCoordIterator, typename NormalIterator>
		inline void
		compute_tangent(TangentIterator targentsBegin, BinormIterator binormsBegin,
								IndexIterator indicesBegin, IndexIterator indicesEnd,
								PositionIterator xyzsBegin, PositionIterator xyzsEnd,
								TexCoordIterator texsBegin, NormalIterator normalsBegin)
		{
			typedef typename std::iterator_traits<PositionIterator>::value_type position_type;
			typedef typename std::iterator_traits<TexCoordIterator>::value_type texcoord_type;
			typedef typename std::iterator_traits<TexCoordIterator>::value_type tangent_type;
			typedef typename std::iterator_traits<TexCoordIterator>::value_type binormal_type;
			typedef typename std::iterator_traits<TexCoordIterator>::value_type normal_type;

			int const num = static_cast<int>(std::distance(xyzsBegin, xyzsEnd));

			for (int i = 0; i < num; ++ i)
			{
				*(targentsBegin + i) = Vector_T<T, 3>::Zero();
				*(binormsBegin + i) = Vector_T<T, 3>::Zero();
			}

			for (IndexIterator iter = indicesBegin; iter != indicesEnd; iter += 3)
			{
				uint16_t const v0Index = *(iter + 0);
				uint16_t const v1Index = *(iter + 1);
				uint16_t const v2Index = *(iter + 2);

				position_type const & v0XYZ(*(xyzsBegin + v0Index));
				position_type const & v1XYZ(*(xyzsBegin + v1Index));
				position_type const & v2XYZ(*(xyzsBegin + v2Index));

				Vector_T<T, 3> v1v0 = v1XYZ - v0XYZ;
				Vector_T<T, 3> v2v0 = v2XYZ - v0XYZ;

				texcoord_type const & v0Tex(*(texsBegin + v0Index));
				texcoord_type const & v1Tex(*(texsBegin + v1Index));
				texcoord_type const & v2Tex(*(texsBegin + v2Index));

				T s1 = v1Tex.x() - v0Tex.x();
				T t1 = v1Tex.y() - v0Tex.y();

				T s2 = v2Tex.x() - v0Tex.x();
				T t2 = v2Tex.y() - v0Tex.y();

				T denominator = s1 * t2 - s2 * t1;
				Vector_T<T, 3> tangent, binormal;
				if (abs(denominator) < std::numeric_limits<T>::epsilon())
				{
					tangent = Vector_T<T, 3>(1, 0, 0);
					binormal = Vector_T<T, 3>(0, 1, 0);
				}
				else
				{
					tangent = (t2 * v1v0 - t1 * v2v0) / denominator;
					binormal = (s1 * v2v0 - s2 * v1v0) / denominator;
				}

				*(targentsBegin + v0Index) += tangent;
				*(binormsBegin + v0Index) += binormal;

				*(targentsBegin + v1Index) += tangent;
				*(binormsBegin + v1Index) += binormal;

				*(targentsBegin + v2Index) += tangent;
				*(binormsBegin + v2Index) += binormal;
			}

			for (int i = 0; i < num; ++ i)
			{
				Vector_T<T, 3> tangent(*(targentsBegin + i));
				Vector_T<T, 3> binormal(*(binormsBegin + i));
				Vector_T<T, 3> normal(*(normalsBegin + i));

				// Gram-Schmidt orthogonalize
				tangent = normalize(tangent - normal * dot(tangent, normal)); 
				// Calculate handedness
				if (dot(cross(normal, tangent), binormal) < 0)
				{
					tangent = -tangent;
				}

				*(targentsBegin + i) = tangent;
				*(binormsBegin + i) = cross(normal, tangent);
			}
		}

		template <typename T, typename NormalIterator, typename IndexIterator, typename PositionIterator>
		inline void
		compute_normal(NormalIterator normalBegin,
								IndexIterator indicesBegin, IndexIterator indicesEnd,
								PositionIterator xyzsBegin, PositionIterator xyzsEnd)
		{
			typedef typename std::iterator_traits<PositionIterator>::value_type position_type;

			NormalIterator normalEnd = normalBegin;
			std::advance(normalEnd, std::distance(xyzsBegin, xyzsEnd));
			std::fill(normalBegin, normalEnd, Vector_T<T, 3>::Zero());

			for (IndexIterator iter = indicesBegin; iter != indicesEnd; iter += 3)
			{
				uint16_t const v0Index = *(iter + 0);
				uint16_t const v1Index = *(iter + 1);
				uint16_t const v2Index = *(iter + 2);

				position_type const & v0(*(xyzsBegin + v0Index));
				position_type const & v1(*(xyzsBegin + v1Index));
				position_type const & v2(*(xyzsBegin + v2Index));

				Vector_T<T, 3> v03(v0.x(), v0.y(), v0.z());
				Vector_T<T, 3> v13(v1.x(), v1.y(), v1.z());
				Vector_T<T, 3> v23(v2.x(), v2.y(), v2.z());

				Vector_T<T, 3> vec(cross(v13 - v03, v23 - v03));

				*(normalBegin + v0Index) += vec;
				*(normalBegin + v1Index) += vec;
				*(normalBegin + v2Index) += vec;
			}

			for (NormalIterator iter = normalBegin; iter != normalEnd; ++ iter)
			{
				*iter = normalize(*iter);
			}
		}

		template <typename T>
		inline void
		intersect(Vector_T<T, 3> const & v0, Vector_T<T, 3> const & v1, Vector_T<T, 3> const & v2,
						Vector_T<T, 3> const & ray_orig, Vector_T<T, 3> const & ray_dir,
						T& t, T& u, T& v)
		{
			// Find vectors for two edges sharing vert0
			Vector_T<T, 3> edge1 = v1 - v0;
			Vector_T<T, 3> edge2 = v2 - v0;

			// Begin calculating determinant - also used to calculate U parameter
			Vector_T<T, 3> pvec(cross(ray_dir, edge2));

			// If determinant is near zero, ray lies in plane of triangle
			T det = dot(edge1, pvec);

			Vector_T<T, 3> tvec;
			if (det > 0)
			{
				tvec = ray_orig - v0;
			}
			else
			{
				tvec = v0 - ray_orig;
				det = -det;
			}

			// Calculate U parameter
			u = dot(tvec, pvec);

			// Prepare to test V parameter
			Vector_T<T, 3> qvec(cross(tvec, edge1));

			// Calculate V parameter
			v = dot(ray_dir, qvec);

			// Calculate t, scale parameters, ray intersects triangle
			t = dot(edge2, qvec);

			T const inv_det = T(1) / det;
			v *= inv_det;
			u *= inv_det;
			t *= inv_det;
		}

		template <typename T>
		inline bool
		bary_centric_in_triangle(T const & u, T const & v)
		{
			// test bounds
			if ((u < 0) || (u > 1))
			{
				return false;
			}
			if ((v < 0) || (u + v > 1))
			{
				return false;
			}
			return true;
		}
	};
}

#include <KlayGE/Vector.hpp>
#include <KlayGE/Rect.hpp>
#include <KlayGE/Size.hpp>
#include <KlayGE/Matrix.hpp>
#include <KlayGE/Quaternion.hpp>
#include <KlayGE/Plane.hpp>
#include <KlayGE/Color.hpp>
#include <KlayGE/Bound.hpp>
#include <KlayGE/Sphere.hpp>
#include <KlayGE/Box.hpp>

#endif		// _MATH_HPP
