// RenderFactory.hpp
// KlayGE ��Ⱦ������ ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ������MakeVertexBuffer (2005.9.7)
//
// 2.8.0
// ������LoadEffect (2005.7.25)
//
// 2.4.0
// ������1D/2D/3D/cube��֧�� (2005.3.8)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERFACTORY_HPP
#define _RENDERFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/GraphicsBuffer.hpp>

#include <string>
#include <map>
#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

namespace KlayGE
{
	class RenderFactory
	{
	public:
		virtual ~RenderFactory();

		static RenderFactoryPtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual RenderEngine& RenderEngineInstance() = 0;
		virtual TexturePtr MakeTexture1D(uint32_t width, uint16_t numMipMaps,
			ElementFormat format) = 0;
		virtual TexturePtr MakeTexture2D(uint32_t width, uint32_t height, uint16_t numMipMaps,
			ElementFormat format) = 0;
		virtual TexturePtr MakeTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint16_t numMipMaps,
			ElementFormat format) = 0;
		virtual TexturePtr MakeTextureCube(uint32_t size, uint16_t numMipMaps,
			ElementFormat format) = 0;
		virtual FrameBufferPtr MakeFrameBuffer() = 0;

		FontPtr MakeFont(std::string const & fontName, uint32_t fontHeight = 12, uint32_t flags = 0);

		RenderEffectPtr LoadEffect(std::string const & effectName);

		virtual RenderLayoutPtr MakeRenderLayout() = 0;

		virtual GraphicsBufferPtr MakeVertexBuffer(BufferUsage usage) = 0;
		virtual GraphicsBufferPtr MakeIndexBuffer(BufferUsage usage) = 0;

		virtual QueryPtr MakeOcclusionQuery() = 0;

		virtual RenderViewPtr Make1DRenderView(Texture& texture, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, Texture::CubeFaces face, int level) = 0;
		virtual RenderViewPtr Make3DRenderView(Texture& texture, uint32_t slice, int level) = 0;
		virtual RenderViewPtr MakeGraphicsBufferRenderView(GraphicsBuffer& gbuffer, uint32_t width, uint32_t height, ElementFormat pf) = 0;
		virtual RenderViewPtr MakeDepthStencilRenderView(uint32_t width, uint32_t height, ElementFormat pf, uint32_t multi_sample) = 0;

		virtual ShaderObjectPtr MakeShaderObject() = 0;

	protected:
		std::map<std::string, std::vector<RenderEffectPtr> > effect_pool_;

		std::map<std::pair<std::string, uint32_t>, FontPtr> font_pool_;
	};
}

#endif			// _RENDERFACTORY_HPP
