// Frustum.cpp
// KlayGE ��׶�� ʵ���ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.3.0
// ���ν��� (2005.2.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Camera.hpp>

#include <KlayGE/Frustum/Frustum.hpp>

namespace KlayGE
{
	Frustum::Frustum()
	{
	}

	void Frustum::ClipScene(Camera const & camera)
	{
		this->CalculateFrustum(camera.ViewMatrix() * camera.ProjMatrix());

		for (RenderQueueType::iterator queueIter = renderQueue_.begin();
			queueIter != renderQueue_.end(); ++ queueIter)
		{
			for (RenderItemsType::iterator itemIter = queueIter->second.begin();
				itemIter != queueIter->second.end();)
			{
				Renderable& obj(*(*itemIter));

				bool should_be_culled = true;
			
				if (obj.CanBeCulled())
				{
					Box const & box(obj.GetBound());
					boost::array<Vector3, 8> vecs;
					vecs[0] = box.LeftBottomNear();
					vecs[1] = box.LeftTopNear();
					vecs[2] = box.RightBottomNear();
					vecs[3] = box.RightTopNear();
					vecs[4] = box.LeftBottomFar();
					vecs[5] = box.LeftTopFar();
					vecs[6] = box.RightBottomFar();
					vecs[7] = box.RightTopFar();

					for (size_t i = 0; i < vecs.size(); ++ i)
					{
						MathLib::TransformCoord(vecs[i], vecs[i], obj.GetWorld());
						if (this->Visiable(vecs[i]))
						{
							should_be_culled = false;
						}
					}
				}
				else
				{
					should_be_culled = false;
				}

				if (should_be_culled)
				{
					itemIter = queueIter->second.erase(itemIter);
				}
				else
				{
					++ itemIter;
				}
			}
		}
	}

	void Frustum::CalculateFrustum(Matrix4 const & clip)
	{
		Vector4 column1(clip(0, 0), clip(1, 0), clip(2, 0), clip(3, 0));
		Vector4 column2(clip(0, 1), clip(1, 1), clip(2, 1), clip(3, 1));
		Vector4 column3(clip(0, 2), clip(1, 2), clip(2, 2), clip(3, 2));
		Vector4 column4(clip(0, 3), clip(1, 3), clip(2, 3), clip(3, 3));

		planes_[0] = column4 - column1;  // left
		planes_[1] = column4 + column1;  // right
		planes_[2] = column4 - column2;  // bottom
		planes_[3] = column4 + column2;  // top
		planes_[4] = column4 - column3;  // near
		planes_[5] = column4 + column3;  // far

		// Loop through each side of the frustum and normalize it.
		for (PlanesType::iterator iter = planes_.begin(); iter != planes_.end(); ++ iter)
		{
			MathLib::Normalize(*iter, *iter);
		}
	}

	bool Frustum::Visiable(Vector3 const & v) const
	{
		for (PlanesType::const_iterator iter = planes_.begin(); iter != planes_.end(); ++ iter)
		{
			if (MathLib::DotCoord(*iter, v) < 0)
			{
				return false;
			}
		}

		return true;
	}
}
