// OGLRenderFactory.h
// KlayGE OpenGL��Ⱦ������ ͷ�ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// ֧��vertex_buffer_object (2005.6.19)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERFACTORY_HPP
#define _OGLRENDERFACTORY_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_OpenGL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace KlayGE
{
	RenderFactory& OGLRenderFactoryInstance();

	class OGLRenderFactory : public RenderFactory
	{
	public:
		OGLRenderFactory();

		CGcontext CGContext() const;

		std::wstring const & Name() const;

		RenderEngine& RenderEngineInstance();

		TexturePtr MakeTexture1D(uint32_t width, uint16_t numMipMaps,
				ElementFormat format, uint32_t access_hint, ElementInitData* init_data);
		TexturePtr MakeTexture2D(uint32_t width, uint32_t height, uint16_t numMipMaps,
				ElementFormat format, uint32_t access_hint, ElementInitData* init_data);
		TexturePtr MakeTexture3D(uint32_t width, uint32_t height, uint32_t depth,
				uint16_t numMipMaps, ElementFormat format, uint32_t access_hint, ElementInitData* init_data);
		TexturePtr MakeTextureCube(uint32_t size, uint16_t numMipMaps,
				ElementFormat format, uint32_t access_hint, ElementInitData* init_data);

		FrameBufferPtr MakeFrameBuffer();

		RenderLayoutPtr MakeRenderLayout();
		GraphicsBufferPtr MakeVertexBuffer(BufferUsage usage, uint32_t access_hint, ElementInitData* init_data);
		GraphicsBufferPtr MakeIndexBuffer(BufferUsage usage, uint32_t access_hint, ElementInitData* init_data);

		QueryPtr MakeOcclusionQuery();

		RenderViewPtr Make1DRenderView(Texture& texture, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, Texture::CubeFaces face, int level);
		RenderViewPtr Make3DRenderView(Texture& texture, uint32_t slice, int level);
		RenderViewPtr MakeGraphicsBufferRenderView(GraphicsBuffer& gbuffer, uint32_t width, uint32_t height, ElementFormat pf);
		RenderViewPtr MakeDepthStencilRenderView(uint32_t width, uint32_t height, ElementFormat pf, uint32_t multi_sample);

		ShaderObjectPtr MakeShaderObject();

	private:
		RasterizerStateObjectPtr DoMakeRasterizerStateObject(RasterizerStateDesc const & desc);
		DepthStencilStateObjectPtr DoMakeDepthStencilStateObject(DepthStencilStateDesc const & desc);
		BlendStateObjectPtr DoMakeBlendStateObject(BlendStateDesc const & desc);
		SamplerStateObjectPtr DoMakeSamplerStateObject(SamplerStateDesc const & desc);

	private:
		CGcontext context_;

	private:
		OGLRenderFactory(OGLRenderFactory const &);
		OGLRenderFactory& operator=(OGLRenderFactory const &);
	};
}

#endif			// _OGLRENDERFACTORY_HPP
