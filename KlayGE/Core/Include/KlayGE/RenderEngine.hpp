// RenderEngine.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
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

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/RenderDeviceCaps.hpp>

#include <vector>
#include <list>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class RenderEngine
	{
	public:
		enum ShadeOptions
		{
			SO_Flat,
			SO_Gouraud,
			SO_Phong
		};

		enum CompareFunction
		{
			CF_AlwaysFail,
			CF_AlwaysPass,
			CF_Less,
			CF_LessEqual,
			CF_Equal,
			CF_NotEqual,
			CF_GreaterEqual,
			CF_Greater
		};

		enum CullMode
		{
			CM_None,
			CM_Clockwise,
			CM_AntiClockwise
		};

		enum FillMode
		{
			FM_Point,
			FM_Line,
			FM_Fill
		};

		// Type of alpha blend factor.
		enum AlphaBlendFactor
		{
			ABF_Zero,
			ABF_One,
			ABF_Src_Alpha,
			ABF_Dst_Alpha,
			ABF_Inv_Src_Alpha,
			ABF_Inv_Dst_Alpha,
			ABF_Src_Color,
			ABF_Dst_Color,
			ABF_Inv_Src_Color,
			ABF_Inv_Dst_Color,
			ABF_Src_Alpha_Sat
		};

		// Enum describing the various actions which can be taken onthe stencil buffer
		enum StencilOperation
		{
			// Leave the stencil buffer unchanged
			SOP_Keep,
			// Set the stencil value to zero
			SOP_Zero,
			// Set the stencil value to the reference value
			SOP_Replace,
			// Increase the stencil value by 1, clamping at the maximum value
			SOP_Increment,
			// Decrease the stencil value by 1, clamping at 0
			SOP_Decrement,
			// Invert the bits of the stencil buffer
			SOP_Invert
		};

	public:
		RenderEngine();
		virtual ~RenderEngine();

		static RenderEnginePtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual void StartRendering() = 0;

		void SetRenderEffect(RenderEffectPtr const & effect);
		RenderEffectPtr GetRenderEffect() const;

		virtual void BeginFrame() = 0;
		void Render(VertexBuffer const & vb);
		virtual void EndFrame() = 0;

		size_t NumPrimitivesJustRendered();
		size_t NumVerticesJustRendered();

		virtual void ClearColor(Color const & clr) = 0;

		virtual void ShadingType(ShadeOptions so) = 0;

		virtual RenderWindowPtr CreateRenderWindow(std::string const & name, RenderSettings const & settings) = 0;

		virtual void CullingMode(CullMode mode) = 0;
		virtual void PolygonMode(FillMode mode) = 0;

		virtual void AlphaBlend(bool enabled) = 0;
		virtual void AlphaBlendFunction(AlphaBlendFactor src_factor, AlphaBlendFactor dst_factor) = 0;

		virtual void DepthBufferDepthTest(bool enabled) = 0;
		virtual void DepthBufferDepthWrite(bool enabled) = 0;
		virtual void DepthBufferFunction(CompareFunction depthFunction) = 0;
		virtual void DepthBias(uint16_t bias) = 0;

		virtual void AlphaTest(bool enabled) = 0;
		virtual void AlphaFunction(CompareFunction alphaFunction, float refValue) = 0;

		void ActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget);
		RenderTargetPtr ActiveRenderTarget(uint32_t n) const;

		// Set a sampler.
		virtual void SetSampler(uint32_t stage, SamplerPtr const & sampler) = 0;
		// Turns off a sampler.
		virtual void DisableSampler(uint32_t stage) = 0;

		// Turns stencil buffer checking on or off. 
		virtual void StencilCheckEnabled(bool enabled) = 0;
		// Determines if this system supports hardware accelerated stencil buffer. 
		virtual bool HasHardwareStencil() = 0;

		// Determines the bit depth of the hardware accelerated stencil buffer, if supported.
		virtual uint16_t StencilBufferBitDepth() = 0;

		// Sets the stencil test function, reference value and mask value.
		virtual void StencilBufferFunction(CompareFunction func, uint32_t refValue, uint32_t mask) = 0;
		// Sets the action to perform if the stencil test fails,
		// if the stencil test passes, but the depth buffer test fails, and
		// if both the stencil test and the depth buffer test passes.
		virtual void StencilBufferOperation(StencilOperation fail, StencilOperation depth_fail, StencilOperation pass) = 0;

		// Get render device capabilities
		RenderDeviceCaps const & DeviceCaps() const;

		// Point sprite support
		virtual void PointSpriteEnable(bool enable) = 0;
		virtual void PointDistanceAttenuation(float quadratic0, float quadratic1, float quadratic2) = 0;
		virtual void PointSize(float size) = 0;
		virtual void PointMinMaxSize(float min_size, float max_size) = 0;

		// Scissor support
		virtual void ScissorTest(bool enabled) = 0;
		virtual void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	protected:
		virtual void DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget) = 0;

		virtual void DoRender(VertexBuffer const & vb) = 0;

		virtual void FillRenderDeviceCaps() = 0;

	protected:
		std::vector<RenderTargetPtr> renderTargets_;

		RenderEffectPtr renderEffect_;

		size_t numPrimitivesJustRendered_;
		size_t numVerticesJustRendered_;

		RenderDeviceCaps caps_;
	};
}

#endif			// _RENDERENGINE_HPP
