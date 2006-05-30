// RenderTarget.hpp
// KlayGE ��ȾĿ���� ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ������IsTexture��SwapBuffers (2005.3.6)
//
// 2.2.0
// ����boost::timer��ʱ (2004.11.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERTARGET_HPP
#define _RENDERTARGET_HPP

#include <KlayGE/Viewport.hpp>
#include <KlayGE/Timer.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class RenderTarget
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();

		virtual uint32_t Left() const;
		virtual uint32_t Top() const;
		virtual uint32_t Width() const;
		virtual uint32_t Height() const;
		virtual uint32_t ColorDepth() const;
		virtual uint32_t DepthBits() const;
		virtual uint32_t StencilBits() const;

		virtual void Update();

		virtual Viewport const & GetViewport() const;
		virtual Viewport& GetViewport();
		virtual void SetViewport(Viewport const & viewport);

		virtual float FPS() const;

		virtual bool Active() const;
		virtual void Active(bool state);

		virtual void SwapBuffers() = 0;

	protected:
		void UpdateStats();

		uint32_t	left_;
		uint32_t	top_;
		uint32_t	width_;
		uint32_t	height_;
		uint32_t	colorDepth_;

		bool		isDepthBuffered_;
		uint32_t	depthBits_;
		uint32_t	stencilBits_;

		// Stats
		float	FPS_;
		float	frameTime_;

		bool	active_;	// Is active i.e. visible

		Viewport viewport_;

		Timer timer_;
	};
}

#endif			// _RENDERTARGET_HPP
