#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/App3D.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Camera.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include "Model.hpp"

using namespace KlayGE;


MD5SkinnedMesh::MD5SkinnedMesh(RenderModelPtr model, std::wstring const & /*name*/)
	: SkinnedMesh(model, L"MD5SkinnedMesh"),
		world_(float4x4::Identity()),
			effect_(Context::Instance().RenderFactoryInstance().LoadEffect("SkinnedMesh.kfx"))
{
}

void MD5SkinnedMesh::BuildMeshInfo()
{
	RenderFactory& rf = Context::Instance().RenderFactoryInstance();

	std::vector<float3> positions(this->NumVertices());
	std::vector<float2> texcoords(this->NumVertices());
	std::vector<float3> normals;
	std::vector<float3> tangents;
	std::vector<float3> binormals;
	for (uint32_t i = 0; i < rl_->NumVertexStreams(); ++ i)
	{
		GraphicsBufferPtr vb = rl_->GetVertexStream(i);
		switch (rl_->VertexStreamFormat(i)[0].usage)
		{
		case VEU_Position:
			{
				GraphicsBufferPtr vb_cpu = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Read | EAH_CPU_Write, NULL);
				vb_cpu->Resize(vb->Size());
				vb->CopyToBuffer(*vb_cpu);

				{
					GraphicsBuffer::Mapper mapper(*vb_cpu, BA_Read_Write);
					std::copy(mapper.Pointer<float3>(), mapper.Pointer<float3>() + positions.size(), positions.begin());

					BOOST_FOREACH(BOOST_TYPEOF(positions)::reference pos, positions)
					{
						std::swap(pos.y(), pos.z());
						pos.z() = -pos.z();
					}

					std::copy(positions.begin(), positions.end(), mapper.Pointer<float3>());
				}

				vb_cpu->CopyToBuffer(*vb);
			}
			break;

		case VEU_TextureCoord:
			{
				GraphicsBufferPtr vb_cpu = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Read, NULL);
				vb_cpu->Resize(vb->Size());
				vb->CopyToBuffer(*vb_cpu);

				GraphicsBuffer::Mapper mapper(*vb_cpu, BA_Read_Only);
				std::copy(mapper.Pointer<float2>(), mapper.Pointer<float2>() + texcoords.size(), texcoords.begin());
			}
			break;

		case VEU_Normal:
			normals.resize(this->NumVertices());
			{
				GraphicsBufferPtr vb_cpu = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Read, NULL);
				vb_cpu->Resize(vb->Size());
				vb->CopyToBuffer(*vb_cpu);

				{
					GraphicsBuffer::Mapper mapper(*vb_cpu, BA_Read_Only);
					std::copy(mapper.Pointer<float3>(), mapper.Pointer<float3>() + normals.size(), normals.begin());

					BOOST_FOREACH(BOOST_TYPEOF(normals)::reference normal, normals)
					{
						std::swap(normal.y(), normal.z());
						normal.z() = -normal.z();
					}

					std::copy(normals.begin(), normals.end(), mapper.Pointer<float3>());
				}

				vb_cpu->CopyToBuffer(*vb);
			}
			break;

		case VEU_Tangent:
			tangents.resize(this->NumVertices());
			{
				GraphicsBufferPtr vb_cpu = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Read, NULL);
				vb_cpu->Resize(vb->Size());
				vb->CopyToBuffer(*vb_cpu);

				{
					GraphicsBuffer::Mapper mapper(*vb_cpu, BA_Read_Only);
					std::copy(mapper.Pointer<float3>(), mapper.Pointer<float3>() + tangents.size(), tangents.begin());

					
					BOOST_FOREACH(BOOST_TYPEOF(tangents)::reference tangent, tangents)
					{
						std::swap(tangent.y(), tangent.z());
						tangent.z() = -tangent.z();
					}

					std::copy(tangents.begin(), tangents.end(), mapper.Pointer<float3>());
				}

				vb_cpu->CopyToBuffer(*vb);
			}
			break;

		case VEU_Binormal:
			binormals.resize(this->NumVertices());
			{
				GraphicsBufferPtr vb_cpu = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Read, NULL);
				vb_cpu->Resize(vb->Size());
				vb->CopyToBuffer(*vb_cpu);

				{
					GraphicsBuffer::Mapper mapper(*vb_cpu, BA_Read_Only);
					std::copy(mapper.Pointer<float3>(), mapper.Pointer<float3>() + binormals.size(), binormals.begin());

					BOOST_FOREACH(BOOST_TYPEOF(binormals)::reference binormal, binormals)
					{
						std::swap(binormal.y(), binormal.z());
						binormal.z() = -binormal.z();
					}

					std::copy(binormals.begin(), binormals.end(), mapper.Pointer<float3>());
				}

				vb_cpu->CopyToBuffer(*vb);
			}
			break;

		default:
			break;
		}
	}
	std::vector<uint16_t> indices(this->NumTriangles() * 3);
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();

		GraphicsBufferPtr ib = rl_->GetIndexStream();
		GraphicsBufferPtr ib_cpu = rf.MakeIndexBuffer(BU_Static, EAH_CPU_Read, NULL);
		ib_cpu->Resize(ib->Size());
		ib->CopyToBuffer(*ib_cpu);

		GraphicsBuffer::Mapper mapper(*ib_cpu, BA_Read_Only);
		std::copy(mapper.Pointer<uint16_t>(), mapper.Pointer<uint16_t>() + indices.size(), indices.begin());
	}

	if (normals.empty())
	{
		normals.resize(this->NumVertices());

		if (tangents.empty() && binormals.empty())
		{
			MathLib::compute_normal<float>(normals.begin(),
				indices.begin(), indices.end(), positions.begin(), positions.end());
		}
		else
		{
			for (size_t i = 0; i < normals.size(); ++ i)
			{
				normals[i] = MathLib::cross(tangents[i], binormals[i]);
			}
		}
	}
	this->AddVertexStream(&normals[0], static_cast<uint32_t>(sizeof(normals[0]) * normals.size()),
			vertex_element(VEU_Normal, 0, EF_BGR32F), EAH_GPU_Read);

	if (tangents.empty())
	{
		tangents.resize(this->NumVertices());
		binormals.resize(this->NumVertices());

		// ����TBN
		MathLib::compute_tangent<float>(tangents.begin(), binormals.begin(),
			indices.begin(), indices.end(),
			positions.begin(), positions.end(),
			texcoords.begin(), normals.begin());
		this->AddVertexStream(&tangents[0], static_cast<uint32_t>(sizeof(tangents[0]) * tangents.size()),
				vertex_element(VEU_Tangent, 0, EF_BGR32F), EAH_GPU_Read);
	}

	box_ = MathLib::compute_bounding_box<float>(positions.begin(), positions.end());

	// ��������
	bool has_normal_map = false;
	for (StaticMesh::TextureSlotsType::iterator iter = texture_slots_.begin();
		iter != texture_slots_.end(); ++ iter)
	{
		if ("DiffuseMap" == iter->first)
		{
			*(effect_->ParameterByName("diffuse_tex")) = LoadTexture(iter->second, EAH_GPU_Read)();
		}
		if ("NormalMap" == iter->first)
		{
			TexturePtr nm = LoadTexture(iter->second, EAH_GPU_Read)();
			*(effect_->ParameterByName("normal_tex")) = nm;
			if (nm)
			{
				has_normal_map = true;
			}
		}
		if ("SpecularMap" == iter->first)
		{
			*(effect_->ParameterByName("specular_tex")) = LoadTexture(iter->second, EAH_GPU_Read)();
		}
	}

	if (has_normal_map)
	{
		technique_ = effect_->TechniqueByName("SkinnedMeshTech");
	}
	else
	{
		technique_ = effect_->TechniqueByName("SkinnedMeshNoNormalMapTech");
	}

	boost::shared_ptr<RenderModel> model = model_.lock();
	if (model)
	{
		if (!checked_pointer_cast<MD5SkinnedModel>(model)->reversed_)
		{
			SkinnedModel::JointsType& joints = checked_pointer_cast<MD5SkinnedModel>(model)->joints_;

			for (size_t i = 0; i < joints.size(); ++ i)
			{
				std::swap(joints[i].bind_quat.y(), joints[i].bind_quat.z());
				joints[i].bind_quat.z() = -joints[i].bind_quat.z();

				std::swap(joints[i].bind_pos.y(), joints[i].bind_pos.z());
				joints[i].bind_pos.z() = -joints[i].bind_pos.z();

				joints[i].inverse_origin_quat = MathLib::inverse(joints[i].bind_quat);
				joints[i].inverse_origin_pos = MathLib::transform_quat(-joints[i].bind_pos, joints[i].inverse_origin_quat);
			}
			checked_pointer_cast<MD5SkinnedModel>(model)->UpdateBinds();

			KeyFramesType& key_frames = *checked_pointer_cast<MD5SkinnedModel>(model)->key_frames_;
			BOOST_FOREACH(BOOST_TYPEOF(key_frames)::reference kf, key_frames)
			{
				for (size_t i = 0; i < kf.second.bind_pos.size(); ++ i)
				{
					std::swap(kf.second.bind_quat[i].y(), kf.second.bind_quat[i].z());
					kf.second.bind_quat[i].z() = -kf.second.bind_quat[i].z();

					std::swap(kf.second.bind_pos[i].y(), kf.second.bind_pos[i].z());
					kf.second.bind_pos[i].z() = -kf.second.bind_pos[i].z();
				}
			}

			checked_pointer_cast<MD5SkinnedModel>(model)->reversed_ = true;
		}
	}
}

