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
	RenderModel::RenderModel(std::wstring const & name)
		: name_(name),
			model_(Matrix4::Identity())
	{
	}

	void RenderModel::AddToSceneManager()
	{
		std::for_each(meshes_.begin(), meshes_.end(), boost::mem_fn(&StaticMesh::AddToSceneManager));
	}

	void RenderModel::OnRenderBegin()
	{
		std::for_each(meshes_.begin(), meshes_.end(), boost::mem_fn(&StaticMesh::OnRenderBegin));
	}

	void RenderModel::OnRenderEnd()
	{
		std::for_each(meshes_.begin(), meshes_.end(), boost::mem_fn(&StaticMesh::OnRenderEnd));
	}


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
				RenderFactory& rf = Context::Instance().RenderFactoryInstance();

				// ������������
				VertexStreamPtr pos_vs = rf.MakeVertexStream(boost::make_tuple(vertex_element(VET_Positions, sizeof(float), 3)), true);
				pos_vs->Assign(&xyzs_[0], static_cast<uint32_t>(xyzs_.size()));
				vb_->AddVertexStream(pos_vs);

				box_ = MathLib::ComputeBoundingBox<float>(xyzs_.begin(), xyzs_.end());

				if (!normals_.empty())
				{
					// ������������
					VertexStreamPtr normal_vs = rf.MakeVertexStream(boost::make_tuple(vertex_element(VET_Normals, sizeof(float), 3)), true);
					normal_vs->Assign(&normals_[0], static_cast<uint32_t>(normals_.size()));
					vb_->AddVertexStream(normal_vs);
				}

				// ������������
				for (size_t i = 0; i < multi_tex_coords_.size(); ++ i)
				{
					VertexStreamPtr tex_vs = rf.MakeVertexStream(boost::make_tuple(vertex_element(static_cast<VertexElementType>(VET_TextureCoords0 + i), sizeof(float), 2)), true);
					tex_vs->Assign(&multi_tex_coords_[i][0], static_cast<uint32_t>(multi_tex_coords_[i].size()));
					vb_->AddVertexStream(tex_vs);
				}

				// ��������
				vb_->SetIndexStream(rf.MakeIndexStream(true));
				vb_->GetIndexStream()->Assign(&indices_[0], static_cast<uint32_t>(indices_.size()));
			}

			beBuilt_ = true;
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
