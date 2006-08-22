// KMesh.hpp
// KlayGE KMesh�� ͷ�ļ�
// Ver 2.7.1
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.1
// LoadKMesh����ʹ���Զ����� (2005.7.13)
//
// 2.7.0
// ���ν��� (2005.6.17)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _KMESH_HPP
#define _KMESH_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Mesh.hpp>

#include <boost/function.hpp>

namespace KlayGE
{
	class KMesh : public StaticMesh
	{
	public:
		KMesh(std::wstring const & name, TexturePtr tex);
		virtual ~KMesh();

		virtual void OnRenderBegin();
		void SetModelMatrix(float4x4 const & model);

	private:
		SamplerPtr sampler_;
		float4x4 model_;
	};

	template <typename T>
	struct CreateKMeshFactory
	{
		KMeshPtr operator()(std::wstring const & name, TexturePtr tex)
		{
			return KMeshPtr(new T(name, tex));
		}
	};

	RenderModelPtr LoadKModel(std::string const & kmodel_name,
		boost::function<KMeshPtr (std::wstring const &, TexturePtr)> CreateFactoryFunc = CreateKMeshFactory<KMesh>());
	SkinnedModelPtr LoadKSkinnedModel(std::string const & kmodel_name,
		boost::function<KMeshPtr (std::wstring const &, TexturePtr)> CreateFactoryFunc = CreateKMeshFactory<KMesh>());
}

#endif			// _KMESH_HPP