void MD5SkinnedMesh::OnRenderBegin()
{
	App3DFramework& app = Context::Instance().AppInstance();
	float4x4 worldview(world_ * app.ActiveCamera().ViewMatrix());
	*(effect_->ParameterByName("worldviewproj")) = worldview * app.ActiveCamera().ProjMatrix();

	boost::shared_ptr<RenderModel> model = model_.lock();
	if (model)
	{
		*(effect_->ParameterByName("joint_rots")) = checked_pointer_cast<MD5SkinnedModel>(model)->GetBindRotations();
		*(effect_->ParameterByName("joint_poss")) = checked_pointer_cast<MD5SkinnedModel>(model)->GetBindPositions();
	}
}

void MD5SkinnedMesh::SetWorld(const float4x4& mat)
{
	world_ = mat;
}

void MD5SkinnedMesh::SetEyePos(const KlayGE::float3& eye_pos)
{
	*(effect_->ParameterByName("eye_pos")) = eye_pos;
}


MD5SkinnedModel::MD5SkinnedModel()
		: SkinnedModel(L"MD5SkinnedModel"),
			reversed_(false)
{
}

void MD5SkinnedModel::SetEyePos(const KlayGE::float3& eye_pos)
{
	for (StaticMeshesPtrType::iterator iter = meshes_.begin(); iter != meshes_.end(); ++ iter)
	{
		checked_pointer_cast<MD5SkinnedMesh>(*iter)->SetEyePos(eye_pos);
	}
}

void MD5SkinnedModel::SetTime(float time)
{
	this->SetFrame(static_cast<int>(time * frame_rate_));
}
