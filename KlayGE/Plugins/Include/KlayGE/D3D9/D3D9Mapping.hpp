// D3D9Mapping.hpp
// KlayGE RenderEngine��D3D9����֮���ӳ�� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������RenderDeviceCaps (2005.7.19)
//
// 2.4.0
// ���ν��� (2005.3.20)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9MAPPING_HPP
#define _D3D9MAPPING_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Sampler.hpp>

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

		static uint32_t Mapping(RenderEngine::AlphaBlendFactor factor);
		static uint32_t Mapping(RenderEngine::CullMode mode);
		static uint32_t Mapping(RenderEngine::FillMode mode);
		static uint32_t Mapping(RenderEngine::ShadeOptions so);
		static uint32_t Mapping(Sampler::TexAddressingMode mode);

		static void Mapping(D3DPRIMITIVETYPE& primType, uint32_t& primCount, VertexBuffer const & vb);
		static void Mapping(std::vector<D3DVERTEXELEMENT9>& elements, size_t stream, VertexStream const & vs);

		static RenderDeviceCaps Mapping(D3DCAPS9 const & d3d_caps);
	};
}

#endif			// _D3D9MAPPING_HPP
