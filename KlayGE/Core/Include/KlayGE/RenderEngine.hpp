// RenderEngine.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2003-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ȥ����RenderTarget��ֱ��ʹ��FrameBuffer (2007.6.20)
//
// 3.5.0
// ֧��Alpha to Coverage (2006.9.24)
//
// 3.4.0
// ������TexelToPixelOffset (2006.8.27)
// ȥ����ClearColor (2006.8.31)
//
// 3.3.0
// ͳһ��RenderState (2006.5.21)
//
// 3.2.0
// ��¶����Clear (2005.12.31)
//
// 3.0.0
// ȥ���˹̶���ˮ�� (2005.8.18)
// ֧��point sprite (2005.9.28)
// ֧��scissor (2005.10.20)
//
// 2.8.0
// ����StencilBuffer��ز��� (2005.7.20)
// ����RenderTarget��֧��MRT (2005.7.25)
// ȥ���������������� (2005.7.30)
//
// 2.7.1
// ViewMatrix��ProjectionMatrix��Ϊconst (2005.7.10)
//
// 2.7.0
// ȥ����TextureCoordSet (2005.6.26)
// TextureAddressingMode, TextureFiltering��TextureAnisotropy�Ƶ�Texture�� (2005.6.27)
//
// 2.4.0
// ������NumFacesJustRendered��NumVerticesJustRendered (2005.3.21)
// ������PolygonMode (2005.3.20)
//
// 2.0.4
// ȥ����WorldMatrices (2004.4.3)
// �������������� (2004.4.7)
//
// 2.0.3
// ȥ����SoftwareBlend (2004.3.10)
//
// 2.0.1
// ȥ����TexBlendMode (2003.10.16)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERENGINE_HPP
#define _RENDERENGINE_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderDeviceCaps.hpp>

namespace KlayGE
{
	class RenderEngine
	{
	public:
		RenderEngine();
		virtual ~RenderEngine();

		static RenderEnginePtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual void StartRendering() = 0;

		virtual void BeginFrame() = 0;
		void Render(RenderTechnique const & tech, RenderLayout const & rl);
		virtual void EndFrame() = 0;

		size_t NumPrimitivesJustRendered();
		size_t NumVerticesJustRendered();

		virtual void CreateRenderWindow(std::string const & name, RenderSettings const & settings) = 0;

		void SetStateObjects(RasterizerStateObjectPtr const & rs_obj, DepthStencilStateObjectPtr const & dss_obj, uint16_t front_stencil_ref, uint16_t back_stencil_ref,
			BlendStateObjectPtr const & bs_obj, Color const & blend_factor, uint32_t sample_mask, ShaderObjectPtr const & shader_obj);

		void BindFrameBuffer(FrameBufferPtr fb);
		FrameBufferPtr CurFrameBuffer() const;
		FrameBufferPtr DefaultFrameBuffer() const;

		// Determines the bit depth of the hardware accelerated stencil buffer, if supported.
		virtual uint16_t StencilBufferBitDepth() = 0;

		// Get render device capabilities
		RenderDeviceCaps const & DeviceCaps() const;

		// Scissor support
		virtual void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		// Return the appropiate offsets as full coordinates in the texture values.
		virtual float4 TexelToPixelOffset() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual bool FullScreen() const = 0;
		virtual void FullScreen(bool fs) = 0;

	protected:
		virtual void DoBindFrameBuffer(FrameBufferPtr fb) = 0;
		virtual void DoRender(RenderTechnique const & tech, RenderLayout const & rl) = 0;

	protected:
		FrameBufferPtr cur_frame_buffer_;
		FrameBufferPtr default_frame_buffer_;

		size_t numPrimitivesJustRendered_;
		size_t numVerticesJustRendered_;

		RenderDeviceCaps caps_;

		RasterizerStateObjectPtr cur_rs_obj_;
		DepthStencilStateObjectPtr cur_dss_obj_;
		BlendStateObjectPtr cur_bs_obj_;
	};
}

#endif			// _RENDERENGINE_HPP
