// D3D9Mapping.hpp
// KlayGE RenderEngine��D3D9����֮���ӳ�� ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ���ν��� (2005.3.20)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9MAPPING_HPP
#define _D3D9MAPPING_HPP

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9Mapping
	{
	public:
		static D3DMATRIX Mapping(Matrix4 const & mat);
		static Matrix4 Mapping(D3DMATRIX const & mat);
		static D3DVECTOR Mapping(Vector3 const & vec);
		static D3DCOLOR MappingToUInt32Color(Color const & clr);
		static D3DCOLORVALUE MappingToFloat4Color(Color const & clr);

		static D3DCMPFUNC Mapping(RenderEngine::CompareFunction func);

		static D3DSTENCILOP Mapping(RenderEngine::StencilOperation op);

		static uint32_t MappingToMagFilter(D3DCAPS9 const & caps, RenderEngine::TexFilterOp tf);
		static uint32_t MappingToMinFilter(D3DCAPS9 const & caps, RenderEngine::TexFilterOp tf);
		static uint32_t MappingToMipFilter(D3DCAPS9 const & caps, RenderEngine::TexFilterOp tf);

		static D3DLIGHTTYPE Mapping(Light::LightTypes type);

		static uint32_t Mapping(RenderEngine::CullMode mode);
		static uint32_t Mapping(RenderEngine::FillMode mode);
		static uint32_t Mapping(RenderEngine::ShadeOptions so);
		static uint32_t Mapping(RenderEngine::FogMode mode);
		static uint32_t Mapping(RenderEngine::TexAddressingMode mode);

		static void Mapping(D3DPRIMITIVETYPE& primType, uint32_t& primCount, VertexBuffer const & vb);
		static void Mapping(D3DVERTEXELEMENT9& element, size_t stream, VertexStream const & vs);
	};
}

#endif			// _D3D9MAPPING_HPP
