// RenderTarget.hpp
// KlayGE ��ȾĿ���� ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ����boost::timer��ʱ (2004.11.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERTARGET_HPP
#define _RENDERTARGET_HPP

#include <KlayGE/Viewport.hpp>
#include <boost/timer.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	class RenderTarget
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();

		virtual int Left() const;
		virtual int Top() const;
		virtual int Width() const;
		virtual int Height() const;
		virtual int ColorDepth() const;

		virtual void Update();

		virtual Viewport const & GetViewport() const;
		virtual Viewport& GetViewport();
		virtual void SetViewport(Viewport const & viewport);

		virtual float FPS() const;

		// Gets a custom (maybe platform-specific) attribute.
		virtual void CustomAttribute(std::string const & name, void* pData) = 0;

		virtual bool Active() const;
		virtual void Active(bool state);

		virtual bool RequiresTextureFlipping() const = 0;

	protected:
		void UpdateStats();

		int		left_;
		int		top_;
		int		width_;
		int		height_;
		int		colorDepth_;
		bool	isDepthBuffered_;

		// Stats
		float	FPS_;
		float	frameTime_;

		bool	active_;

		Viewport viewport_;

		boost::timer timer_;
	};
}

#endif			// _RENDERTARGET_HPP
