// Math.hpp
// KlayGE ��ѧ������ ͷ�ļ�
// Ver 3.12.0
// ��Ȩ����(C) ������, 2003-2011
// Homepage: http://www.klayge.org
//
// 3.12.0
// Implement moved to Math.cpp (2011.2.15)
//
// 3.8.0
// ������decompose (2009.1.24)
//
// 3.6.0
// ��д��intersect_ray (2007.5.11)
// ����Quake III��recip_sqrt (2007.6.22)
//
// 3.4.0
// ������refract��fresnel_term (2006.8.22)
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

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>

#include <limits>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iterator>

#include <boost/static_assert.hpp>

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
			return static_cast<T>(static_cast<int>(x > 0 ? x : (x - 1)));
		}

		// ȡx��С������
		template <typename T>
		inline T
		frac(T const & x)
		{
			return x - static_cast<int>(x);
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
		KLAYGE_CORE_API float abs(float x);
		KLAYGE_CORE_API float sqrt(float x);
		KLAYGE_CORE_API float recip_sqrt(float number);

		KLAYGE_CORE_API float pow(float x, float y);
		KLAYGE_CORE_API float exp(float x);

		KLAYGE_CORE_API float log(float x);
		KLAYGE_CORE_API float log10(float x);

		KLAYGE_CORE_API float sin(float x);
		KLAYGE_CORE_API float cos(float x);
		KLAYGE_CORE_API void sincos(float x, float& s, float& c);
		KLAYGE_CORE_API float tan(float x);

		KLAYGE_CORE_API float asin(float x);
		KLAYGE_CORE_API float acos(float x);
		KLAYGE_CORE_API float atan(float x);

		KLAYGE_CORE_API float sinh(float x);
		KLAYGE_CORE_API float cosh(float x);
		KLAYGE_CORE_API float tanh(float x);


		// �������͵�Dot
		template <typename T>
		typename T::value_type dot(T const & lhs, T const & rhs);

		// Length��ƽ��
		template <typename T>
		typename T::value_type length_sq(T const & rhs);

		// �������͵�Length
		template <typename T>
		typename T::value_type length(T const & rhs);

		// �������͵�Lerp
		template <typename T>
		T lerp(T const & lhs, T const & rhs, float s);

		template <typename T>
		T maximize(T const & lhs, T const & rhs);

		template <typename T>
		T minimize(T const & lhs, T const & rhs);

		template <typename T>
		Vector_T<typename T::value_type, 4> transform(T const & v, Matrix4_T<typename T::value_type> const & mat);

		template <typename T>
		T transform_coord(T const & v, Matrix4_T<typename T::value_type> const & mat);

		template <typename T>
		T transform_normal(T const & v, Matrix4_T<typename T::value_type> const & mat);

		template <typename T>
		T bary_centric(T const & v1, T const & v2, T const & v3,
			typename T::value_type const & f, typename T::value_type const & g);

		template <typename T>
		T normalize(T const & rhs);

		template <typename T>
		Plane_T<T> normalize(Plane_T<T> const & rhs);

		template <typename T>
		T reflect(T const & incident, T const & normal);

		template <typename T>
		T refract(T const & incident, T const & normal, typename T::value_type const & refraction_index);

		template <typename T>
		T fresnel_term(T const & cos_theta, T const & refraction_index);


		// 2D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		T cross(Vector_T<T, 2> const & lhs, Vector_T<T, 2> const & rhs);


		// 3D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		T angle(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs);

		template <typename T>
		Vector_T<T, 3> cross(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs);

		template <typename T>
		Vector_T<T, 3> transform_quat(Vector_T<T, 3> const & v, Quaternion_T<T> const & quat);

		template <typename T>
		Vector_T<T, 3> project(Vector_T<T, 3> const & vec,
			Matrix4_T<T> const & world, Matrix4_T<T> const & view, Matrix4_T<T> const & proj,
			int const viewport[4], T const & nearPlane, T const & farPlane);

		template <typename T>
		Vector_T<T, 3> unproject(Vector_T<T, 3> const & winVec, T const & clipW,
			Matrix4_T<T> const & world, Matrix4_T<T> const & view, Matrix4_T<T> const & proj,
			int const viewport[4], T const & nearPlane, T const & farPlane);


		// 4D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Vector_T<T, 4> cross(Vector_T<T, 4> const & v1, Vector_T<T, 4> const & v2, Vector_T<T, 4> const & v3);


		// 4D ����
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Matrix4_T<T> mul(Matrix4_T<T> const & lhs, Matrix4_T<T> const & rhs);

		template <typename T>
		T determinant(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> inverse(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt,
			Vector_T<T, 3> const & vUp = Vector_T<T, 3>(0, 1, 0));

		template <typename T>
		Matrix4_T<T> look_at_rh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt,
			Vector_T<T, 3> const & vUp = Vector_T<T, 3>(0, 1, 0));

		template <typename T>
		Matrix4_T<T> ortho_lh(T const & w, T const & h, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> ortho_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);

		template <typename T>
		Matrix4_T<T> perspective_lh(T const & width, T const & height, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_fov_lh(T const & fov, T const & aspect, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);

		template <typename T>
		Matrix4_T<T> reflect(Plane_T<T> const & p);

		template <typename T>
		Matrix4_T<T> rotation_x(T const & x);
		template <typename T>
		Matrix4_T<T> rotation_y(T const & y);
		template <typename T>
		Matrix4_T<T> rotation_z(T const & z);
		template <typename T>
		Matrix4_T<T> rotation(T const & angle, T const & x, T const & y, T const & z);
		template <typename T>
		Matrix4_T<T> rotation_matrix_yaw_pitch_roll(T const & yaw, T const & pitch, T const & roll);

		template <typename T>
		Matrix4_T<T> scaling(T const & sx, T const & sy, T const & sz);
		template <typename T>
		Matrix4_T<T> scaling(Vector_T<T, 3> const & s);

		template <typename T>
		Matrix4_T<T> shadow(Vector_T<T, 4> const & l, Plane_T<T> const & p);

		template <typename T>
		Matrix4_T<T> to_matrix(Quaternion_T<T> const & quat);

		template <typename T>
		Matrix4_T<T> translation(T const & x, T const & y, T const & z);
		template <typename T>
		Matrix4_T<T> translation(Vector_T<T, 3> const & pos);

		template <typename T>
		Matrix4_T<T> transpose(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> lh_to_rh(Matrix4_T<T> const & rhs);

		template <typename T>
		void decompose(Vector_T<T, 3>& scale, Quaternion_T<T>& rot, Vector_T<T, 3>& trans, Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> transformation(Vector_T<T, 3> const * scaling_center, Quaternion_T<T> const * scaling_rotation, Vector_T<T, 3> const * scale,
			Vector_T<T, 3> const * rotation_center, Quaternion_T<T> const * rotation, Vector_T<T, 3> const * trans);


		template <typename T>
		Matrix4_T<T> ortho_rh(T const & width, T const & height, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> ortho_off_center_rh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_rh(T const & width, T const & height,
			T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_fov_rh(T const & fov, T const & aspect,
			T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_off_center_rh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);

		template <typename T>
		Matrix4_T<T> rh_to_lh(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> rotation_matrix_yaw_pitch_roll(Vector_T<T, 3> const & ang);


		// ��Ԫ��
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Quaternion_T<T> conjugate(Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> axis_to_axis(Vector_T<T, 3> const & from, Vector_T<T, 3> const & to);
		template <typename T>
		Quaternion_T<T> unit_axis_to_unit_axis(Vector_T<T, 3> const & from, Vector_T<T, 3> const & to);

		template <typename T>
		Quaternion_T<T> bary_centric(Quaternion_T<T> const & q1, Quaternion_T<T> const & q2,
			Quaternion_T<T> const & q3, typename Quaternion_T<T>::value_type const & f, typename Quaternion_T<T>::value_type const & g);

		template <typename T>
		Quaternion_T<T> exp(Quaternion_T<T> const & rhs);
		template <typename T>
		Quaternion_T<T> ln(Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> inverse(Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> mul(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> rotation_quat_yaw_pitch_roll(T const & yaw, T const & pitch, T const & roll);

		template <typename T>
		void to_yaw_pitch_roll(T& yaw, T& pitch, T& roll, Quaternion_T<T> const & quat);

		template <typename T>
		void to_axis_angle(Vector_T<T, 3>& vec, T& ang, Quaternion_T<T> const & quat);

		template <typename T>
		Quaternion_T<T> to_quaternion(Matrix4_T<T> const & mat);

		template <typename T>
		Quaternion_T<T> rotation_axis(Vector_T<T, 3> const & v, T const & angle);

		template <typename T>
		Quaternion_T<T> slerp(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs, T const & slerp);

		template <typename T>
		Quaternion_T<T> rotation_quat_yaw_pitch_roll(Vector_T<T, 3> const & ang);


		// ƽ��
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		T dot(Plane_T<T> const & lhs, Vector_T<T, 4> const & rhs);
		template <typename T>
		T dot_coord(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs);
		template <typename T>
		T dot_normal(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs);

		template <typename T>
		Plane_T<T> from_point_normal(Vector_T<T, 3> const & point, Vector_T<T, 3> const & normal);
		template <typename T>
		Plane_T<T> from_points(Vector_T<T, 3> const & v0, Vector_T<T, 3> const & v1, Vector_T<T, 3> const & v2);
		template <typename T>
		Plane_T<T> mul(Plane_T<T> const & p, Matrix4_T<T> const & mat);

		// ��ֱ�ߺ�ƽ��Ľ��㣬ֱ��orig + t * dir
		template <typename T>
		T intersect_ray(Plane_T<T> const & p, Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir);
		
		// From Game Programming Gems 5, Section 2.6.
		template <typename T>
		void oblique_clipping(Matrix4_T<T>& proj, Plane_T<T> const & clip_plane);


		// ��ɫ
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Color_T<T> negative(Color_T<T> const & rhs);
		template <typename T>
		Color_T<T> modulate(Color_T<T> const & lhs, Color_T<T> const & rhs);


		// ��Χ
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		bool vec_in_sphere(Sphere_T<T> const & sphere, Vector_T<T, 3> const & v);

		template <typename T>
		bool intersect_ray(Sphere_T<T> const & sphere, Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir);

		template <typename T>
		bool vec_in_box(AABBox_T<T> const & box, Vector_T<T, 3> const & v);

		template <typename T>
		bool intersect_ray(AABBox_T<T> const & box, Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir);

		template <typename value_type, typename Iterator>
		inline AABBox_T<value_type>
		compute_aabbox(Iterator first, Iterator last)
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
			return AABBox_T<value_type>(minVec, maxVec);
		}

		template <typename value_type, typename Iterator>
		inline OBBox_T<value_type>
		compute_obbox(Iterator first, Iterator last)
		{
			// Compute the mean of the points.
			Vector_T<value_type, 3> center = *first;
			Iterator iter = first;
			++ iter;
			uint32_t n = 1;
			for (; iter != last; ++ iter, ++ n)
			{
				center += *iter;
			}
			value_type inv_num_points = value_type(1) / n;
			center *= inv_num_points;

			// Compute the covariance matrix of the points.
			value_type cov[6];
			for (int i = 0; i < 6; ++ i)
			{
				cov[i] = 0;
			}

			for (Iterator iter = first; iter != last; ++ iter)
			{
				Vector_T<value_type, 3> diff = *iter - center;
				cov[0] += diff[0] * diff[0];
				cov[1] += diff[0] * diff[1];
				cov[2] += diff[0] * diff[2];
				cov[3] += diff[1] * diff[1];
				cov[4] += diff[1] * diff[2];
				cov[5] += diff[2] * diff[2];
			}

			for (int i = 0; i < 6; ++ i)
			{
				cov[i] *= inv_num_points;
			}

			// Tridiagonal

			value_type diagonal[3];
			value_type sub_diagonal[3];
			value_type matrix[3][3];
			bool is_rotation = false;

			value_type m00 = cov[0];
			value_type m01 = cov[1];
			value_type m02 = cov[2];
			value_type m11 = cov[3];
			value_type m12 = cov[4];
			value_type m22 = cov[5];

			diagonal[0] = m00;
			sub_diagonal[2] = 0;
			if (abs(m02) > value_type(1e-6))
			{
				value_type length = sqrt(m01 * m01 + m02 * m02);
				value_type inv_length = 1 / length;
				m01 *= inv_length;
				m02 *= inv_length;
				value_type q = 2 * m01 * m12 + m02 * (m22 - m11);
				diagonal[1] = m11 + m02 * q;
				diagonal[2] = m22 - m02 * q;
				sub_diagonal[0] = length;
				sub_diagonal[1] = m12 - m01 * q;
				matrix[0][0] = 1;
				matrix[0][1] = 0;
				matrix[0][2] = 0;
				matrix[1][0] = 0;
				matrix[1][1] = m01;
				matrix[1][2] = m02;
				matrix[2][0] = 0;
				matrix[2][1] = m02;
				matrix[2][2] = -m01;
				is_rotation = false;
			}
			else
			{
				diagonal[1] = m11;
				diagonal[2] = m22;
				sub_diagonal[0] = m01;
				sub_diagonal[1] = m12;
				matrix[0][0] = 1;
				matrix[0][1] = 0;
				matrix[0][2] = 0;
				matrix[1][0] = 0;
				matrix[1][1] = 1;
				matrix[1][2] = 0;
				matrix[2][0] = 0;
				matrix[2][1] = 0;
				matrix[2][2] = 1;
				is_rotation = true;
			}

			// QLAlgorithm

			int const nIterPower = 32;

			for (int i0 = 0; i0 < 3; ++ i0)
			{
				int i1;
				for (i1 = 0; i1 < nIterPower; ++ i1)
				{
					int i2;
					for (i2 = i0; i2 <= 3 - 2; ++ i2)
					{
						value_type tmp = abs(diagonal[i2]) + abs(diagonal[i2+1]);

						if (abs(sub_diagonal[i2]) + tmp == tmp)
						{
							break;
						}
					}
					if (i2 == i0)
					{
						break;
					}

					value_type value0 = (diagonal[i0 + 1] - diagonal[i0]) / (2 * sub_diagonal[i0]);
					value_type value1 = sqrt(value0 * value0 + 1);
					if (value0 < 0)
					{
						value0 = diagonal[i2] - diagonal[i0] + sub_diagonal[i0] / (value0 - value1);
					}
					else
					{
						value0 = diagonal[i2] - diagonal[i0] + sub_diagonal[i0] / (value0 + value1);
					}

					value_type sn = 1, cs = 1, value2 = 0;
					for (int i3 = i2 - 1; i3 >= i0; -- i3)
					{
						value_type value3 = sn * sub_diagonal[i3];
						value_type value4 = cs * sub_diagonal[i3];
						if (abs(value3) >= abs(value0))
						{
							cs = value0 / value3;
							value1 = sqrt(cs * cs + 1);
							sub_diagonal[i3 + 1] = value3 * value1;
							sn = 1 / value1;
							cs *= sn;
						}
						else
						{
							sn = value3 / value0;
							value1 = sqrt(sn * sn + 1);
							sub_diagonal[i3 + 1] = value0 * value1;
							cs = 1 / value1;
							sn *= cs;
						}
						value0 = diagonal[i3 + 1] - value2;
						value1 = (diagonal[i3] - value0) * sn + 2 * value4 * cs;
						value2 = sn * value1;
						diagonal[i3 + 1] = value0 + value2;
						value0 = cs * value1 - value4;

						for (int i4 = 0; i4 < 3; ++ i4)
						{
							value3 = matrix[i4][i3 + 1];
							matrix[i4][i3 + 1] = sn * matrix[i4][i3] + cs * value3;
							matrix[i4][i3] = cs * matrix[i4][i3] - sn * value3;
						}
					}
					diagonal[i0] -= value2;
					sub_diagonal[i0] = value0;
					sub_diagonal[i2] = 0;
				}
			}

			// IncreasingSort

			// Sort the eigenvalues in increasing order, e[0] <= ... <= e[mSize-1]
			for (int i0 = 0, i1; i0 <= 3 - 2; ++ i0)
			{ 
				// Locate the minimum eigenvalue.
				i1 = i0;
				float min_value = diagonal[i1];
				int i2;
				for (i2 = i0 + 1; i2 < 3; ++ i2)
				{
					if (diagonal[i2] < min_value)
					{
						i1 = i2;
						min_value = diagonal[i1];
					}
				}

				if (i1 != i0)
				{
					// Swap the eigenvalues.
					diagonal[i1] = diagonal[i0];
					diagonal[i0] = min_value;

					// Swap the eigenvectors corresponding to the eigenvalues.
					for (i2 = 0; i2 < 3; ++ i2)
					{
						value_type tmp = matrix[i2][i0];
						matrix[i2][i0] = matrix[i2][i1];
						matrix[i2][i1] = tmp;
						is_rotation = !is_rotation;
					}
				}
			}

			// GuaranteeRotation

			if (!is_rotation)
			{
				// Change sign on the first column.
				for (int row = 0; row < 3; ++ row)
				{
					matrix[row][0] = -matrix[row][0];
				}
			}

			Vector_T<value_type, 3> axis[3];
			Vector_T<value_type, 3> extent;
			for (int i = 0; i < 3; ++ i)
			{
				extent[i] = diagonal[i];
				for (int row = 0; row < 3; ++row)
				{
					axis[i][row] = matrix[row][i];
				}
			}

			// Let C be the box center and let U0, U1, and U2 be the box axes.  Each
			// input point is of the form X = C + y0*U0 + y1*U1 + y2*U2.  The
			// following code computes min(y0), max(y0), min(y1), max(y1), min(y2),
			// and max(y2).  The box center is then adjusted to be
			//   C' = C + 0.5*(min(y0)+max(y0))*U0 + 0.5*(min(y1)+max(y1))*U1 +
			//        0.5*(min(y2)+max(y2))*U2

			Vector_T<value_type, 3> diff = *first - center;
			Vector_T<value_type, 3> pmin(dot(diff, axis[0]), dot(diff, box.Axis[1]), dot(diff, box.Axis[2]));
			Vector_T<value_type, 3> pmax = pmin;
			iter = first;
			++ iter;
			for (; iter != last; ++ iter)
			{
				diff = *iter - center;
				for (int j = 0; j < 3; ++j)
				{
					float d = dot(diff, axis[j]);
					if (d < pmin[j])
					{
						pmin[j] = d;
					}
					else if (d > pmax[j])
					{
						pmax[j] = d;
					}
				}
			}

			center += (value_type(0.5) * (pmin[0] + pmax[0])) * axis[0]
				+ (value_type(0.5) * (pmin[1] + pmax[1])) * axis[1]
				+ (value_type(0.5) * (pmin[2] + pmax[2])) * axis[2];

			extent[0] = value_type(0.5) * (pmax[0] - pmin[0]);
			extent[1] = value_type(0.5) * (pmax[1] - pmin[1]);
			extent[2] = value_type(0.5) * (pmax[2] - pmin[2]);

			return OBBox_T<value_type>(center, axis[0], axis[1], axis[2], extent);
		}

		template <typename value_type, typename Iterator>
		inline Sphere_T<value_type>
		compute_sphere(Iterator first, Iterator last)
		{
			// from Graphics Gems I p301

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
		template <typename TangentIterator, typename BinormIterator,
			typename IndexIterator, typename PositionIterator, typename TexCoordIterator, typename NormalIterator>
		inline void
		compute_tangent(TangentIterator targentsBegin, BinormIterator binormsBegin,
								IndexIterator indicesBegin, IndexIterator indicesEnd,
								PositionIterator xyzsBegin, PositionIterator xyzsEnd,
								TexCoordIterator texsBegin, NormalIterator normalsBegin)
		{
			typedef typename std::iterator_traits<PositionIterator>::value_type position_type;
			typedef typename std::iterator_traits<TexCoordIterator>::value_type texcoord_type;
			typedef typename std::iterator_traits<TangentIterator>::value_type tangent_type;
			typedef typename std::iterator_traits<BinormIterator>::value_type binormal_type;
			typedef typename std::iterator_traits<NormalIterator>::value_type normal_type;
			typedef typename position_type::value_type value_type;

			int const num = static_cast<int>(std::distance(xyzsBegin, xyzsEnd));

			for (int i = 0; i < num; ++ i)
			{
				*(targentsBegin + i) = tangent_type::Zero();
				*(binormsBegin + i) = binormal_type::Zero();
			}

			for (IndexIterator iter = indicesBegin; iter != indicesEnd; iter += 3)
			{
				uint16_t const v0Index = *(iter + 0);
				uint16_t const v1Index = *(iter + 1);
				uint16_t const v2Index = *(iter + 2);

				position_type const & v0XYZ(*(xyzsBegin + v0Index));
				position_type const & v1XYZ(*(xyzsBegin + v1Index));
				position_type const & v2XYZ(*(xyzsBegin + v2Index));

				Vector_T<value_type, 3> v1v0 = v1XYZ - v0XYZ;
				Vector_T<value_type, 3> v2v0 = v2XYZ - v0XYZ;

				texcoord_type const & v0Tex(*(texsBegin + v0Index));
				texcoord_type const & v1Tex(*(texsBegin + v1Index));
				texcoord_type const & v2Tex(*(texsBegin + v2Index));

				value_type s1 = v1Tex.x() - v0Tex.x();
				value_type t1 = v1Tex.y() - v0Tex.y();

				value_type s2 = v2Tex.x() - v0Tex.x();
				value_type t2 = v2Tex.y() - v0Tex.y();

				value_type denominator = s1 * t2 - s2 * t1;
				Vector_T<value_type, 3> tangent, binormal;
				if (MathLib::abs(denominator) < std::numeric_limits<value_type>::epsilon())
				{
					tangent = Vector_T<value_type, 3>(1, 0, 0);
					binormal = Vector_T<value_type, 3>(0, 1, 0);
				}
				else
				{
					tangent = (t2 * v1v0 - t1 * v2v0) / denominator;
					binormal = (s1 * v2v0 - s2 * v1v0) / denominator;
				}

				tangent_type t = Vector_T<value_type, 4>(tangent.x(), tangent.y(), tangent.z(), value_type(1));

				*(targentsBegin + v0Index) += t;
				*(binormsBegin + v0Index) += binormal;

				*(targentsBegin + v1Index) += t;
				*(binormsBegin + v1Index) += binormal;

				*(targentsBegin + v2Index) += t;
				*(binormsBegin + v2Index) += binormal;
			}

			for (int i = 0; i < num; ++ i)
			{
				tangent_type t(*(targentsBegin + i));
				Vector_T<value_type, 3> tangent(t.x(), t.y(), t.z());
				binormal_type binormal(*(binormsBegin + i));
				normal_type normal(*(normalsBegin + i));

				// Gram-Schmidt orthogonalize
				tangent = normalize(tangent - normal * dot(tangent, normal));
				// Calculate handedness
				value_type k = 1;
				if (dot(cross(normal, tangent), binormal) < 0)
				{
					k = -1;
				}

				*(targentsBegin + i) = Vector_T<value_type, 4>(tangent.x(), tangent.y(), tangent.z(), k);
				*(binormsBegin + i) = cross(normal, tangent);
			}
		}

		template <typename NormalIterator, typename IndexIterator, typename PositionIterator>
		inline void
		compute_normal(NormalIterator normalBegin,
								IndexIterator indicesBegin, IndexIterator indicesEnd,
								PositionIterator xyzsBegin, PositionIterator xyzsEnd)
		{
			typedef typename std::iterator_traits<PositionIterator>::value_type position_type;
			typedef typename std::iterator_traits<NormalIterator>::value_type normal_type;
			typedef typename position_type::value_type value_type;

			NormalIterator normalEnd = normalBegin;
			std::advance(normalEnd, std::distance(xyzsBegin, xyzsEnd));
			std::fill(normalBegin, normalEnd, normal_type::Zero());

			for (IndexIterator iter = indicesBegin; iter != indicesEnd; iter += 3)
			{
				uint16_t const v0Index = *(iter + 0);
				uint16_t const v1Index = *(iter + 1);
				uint16_t const v2Index = *(iter + 2);

				position_type const & v0(*(xyzsBegin + v0Index));
				position_type const & v1(*(xyzsBegin + v1Index));
				position_type const & v2(*(xyzsBegin + v2Index));

				Vector_T<value_type, 3> v03(v0.x(), v0.y(), v0.z());
				Vector_T<value_type, 3> v13(v1.x(), v1.y(), v1.z());
				Vector_T<value_type, 3> v23(v2.x(), v2.y(), v2.z());

				Vector_T<value_type, 3> vec(cross(v13 - v03, v23 - v03));

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
		void intersect(Vector_T<T, 3> const & v0, Vector_T<T, 3> const & v1, Vector_T<T, 3> const & v2,
						Vector_T<T, 3> const & ray_orig, Vector_T<T, 3> const & ray_dir,
						T& t, T& u, T& v);

		template <typename T>
		bool bary_centric_in_triangle(T const & u, T const & v);


		// Color space
		///////////////////////////////////////////////////////////////////////////////
		KLAYGE_CORE_API float linear_to_srgb(float linear);
		KLAYGE_CORE_API float srgb_to_linear(float srgb);

		
		// Dual quaternion
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Quaternion_T<T> quat_trans_to_udq(Quaternion_T<T> const & q, Vector_T<T, 3> const & t);

		template <typename T>
		Vector_T<T, 3> udq_to_trans(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		Vector_T<T, 3> dq_to_trans(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		Matrix4_T<T> udq_to_matrix(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > conjugate(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > inverse(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		Quaternion_T<T> mul_real(Quaternion_T<T> const & lhs_real, Quaternion_T<T> const & rhs_real);

		template <typename T>
		Quaternion_T<T> mul_dual(Quaternion_T<T> const & lhs_real, Quaternion_T<T> const & lhs_dual,
			Quaternion_T<T> const & rhs_real, Quaternion_T<T> const & rhs_dual);

		template <typename T>
		void udq_to_screw(T& angle, T& pitch, Vector_T<T, 3>& dir, Vector_T<T, 3>& moment,
			Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > udq_from_screw(T const & angle, T const & pitch,
			Vector_T<T, 3> const & dir, Vector_T<T, 3> const & moment);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > sclerp(Quaternion_T<T> const & lhs_real, Quaternion_T<T> const & lhs_dual,
			Quaternion_T<T> const & rhs_real, Quaternion_T<T> const & rhs_dual, float const & slerp);
	}
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
#include <KlayGE/AABBox.hpp>
#include <KlayGE/Frustum.hpp>
#include <KlayGE/OBBox.hpp>

#endif		// _MATH_HPP
