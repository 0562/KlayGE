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

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Mesh.hpp>

#include <boost/function.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class KMesh : public StaticMesh
	{
	public:
		KMesh(std::wstring const & name, TexturePtr tex);
		virtual ~KMesh();

		virtual void OnRenderBegin();

	private:
		SamplerPtr sampler_;
	};

	template <typename T>
	KMeshPtr CreateFactory(std::wstring const & name, TexturePtr tex)
	{
		return KMeshPtr(new T(name, tex));
	}

	StaticMeshPtr LoadKMesh(std::string const & kmeshName,
		boost::function<KMeshPtr (std::wstring const &, TexturePtr)> CreateFactoryFunc = CreateFactory<KMesh>);
}

#endif			// _KMESH_HPP
