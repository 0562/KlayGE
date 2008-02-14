// OCTree.cpp
// KlayGE �˲����� ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2004-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// �����˱����ٶ� (2007.12.18)
//
// 3.0.0
// ��֤�˻���˳�� (2005.8.17)
//
// 2.6.0
// ������CanBeCulled��bug (2005.5.26)
//
// 2.4.0
// �������԰˲��� (2005.3.20)
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Vector.hpp>
#include <KlayGE/Plane.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/SceneObject.hpp>
#include <KlayGE/RenderableHelper.hpp>

#include <algorithm>
#include <functional>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <KlayGE/OCTree/OCTree.hpp>

namespace KlayGE
{
	OCTree::OCTree(uint32_t max_tree_depth)
		: max_tree_depth_(std::min<uint32_t>(max_tree_depth, 16UL)),
			rebuild_tree_(false)
	{
	}

	void OCTree::ClipScene(Camera const & camera)
	{
		if (rebuild_tree_)
		{
			typedef std::vector<size_t> ObjIndicesTypes;
			std::vector<ObjIndicesTypes> obj_indices(1);
			octree_.resize(1);
			Box bb_root(float3(0, 0, 0), float3(0, 0, 0));
			octree_[0].first_child_index = -1;
			std::vector<float3> aabbs_center_in_ws(scene_objs_.size());
			std::vector<float3> aabbs_half_size_in_ws(scene_objs_.size());
			for (size_t i = 0; i < scene_objs_.size(); ++ i)
			{
				SceneObjectPtr const & obj = scene_objs_[i];
				if (obj->Cullable() && !obj->ShortAge() && !obj->Moveable())
				{
					Box const & box = obj->GetBound();
					float4x4 const & mat = obj->GetModelMatrix();

					float3 min, max;
					min = max = MathLib::transform_coord(box[0], mat);
					for (size_t j = 1; j < 8; ++ j)
					{
						float3 vec = MathLib::transform_coord(box[j], mat);
						min = MathLib::minimize(min, vec);
						max = MathLib::maximize(max, vec);
					}

					Box aabb_in_ws(min, max);

					bb_root |= aabb_in_ws;
					obj_indices[0].push_back(i);

					aabbs_center_in_ws[i] = aabb_in_ws.Center();
					aabbs_half_size_in_ws[i] = aabb_in_ws.HalfSize();
				}
			}
			{
				float3 const & size = bb_root.Max() - bb_root.Min();
				float max_dim = std::max(std::max(size.x(), size.y()), size.z()) / 2;
				octree_[0].bb_center = bb_root.Center();
				octree_[0].bb_half_size = float3(max_dim, max_dim, max_dim);
			}
			base_address_.push_back(0);
			base_address_.push_back(1);

			for (uint32_t d = 1; d <= max_tree_depth_; ++ d)
			{
				size_t const original_size = octree_.size();
				for (size_t i = base_address_[d - 1]; i < base_address_[d]; ++ i)
				{
					if (obj_indices[i].size() > 1)
					{
						float3 const parent_center = octree_[i].bb_center;
						float3 const new_half_size = octree_[i].bb_half_size / 2;
						octree_[i].first_child_index = static_cast<int>(base_address_[d] + octree_.size() - original_size);

						for (size_t j = 0; j < 8; ++ j)
						{
							octree_.push_back(octree_node_t());
							octree_node_t& new_node = octree_.back();
							new_node.first_child_index = -1;
							new_node.bb_half_size = new_half_size;
							obj_indices.push_back(ObjIndicesTypes());
							ObjIndicesTypes& new_node_obj_indices = obj_indices.back();
							ObjIndicesTypes& parent_obj_indices = obj_indices[i];

							if (j & 1)
							{
								new_node.bb_center.x() = parent_center.x() + new_half_size.x();
							}
							else
							{
								new_node.bb_center.x() = parent_center.x() - new_half_size.x();
							}
							if (j & 2)
							{
								new_node.bb_center.y() = parent_center.y() + new_half_size.y();
							}
							else
							{
								new_node.bb_center.y() = parent_center.y() - new_half_size.y();
							}
							if (j & 4)
							{
								new_node.bb_center.z() = parent_center.z() + new_half_size.z();
							}
							else
							{
								new_node.bb_center.z() = parent_center.z() - new_half_size.z();
							}

							BOOST_FOREACH(size_t obj_index, parent_obj_indices)
							{
								float3 const t = aabbs_center_in_ws[obj_index] - new_node.bb_center;
								float3 const e = aabbs_half_size_in_ws[obj_index] + new_node.bb_half_size;
								if ((abs(t.x()) <= e.x()) && (abs(t.y()) <= e.y()) && (abs(t.y()) <= e.y()))
								{
									new_node_obj_indices.push_back(obj_index);
								}
							}
						}

						obj_indices[i].clear();
					}
				}

				base_address_.push_back(base_address_.back() + octree_.size() - original_size);
			}

			rebuild_tree_ = false;
		}

		if (!octree_.empty())
		{
			Frustum frustum(camera.ViewMatrix() * camera.ProjMatrix());
			this->NodeVisible(0, frustum);
		}

		visible_marks_.resize(scene_objs_.size());
		for (size_t i = start_index_; i < scene_objs_.size(); ++ i)
		{
			SceneObjectPtr const & obj = scene_objs_[i];
			if (obj->Visible())
			{
				if (obj->Cullable() && !obj->ShortAge())
				{
					Box const & box = obj->GetBound();
					float4x4 const & mat = obj->GetModelMatrix();

					float3 min, max;
					min = max = MathLib::transform_coord(box[0], mat);
					for (size_t j = 1; j < 8; ++ j)
					{
						float3 vec = MathLib::transform_coord(box[j], mat);
						min = MathLib::minimize(min, vec);
						max = MathLib::maximize(max, vec);
					}

					visible_marks_[i] = this->BBVisible(0, (max + min) / 2, (max - min) / 2);
				}
				else
				{
					visible_marks_[i] = 1;
				}
			}
			else
			{
				visible_marks_[i] = 0;
			}
		}
	}

