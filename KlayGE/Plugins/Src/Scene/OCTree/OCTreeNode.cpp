// OCTreeNode.cpp
// KlayGE �˲�������� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ���ν��� (2004.6.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>

#include <KlayGE/OCTree/OCTree.hpp>

#include <iostream>
using namespace std;

namespace KlayGE
{
	OCTreeNode::OCTreeNode(Box const & box)
		: box_(box)
	{
	}

	OCTreeNode::~OCTreeNode()
	{
	}

	void OCTreeNode::Clear()
	{
		renderables_.clear();
		for (ChildrenType::iterator iter = children_.begin(); iter != children_.end(); ++ iter)
		{
			iter->reset();
		}
	}

	bool OCTreeNode::InsideNode(RenderablePtr const & renderable)
	{
		Box box(renderable->GetBound());

		if (MathLib::VecInBox(box_, box.LeftBottomNear())
			&& MathLib::VecInBox(box_, box.LeftTopNear())
			&& MathLib::VecInBox(box_, box.RightBottomNear())
			&& MathLib::VecInBox(box_, box.RightTopNear())
			&& MathLib::VecInBox(box_, box.LeftBottomFar())
			&& MathLib::VecInBox(box_, box.LeftTopFar())
			&& MathLib::VecInBox(box_, box.RightBottomFar())
			&& MathLib::VecInBox(box_, box.RightTopFar()))
		{
			return true;
		}

		return false;
	}

	void OCTreeNode::AddRenderable(RenderablePtr const & renderable)
	{
		if (!renderable->CanBeCulled())
		{
			SceneNode::AddRenderable(renderable);
		}
		else
		{
			if (this->InsideNode(renderable))
			{
				if (children_[0])
				{
					bool inserted(false);
					for (ChildrenType::iterator iter = children_.begin(); iter != children_.end(); ++ iter)
					{
						if ((*iter)->InsideNode(renderable))
						{
							(*iter)->AddRenderable(renderable);
							inserted = true;

							break;
						}
					}

					if (!inserted)
					{
						SceneNode::AddRenderable(renderable);
					}
				}
				else
				{
					if (renderables_.size() > 0)
					{
						Vector3 center((box_.LeftBottomNear() + box_.RightTopFar()) / 2);

						children_[0] = OCTreeNodePtr(new OCTreeNode(Box(box_.LeftBottomNear(), center)));
						children_[1] = OCTreeNodePtr(new OCTreeNode(Box(box_.LeftTopNear(), center)));
						children_[2] = OCTreeNodePtr(new OCTreeNode(Box(box_.RightBottomNear(), center)));
						children_[3] = OCTreeNodePtr(new OCTreeNode(Box(box_.RightTopNear(), center)));
						children_[4] = OCTreeNodePtr(new OCTreeNode(Box(box_.LeftBottomFar(), center)));
						children_[5] = OCTreeNodePtr(new OCTreeNode(Box(box_.LeftTopFar(), center)));
						children_[6] = OCTreeNodePtr(new OCTreeNode(Box(box_.RightBottomFar(), center)));
						children_[7] = OCTreeNodePtr(new OCTreeNode(Box(box_.RightTopFar(), center)));

						for (RenderablesType::iterator i = renderables_.begin(); i != renderables_.end();)
						{
							bool inserted(false);
							for (ChildrenType::iterator iter = children_.begin(); iter != children_.end(); ++ iter)
							{
								if ((*iter)->InsideNode(*i))
								{
									(*iter)->AddRenderable(*i);
									inserted = true;

									break;
								}
							}

							if (inserted)
							{
								i = renderables_.erase(i);
							}
							else
							{
								++ i;
							}
						}
					}
					else
					{
						SceneNode::AddRenderable(renderable);
					}
				}
			}
		}
	}

	void OCTreeNode::Clip(Frustum const & frustum)
	{
		for (ChildrenType::iterator iter = children_.begin(); iter != children_.end(); ++ iter)
		{
			OCTreeNodePtr& child(*iter);

			if (child && (!child->renderables_.empty()))
			{
				Box& box(child->box_);
				if (frustum.Visiable(box.LeftBottomNear())
						|| frustum.Visiable(box.LeftTopNear())
						|| frustum.Visiable(box.RightBottomNear())
						|| frustum.Visiable(box.RightTopNear())
						|| frustum.Visiable(box.LeftBottomFar())
						|| frustum.Visiable(box.LeftTopFar())
						|| frustum.Visiable(box.RightBottomFar())
						|| frustum.Visiable(box.RightTopFar()))
				{
					child->Clip(frustum);
				}
				else
				{
					child.reset();
				}
			}
		}
	}

	void OCTreeNode::GetRenderables(std::vector<RenderablePtr>& renderables)
	{
		renderables.insert(renderables.end(), renderables_.begin(), renderables_.end());

		for (ChildrenType::iterator iter = children_.begin(); iter != children_.end(); ++ iter)
		{
			if (*iter)
			{
				(*iter)->GetRenderables(renderables);
			}
		}
	}
}
