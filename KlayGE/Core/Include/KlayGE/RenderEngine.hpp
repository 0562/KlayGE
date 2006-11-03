// RenderEngine.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.5.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.5.0
// ֧��Alpha to Coverage (2006.9.34)
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
#include <KlayGE/RenderStateObject.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/RenderDeviceCaps.hpp>

#include <vector>
#include <list>
#include <boost/array.hpp>

namespace KlayGE
{
	class RenderEngine
	{
	public:
		enum ClearBufferMask
		{
			CBM_Color   = 1UL << 0,
			CBM_Depth   = 1UL << 1,
			CBM_Stencil = 1UL << 2
		};

	public:
		RenderEngine();
		virtual ~RenderEngine();

		static RenderEnginePtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual void StartRendering() = 0;

		void SetRenderTechnique(RenderTechniquePtr const & tech);
		RenderTechniquePtr GetRenderTechnique() const;

		virtual void BeginFrame() = 0;
		void Render(RenderLayout const & rl);
		virtual void EndFrame() = 0;

		size_t NumPrimitivesJustRendered();
		size_t NumVerticesJustRendered();

		virtual void Clear(uint32_t masks, Color const & clr, float depth, int32_t stencil) = 0;

		virtual RenderWindowPtr CreateRenderWindow(std::string const & name, RenderSettings const & settings) = 0;

		virtual void SetStateObjects(RenderStateObject const & rs_obj, ShaderObject const & shader_obj) = 0;

		void BindRenderTarget(RenderTargetPtr rt);
		RenderTargetPtr CurRenderTarget() const;
		RenderTargetPtr DefaultRenderTarget() const;

		// Determines the bit depth of the hardware accelerated stencil buffer, if supported.
		virtual uint16_t StencilBufferBitDepth() = 0;

		// Get render device capabilities
		RenderDeviceCaps const & DeviceCaps() const;

		// Scissor support
		virtual void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		// Return the appropiate offsets as full coordinates in the texture values.
		virtual float4 TexelToPixelOffset() const = 0;

	protected:
		virtual void DoBindRenderTarget(RenderTargetPtr rt) = 0;
		virtual void DoRender(RenderLayout const & rl) = 0;

		virtual void FillRenderDeviceCaps() = 0;

	protected:
		RenderTargetPtr cur_render_target_;
		RenderTargetPtr default_render_target_;

		RenderTechniquePtr render_tech_;

		size_t numPrimitivesJustRendered_;
		size_t numVerticesJustRendered_;

		RenderDeviceCaps caps_;
	};
}

#endif			// _RENDERENGINE_HPP
