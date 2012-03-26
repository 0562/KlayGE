// Frustum.hpp
// KlayGE ��׶�� ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2004-2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ����Core�� (2010.5.23)
//
// 2.5.0
// ��ΪLUTʵ�� (2005.3.30)
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _FRUSTUM_HPP
#define _FRUSTUM_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 6385)
#endif
#include <boost/array.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

namespace KlayGE
{
	template <typename T>
	class KLAYGE_CORE_API Frustum_T : public Bound_T<T>
	{
	public:
		void ClipMatrix(Matrix4_T<T> const & clip);

		bool IsEmpty() const;

		bool VecInBound(Vector_T<T, 3> const & v) const;
		float MaxRadiusSq() const;

		Plane_T<T> const & FrustumPlane(uint32_t index) const;

		BoundOverlap CollisionDet(AABBox_T<T> const & aabb) const;
		BoundOverlap CollisionDet(OBBox_T<T> const & obb) const;
		BoundOverlap CollisionDet(Sphere_T<T> const & sphere) const;
		BoundOverlap CollisionDet(Frustum_T<T> const & frustum) const;

	private:
		typedef boost::array<Plane_T<T>, 6> planes_t;
		planes_t planes_;

		// Look-Up Table
		typedef boost::array<int, 6> vertex_lut_t;
		vertex_lut_t vertex_lut_;
	};

	typedef Frustum_T<float> Frustum;
}

#endif			// _FRUSTUM_HPP
