// OCTree.hpp
// KlayGE �˲����� ͷ�ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// �����˲ü�Ч�� (2005.3.30)
//
// 2.4.0
// �������԰˲��� (2005.3.20)
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OCTREE_HPP
#define _OCTREE_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneNode.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Box.hpp>

#include <vector>
#include <map>
#include <string>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Scene_OCTree_d.lib")
#else
	#pragma comment(lib, "KlayGE_Scene_OCTree.lib")
#endif

namespace KlayGE
{
	class OCTree : public SceneManager
	{
	public:
		typedef std::string tree_id_t;

	public:
		OCTree(Box const & box, uint32_t maxNumObjInANode);

	private:
		void ClipScene(Camera const & camera);

		tree_id_t Child(tree_id_t const & id, int child_no);
		Box AreaBox(tree_id_t const & id);
		bool InsideChild(tree_id_t const & id, SceneObjectPtr const & renderable);
		void InsertSceneObject(tree_id_t const & id, SceneObjectPtr const & renderable);

	private:
		OCTree(OCTree const & rhs);
		OCTree& operator=(OCTree const & rhs);

	private:
		typedef std::map<tree_id_t, SceneObjectsType> linear_octree_t;
		linear_octree_t octree_;
		Box root_box_;

		uint32_t maxNumObjInANode_;
	};
}

#endif		// _OCTREE_HPP
