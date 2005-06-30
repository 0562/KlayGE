// KMesh.hpp
// KlayGE KMesh�� ͷ�ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
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
		explicit KMesh(std::wstring const & name, std::string const & tex_name);
		~KMesh();

		virtual void OnRenderBegin();

	private:
		TexturePtr tex_;
	};

	StaticMeshPtr LoadKMesh(std::string const & kmeshName);
}

#endif			// _KMESH_HPP
