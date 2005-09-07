// Mesh.cpp
// KlayGE Mesh�� ʵ���ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// �Ľ���StaticMesh (2005.6.16)
//
// 2.1.2
// ���ν��� (2004.5.26)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <algorithm>
#include <boost/mem_fn.hpp>

#include <KlayGE/Mesh.hpp>

namespace KlayGE
{
	StaticMesh::StaticMesh(std::wstring const & name)
		: name_(name),
			beBuilt_(false),
			model_(Matrix4::Identity())
	{
		vb_ = Context::Instance().RenderFactoryInstance().MakeVertexBuffer(VertexBuffer::BT_TriangleList);
	}

	StaticMesh::~StaticMesh()
	{
	}

	std::wstring const & StaticMesh::Name() const
	{
		return name_;
	}

	void StaticMesh::AddToSceneManager()
	{
		this->BuildRenderable();

		std::for_each(children_.begin(), children_.end(), boost::mem_fn(&StaticMesh::AddToSceneManager));

		if (!xyzs_.empty())
		{
			Renderable::AddToSceneManager();
		}
	}

	void StaticMesh::ComputeNormal()
	{
		normals_.resize(xyzs_.size());
		MathLib::ComputeNormal<float>(normals_.begin(),
			indices_.begin(), indices_.end(), xyzs_.begin(), xyzs_.end());

		std::for_each(children_.begin(), children_.end(), boost::mem_fn(&StaticMesh::BuildRenderable));
	}

	void StaticMesh::OnRenderBegin()
	{
		std::for_each(children_.begin(), children_.end(), boost::mem_fn(&StaticMesh::OnRenderBegin));
	}

	void StaticMesh::OnRenderEnd()
	{
		std::for_each(children_.begin(), children_.end(), boost::mem_fn(&StaticMesh::OnRenderEnd));
	}

	Box StaticMesh::GetBound() const
	{
		return box_;
	}

	void StaticMesh::SetModelMatrix(Matrix4 const & mat)
	{
		model_ = mat;
	}

	void StaticMesh::BuildRenderable()
	{
		if (!beBuilt_)
		{
			if (!xyzs_.empty())
			{
				// ������������
				vb_->AddVertexStream(VST_Positions, sizeof(float), 3, true);
				vb_->GetVertexStream(VST_Positions)->Assign(&xyzs_[0], xyzs_.size());

				box_ = MathLib::ComputeBoundingBox<float>(xyzs_.begin(), xyzs_.end());

				if (!normals_.empty())
				{
					// ������������
					vb_->AddVertexStream(VST_Normals, sizeof(float), 3, true);
					vb_->GetVertexStream(VST_Normals)->Assign(&normals_[0], normals_.size());
				}

				// ������������
				for (size_t i = 0; i < multi_tex_coords_.size(); ++ i)
				{
					vb_->AddVertexStream(static_cast<VertexStreamType>(VST_TextureCoords0 + i), sizeof(float), 2, true);
					vb_->GetVertexStream(static_cast<VertexStreamType>(VST_TextureCoords0 + i))->Assign(&multi_tex_coords_[i][0],
						multi_tex_coords_[i].size());
				}

				// ��������
				vb_->AddIndexStream(true);
				vb_->GetIndexStream()->Assign(&indices_[0], indices_.size());
			}

			beBuilt_ = true;

			std::for_each(children_.begin(), children_.end(), boost::mem_fn(&StaticMesh::BuildRenderable));
		}
	}


	BoneMesh::~BoneMesh()
	{
	}

	std::wstring const & BoneMesh::Name() const
	{
		static std::wstring name(L"Bone Mesh");
		return name;
	}
}
