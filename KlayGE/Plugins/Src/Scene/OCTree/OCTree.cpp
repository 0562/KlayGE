// OCTree.cpp
// KlayGE �˲����� ʵ���ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/RenderableHelper.hpp>

#include <set>

#include <KlayGE/OCTree/OCTree.hpp>

namespace KlayGE
{
	OCTree::OCTree(Box const & box)
		: root_box_(box)
	{
	}

	void OCTree::ClipScene(Camera const & camera)
	{
		frustum_.CalculateFrustum(camera.ViewMatrix() * camera.ProjMatrix());

#ifdef KLAYGE_DEBUG
		std::vector<RenderablePtr> renderBoxes;
#endif

		std::set<RenderablePtr> renderables;
		for (linear_octree_t::iterator iter = linear_octree_.begin(); iter != linear_octree_.end(); ++ iter)
		{
			Box box = this->AreaBox(iter->first);

			for (size_t i = 0; i < 8; ++ i)
			{
				if (frustum_.Visiable(box[i]))
				{
					renderables.insert(iter->second);
					break;
				}
			}

#ifdef KLAYGE_DEBUG
			renderBoxes.push_back(RenderablePtr(new RenderableBox(box)));
#endif
		}

		renderQueue_.clear();

		for (std::vector<RenderablePtr>::iterator iter = uncullables_.begin();
			iter != uncullables_.end(); ++ iter)
		{
			SceneManager::PushRenderable(*iter);
		}

#ifdef KLAYGE_DEBUG
		for (std::vector<RenderablePtr>::iterator iter = renderBoxes.begin();
			iter != renderBoxes.end(); ++ iter)
		{
			SceneManager::PushRenderable(*iter);
		}
#endif

		for (std::set<RenderablePtr>::iterator iter = renderables.begin();
			iter != renderables.end(); ++ iter)
		{
			SceneManager::PushRenderable(*iter);
		}

		uncullables_.clear();
		linear_octree_.clear();
	}

	void OCTree::PushRenderable(RenderablePtr const & renderable)
	{
		if (!renderable->CanBeCulled())
		{
			uncullables_.push_back(renderable);
		}
		else
		{
			std::vector<tree_id_t> ids;
			ids.push_back("0");
			for (linear_octree_t::iterator iter = linear_octree_.begin(); iter != linear_octree_.end(); ++ iter)
			{
				if (iter->first.length() >= ids[0].length())
				{
					if (iter->first.length() > ids[0].length())
					{
						ids.clear();
					}

					ids.push_back(iter->first);
				}
			}

			for (std::vector<tree_id_t>::iterator iter = ids.begin(); iter != ids.end(); ++ iter)
			{
				for (int i = 0; i < 8; ++ i)
				{
					tree_id_t child_id = this->Child(*iter, i);
					if (this->InsideChild(child_id, renderable))
					{
						assert(!linear_octree_[child_id]);

						linear_octree_[child_id] = renderable;
					}
				}
			}
		}
	}

	OCTree::tree_id_t OCTree::Child(tree_id_t const & id, int child_no)
	{
		assert(child_no >= 0);
		assert(child_no < 8);

		tree_id_t child_id = id;
		child_id.push_back(static_cast<char>(child_no + '0'));

		return child_id;
	}

	Box OCTree::AreaBox(tree_id_t const & id)
	{
		Box ret = root_box_;

		for (tree_id_t::const_iterator iter = id.begin() + 1; iter != id.end(); ++ iter)
		{
			Vector3 const & min = ret.Min();
			Vector3 const & max = ret.Max();
			Vector3 const & center = ret.Center();

			switch (*iter - '0')
			{
			case 0:
				ret = Box(min, center);
				break;

			case 1:
				ret = Box(Vector3(center.x(), min.y(), min.z()),
					Vector3(max.x(), center.y(), center.z()));
				break;

			case 2:
				ret = Box(Vector3(min.x(), center.y(), min.z()),
					Vector3(center.x(), max.y(), center.z()));
				break;

			case 3:
				ret = Box(Vector3(center.x(), center.y(), min.z()),
					Vector3(max.x(), max.y(), center.z()));
				break;

			case 4:
				ret = Box(Vector3(min.x(), min.y(), center.z()),
					Vector3(center.x(), center.y(), max.z()));
				break;

			case 5:
				ret = Box(Vector3(center.x(), min.y(), center.z()),
					Vector3(max.x(), center.y(), max.z()));
				break;

			case 6:
				ret = Box(Vector3(min.x(), center.y(), center.z()),
					Vector3(center.x(), max.y(), max.z()));
				break;

			case 7:
				ret = Box(center, max);
				break;

			default:
				assert(false);
				break;
			}
		}

		return ret;
	}

	bool OCTree::InsideChild(tree_id_t const & id, RenderablePtr renderable)
	{
		Box area_box = this->AreaBox(id);
		Box const & box(renderable->GetBound());

		for (size_t i = 0; i < 8; ++ i)
		{
			Vector3 vec;
			MathLib::TransformCoord(vec, box[i], renderable->GetWorld());
			if (MathLib::VecInBox(area_box, vec))
			{
				return true;
			}
		}
		return false;
	}
}
