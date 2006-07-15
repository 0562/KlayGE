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
#include <KlayGE/RenderLayout.hpp>

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
		static D3DMATRIX Mapping(float4x4 const & mat);
		static float4x4 Mapping(D3DMATRIX const & mat);
		static D3DVECTOR Mapping(float3 const & vec);
		static D3DCOLOR MappingToUInt32Color(Color const & clr);
		static D3DCOLORVALUE MappingToFloat4Color(Color const & clr);
		static uint32_t MappingColorMask(uint32_t mask);

		static D3DCMPFUNC Mapping(RenderEngine::CompareFunction func);

		static D3DSTENCILOP Mapping(RenderEngine::StencilOperation op);

		static uint32_t Mapping(RenderEngine::AlphaBlendFactor factor);
		static uint32_t Mapping(RenderEngine::CullMode mode);
		static uint32_t Mapping(RenderEngine::PolygonMode mode);
		static uint32_t Mapping(RenderEngine::ShadeMode mode);
		static uint32_t Mapping(RenderEngine::BlendOperation bo);
		static uint32_t Mapping(Sampler::TexAddressingMode mode);

		static void Mapping(D3DPRIMITIVETYPE& primType, uint32_t& primCount, RenderLayout const & rl);
		static void Mapping(std::vector<D3DVERTEXELEMENT9>& elements, size_t stream, vertex_elements_type const & vet);

		static RenderDeviceCaps Mapping(D3DCAPS9 const & d3d_caps);

		static D3DFORMAT MappingFormat(ElementFormat pf);
		static ElementFormat MappingFormat(D3DFORMAT d3dfmt);
	};
}

#endif			// _D3D9MAPPING_HPP
