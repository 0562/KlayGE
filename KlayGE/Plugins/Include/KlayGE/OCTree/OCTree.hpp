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

#define KLAYGE_LIB_NAME KlayGE_Scene_OCTree
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneNode.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Box.hpp>

#include <vector>
#include <map>
#include <string>

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
		void Clear();

		tree_id_t Child(tree_id_t const & id, int child_no);
		Box AreaBox(tree_id_t const & id);
		bool InsideChild(tree_id_t const & id, SceneObjectPtr const & renderable);
		void InsertSceneObject(tree_id_t const & id, SceneObjectPtr const & renderable);

		void DoAddSceneObject(SceneObjectPtr const & obj);
		SceneObjectsType::iterator DoDelSceneObject(SceneObjectsType::iterator iter);

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
