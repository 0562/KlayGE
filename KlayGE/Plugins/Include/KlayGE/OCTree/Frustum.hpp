// Frustum.hpp
// KlayGE �˲�����׶�� ͷ�ļ�
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

#ifndef _FRUSTUM_HPP
#define _FRUSTUM_HPP

#include <KlayGE/Math.hpp>
#include <boost/array.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Scene_OCTree_d.lib")
#else
	#pragma comment(lib, "KlayGE_Scene_OCTree.lib")
#endif

namespace KlayGE
{
	class Frustum
	{
	public:
		enum VIS
		{
			VIS_YES,
			VIS_NO,
			VIS_PART,
		};

	public:
		explicit Frustum(float4x4 const & clip);

		VIS Visiable(Box const & box) const;

	private:
		typedef boost::array<Plane, 6> planes_t;
		planes_t planes_;

		// Look-Up Table
		typedef boost::array<int, 6> vertex_lut_t;
		vertex_lut_t vertex_lut_;
	};
}

#endif			// _FRUSTUM_HPP
