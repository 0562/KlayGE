// Frustum.cpp
// KlayGE ��׶�� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ��ΪLUTʵ�� (2005.3.30)
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <KlayGE/OCTree/Frustum.hpp>

namespace KlayGE
{
	Frustum::Frustum(float4x4 const & clip)
	{
		float4 const & column1(clip.Col(0));
		float4 const & column2(clip.Col(1));
		float4 const & column3(clip.Col(2));
		float4 const & column4(clip.Col(3));

		planes_[0] = column4 - column1;  // left
		planes_[1] = column4 + column1;  // right
		planes_[2] = column4 - column2;  // bottom
		planes_[3] = column4 + column2;  // top
		planes_[4] = column4 - column3;  // near
		planes_[5] = column4 + column3;  // far

		// Loop through each side of the frustum and normalize it.
		BOOST_FOREACH(BOOST_TYPEOF(planes_)::reference plane, planes_)
		{
			plane = MathLib::normalize(plane);
		}

		//  build a bit-field that will tell us the indices for the nearest and farthest vertices from each plane...
		for (int i = 0; i < 6; ++ i)
		{
			vertex_lut_[i] = ((planes_[i].a() < 0) ? 1 : 0) | ((planes_[i].b() < 0) ? 2 : 0) | ((planes_[i].c() < 0) ? 4 : 0);
		}
	}

	Frustum::VIS Frustum::Visiable(Box const & box) const
	{
		bool intersect = false;
		for (int i = 0; i < 6; ++ i)
		{
			int const n = vertex_lut_[i];

			// v1 is diagonally opposed to v0
			float3 v0((n & 1) ? box.Min().x() : box.Max().x(), (n & 2) ? box.Min().y() : box.Max().y(), (n & 4) ? box.Min().z() : box.Max().z());
			float3 v1((n & 1) ? box.Max().x() : box.Min().x(), (n & 2) ? box.Max().y() : box.Min().y(), (n & 4) ? box.Max().z() : box.Min().z());

			if (MathLib::dot_coord(planes_[i], v0) < 0)
			{
				return VIS_NO;
			}
			if (MathLib::dot_coord(planes_[i], v1) < 0)
			{
				intersect = true;
			}
		}

		return intersect ? VIS_PART : VIS_YES;
	}
}
