// OCTree.hpp
// KlayGE �˲����� ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2004-2007
// Homepage: http://www.klayge.org
//
// 3.7.0
// �����˱����ٶ� (2007.12.18)
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

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneNode.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/AABBox.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
#include <boost/pool/pool_alloc.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <vector>

namespace KlayGE
{
	class OCTree : public SceneManager
	{
	public:
		OCTree();

		void MaxTreeDepth(uint32_t max_tree_depth);
		uint32_t MaxTreeDepth() const;

		bool AABBVisible(AABBox const & aabb);
		bool OBBVisible(OBBox const & obb);
		bool SphereVisible(Sphere const & sphere);

	private:
		void ClipScene();
		void ClearObject();

		void OnAddSceneObject(SceneObjectPtr const & obj);
		void OnDelSceneObject(SceneObjectsType::iterator iter);

		void NodeVisible(size_t index);
		bool BoundVisible(size_t index, AABBox const & aabb) const;
		bool BoundVisible(size_t index, OBBox const & obb) const;
		bool BoundVisible(size_t index, Sphere const & sphere) const;
		bool BoundVisible(size_t index, Frustum const & frustum) const;

	private:
		OCTree(OCTree const & rhs);
		OCTree& operator=(OCTree const & rhs);

	private:
		struct octree_node_t
		{
			AABBox bb;
			int first_child_index;
			BoundOverlap visible;
		};

		std::vector<octree_node_t, boost::pool_allocator<octree_node_t> > octree_;
		std::vector<size_t, boost::pool_allocator<size_t> > base_address_;

		uint32_t max_tree_depth_;

		bool rebuild_tree_;

#ifdef KLAYGE_DEBUG
		RenderablePtr node_renderable_;
#endif
	};
}

#endif		// _OCTREE_HPP
