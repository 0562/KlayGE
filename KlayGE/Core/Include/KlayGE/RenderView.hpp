// RenderView.hpp
// KlayGE ��Ⱦ��ͼ�� ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2006-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ������Clear (2007.8.23)
//
// 3.3.0
// ���ν��� (2006.5.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERVIEW_HPP
#define _RENDERVIEW_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ElementFormat.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API RenderView
	{
	public:
		virtual ~RenderView()
		{
		}

		static RenderViewPtr NullObject();

		uint32_t Width() const
		{
			return width_;
		}
		uint32_t Height() const
		{
			return height_;
		}
		ElementFormat Format() const
		{
			return pf_;
		}
		uint32_t Bpp() const
		{
			return NumFormatBits(pf_);
		}

		virtual void ClearColor(Color const & clr) = 0;
		virtual void ClearDepth(float depth) = 0;
		virtual void ClearStencil(int32_t stencil) = 0;
		virtual void ClearDepthStencil(float depth, int32_t stencil) = 0;

		virtual void OnAttached(FrameBuffer& fb, uint32_t att) = 0;
		virtual void OnDetached(FrameBuffer& fb, uint32_t att) = 0;

		virtual void OnBind(FrameBuffer& fb, uint32_t att);
		virtual void OnUnbind(FrameBuffer& fb, uint32_t att);

	protected:
		uint32_t width_;
		uint32_t height_;
		ElementFormat pf_;
	};
}

#endif			// _RENDERVIEW_HPP
