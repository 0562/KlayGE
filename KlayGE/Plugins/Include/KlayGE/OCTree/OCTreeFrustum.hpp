// OctreeFrustum.hpp
// KlayGE �˲�����׶�� ͷ�ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OCTREEFRUSTUM_HPP
#define _OCTREEFRUSTUM_HPP

#include <KlayGE/Math.hpp>
#include <boost/array.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Scene_OCTree_d.lib")
#else
	#pragma comment(lib, "KlayGE_Scene_OCTree.lib")
#endif

namespace KlayGE
{
	class OCTreeFrustum
	{
	public:
		explicit OCTreeFrustum(Matrix4 const & clip);

		bool Visiable(Box const & box, Matrix4 const & model) const;
		bool Visiable(Vector3 const & v) const;

	private:
		typedef boost::array<Plane, 6> planes_t;
		planes_t planes_;

		typedef std::pair<Vector3, Vector3> line_t;
		boost::array<line_t, 4> frustumLines_;
	};
}

#endif			// _OCTREEFRUSTUM_HPP
