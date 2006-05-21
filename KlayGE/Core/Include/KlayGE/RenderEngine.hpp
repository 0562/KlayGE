// RenderEngine.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
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

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/RenderDeviceCaps.hpp>

#include <vector>
#include <list>
#include <boost/array.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	inline uint32_t
	float_to_uint32(float v)
	{
		return *reinterpret_cast<uint32_t*>(&v);
	}
	inline float
	uint32_to_float(uint32_t v)
	{
		return *reinterpret_cast<float*>(&v);
	}

	class RenderEngine
	{
	public:
		enum ShadeMode
		{
			SM_Flat,
			SM_Gouraud
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

		enum PolygonMode
		{
			PM_Point,
			PM_Line,
			PM_Fill
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

		enum BlendOperation
		{
			BOP_Add		= 1,
			BOP_Sub		= 2,
			BOP_Rev_Sub	= 3,
			BOP_Min		= 4,
			BOP_Max		= 5,
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

		enum ClearBufferMask
		{
			CBM_Color   = 1UL << 0,
			CBM_Depth   = 1UL << 1,
			CBM_Stencil = 1UL << 2
		};

		enum RenderStateType
		{
			RST_PolygonMode				= 0x00,
			RST_ShadeMode				= 0x01,
			RST_CullMode				= 0x02,
			RST_Clipping				= 0x03,

			RST_BlendEnable				= 0x04,
			RST_BlendOp					= 0x05,
			RST_SrcBlend				= 0x06,
			RST_DestBlend				= 0x07,
			RST_BlendOpAlpha			= 0x08,
			RST_SrcBlendAlpha			= 0x09,
			RST_DestBlendAlpha			= 0x0A,
			
			RST_DepthEnable				= 0x0B,
			RST_DepthMask				= 0x0C,
			RST_DepthFunc				= 0x0D,
			RST_PolygonOffsetFactor		= 0x0E,
			RST_PolygonOffsetUnits		= 0x0F,

			// Turns stencil buffer checking on or off. 
			RST_FrontStencilEnable		= 0x10,
			// Stencil test function
			RST_FrontStencilFunc		= 0x11,
			// Stencil test reference value
			RST_FrontStencilRef			= 0x12,
			// Stencil test mask value
			RST_FrontStencilMask		= 0x13,
			// Sets the action to perform if the stencil test fails,
			RST_FrontStencilFail		= 0x14,
			// if the stencil test passes, but the depth buffer test fails
			RST_FrontStencilDepthFail	= 0x15,
			// if both the stencil test and the depth buffer test passes
			RST_FrontStencilPass		= 0x16,
			RST_FrontStencilWriteMask	= 0x17,
			RST_BackStencilEnable		= 0x18,
			RST_BackStencilFunc			= 0x19,
			RST_BackStencilRef			= 0x1A,
			RST_BackStencilMask			= 0x1B,
			RST_BackStencilFail			= 0x1C,
			RST_BackStencilDepthFail	= 0x1D,
			RST_BackStencilPass			= 0x1E,
			RST_BackStencilWriteMask	= 0x1F,

			RST_ColorMask0				= 0x20,
			RST_ColorMask1				= 0x21,
			RST_ColorMask2				= 0x22,
			RST_ColorMask3				= 0x23,

			RST_NUM_RENDER_STATES		= 0x24
		};

		enum ColorMask
		{
			CMASK_Red   = 1UL << 3,
			CMASK_Green = 1UL << 2,
			CMASK_Blue  = 1UL << 1,
			CMASK_Alpha = 1UL << 0,
			CMASK_All   = CMASK_Red | CMASK_Green | CMASK_Blue | CMASK_Alpha
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
		void Render(RenderLayout const & rl);
		virtual void EndFrame() = 0;

		size_t NumPrimitivesJustRendered();
		size_t NumVerticesJustRendered();

		virtual void ClearColor(Color const & clr) = 0;
		virtual void Clear(uint32_t masks) = 0;

		virtual RenderWindowPtr CreateRenderWindow(std::string const & name, RenderSettings const & settings) = 0;

		void SetRenderState(RenderStateType rst, uint32_t state);
		uint32_t GetRenderState(RenderStateType rst);

		void ActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget);
		RenderTargetPtr ActiveRenderTarget(uint32_t n) const;

		// Set a sampler.
		virtual void SetSampler(uint32_t stage, SamplerPtr const & sampler) = 0;
		// Turns off a sampler.
		virtual void DisableSampler(uint32_t stage) = 0;

		// Determines the bit depth of the hardware accelerated stencil buffer, if supported.
		virtual uint16_t StencilBufferBitDepth() = 0;

		// Get render device capabilities
		RenderDeviceCaps const & DeviceCaps() const;

		// Scissor support
		virtual void ScissorTest(bool enabled) = 0;
		virtual void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	protected:
		virtual void DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget) = 0;
		virtual void DoRender(RenderLayout const & rl) = 0;
		virtual void DoFlushRenderStates() = 0;

		virtual void InitRenderStates() = 0;
		virtual void FillRenderDeviceCaps() = 0;

	protected:
		std::vector<RenderTargetPtr> renderTargets_;

		RenderEffectPtr renderEffect_;

		size_t numPrimitivesJustRendered_;
		size_t numVerticesJustRendered_;

		RenderDeviceCaps caps_;

		boost::array<uint32_t, RST_NUM_RENDER_STATES> render_states_;
		boost::array<bool, RST_NUM_RENDER_STATES> dirty_render_states_;
	};
}

#endif			// _RENDERENGINE_HPP
