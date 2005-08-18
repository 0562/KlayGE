// OGLMapping.hpp
// KlayGE RenderEngine��OpenGL����֮���ӳ�� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.19)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLMAPPING_HPP
#define _OGLMAPPING_HPP

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
	class OGLMapping
	{
	public:
		static void Mapping(GLfloat* clr4, Color const & clr);

		static GLenum Mapping(RenderEngine::CompareFunction func);

		static GLenum Mapping(RenderEngine::StencilOperation op);

		static GLenum Mapping(RenderEngine::AlphaBlendFactor factor);
		static GLenum Mapping(RenderEngine::FillMode mode);
		static GLenum Mapping(RenderEngine::ShadeOptions so);
		static GLint Mapping(Sampler::TexAddressingMode mode);

		static void Mapping(GLenum& primType, uint32_t& primCount, VertexBuffer const & vb);
	};
}

#endif			// _OGLMAPPING_HPP