	void OCTree::Clear()
	{
		scene_objs_.resize(0);
		octree_.clear();
		rebuild_tree_ = true;
	}

	void OCTree::DoAddSceneObject(SceneObjectPtr const & obj)
	{
		scene_objs_.push_back(obj);
		if (obj->Cullable() && !obj->ShortAge() && !obj->Moveable())
		{
			rebuild_tree_ = true;
		}
	}

	SceneManager::SceneObjectsType::iterator OCTree::DoDelSceneObject(SceneManager::SceneObjectsType::iterator iter)
	{
		if ((*iter)->Cullable() && !(*iter)->ShortAge() && !(*iter)->Moveable())
		{
			rebuild_tree_ = true;
		}
		return scene_objs_.erase(iter);
	}

	void OCTree::NodeVisible(size_t index, Frustum const & frustum)
	{
		assert(index < octree_.size());

		octree_node_t& node = octree_[index];
		Frustum::VIS const vis = frustum.Visiable(Box(node.bb_center - node.bb_half_size, node.bb_center + node.bb_half_size));
		node.visible = vis;
		if (Frustum::VIS_PART == vis)
		{
			if (node.first_child_index != -1)
			{
				for (int i = 0; i < 8; ++ i)
				{
					this->NodeVisible(node.first_child_index + i, frustum);
				}
			}
		}

#ifdef KLAYGE_DEBUG
		if ((vis != Frustum::VIS_NO) && (-1 == node.first_child_index))
		{
			RenderablePtr box_helper(new RenderableLineBox(Box(node.bb_center - node.bb_half_size, node.bb_center + node.bb_half_size), Color(1, 1, 1, 1)));
			box_helper->AddToRenderQueue();
		}
#endif
	}

	bool OCTree::BBVisible(size_t index, float3 const & bb_center, float3 const & bb_half_size)
	{
		assert(index < octree_.size());

		octree_node_t const & node = octree_[index];
		float3 const t = bb_center - node.bb_center;
		float3 const e = bb_half_size + node.bb_half_size;
		if ((abs(t.x()) <= e.x()) && (abs(t.y()) <= e.y()) && (abs(t.y()) <= e.y()))
		{
			Frustum::VIS const vis = node.visible;
			switch (vis)
			{
			case Frustum::VIS_YES:
				return true;

			case Frustum::VIS_NO:
				return false;

			case Frustum::VIS_PART:
				{
					if (node.first_child_index != -1)
					{
						for (int i = node.first_child_index, i_end = node.first_child_index + 8; i < i_end; ++ i)
						{
							if (this->BBVisible(i, bb_center, bb_half_size))
							{
								return true;
							}
						}
						return false;
					}
					else
					{
						return true;
					}
				}
				break;

			default:
				BOOST_ASSERT(false);
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}
