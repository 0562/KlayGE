// Frustum.hpp
// KlayGE ��׶�� ͷ�ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.3.0
// ���ν��� (2005.2.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _FRUSTUM_HPP
#define _FRUSTUM_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Box.hpp>
#include <KlayGE/Plane.hpp>

#include <vector>
#include <boost/array.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Scene_Frustum_d.lib")
#else
	#pragma comment(lib, "KlayGE_Scene_Frustum.lib")
#endif

namespace KlayGE
{
	class Frustum : public SceneManager
	{
	public:
		Frustum();

		void ClipScene(Camera const & camera);

	private:
		void CalculateFrustum(Matrix4 const & clip);
		bool Visiable(Vector3 const & v) const;

	private:
		typedef boost::array<Plane, 6> PlanesType;
		PlanesType planes_;

	private:
		Frustum(Frustum const & rhs);
		Frustum& operator=(Frustum const & rhs);
	};
}

#endif		// _FRUSTUM_HPP
