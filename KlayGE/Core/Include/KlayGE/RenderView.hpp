// RenderView.hpp
// KlayGE ��Ⱦ��ͼ�� ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ���ν��� (2006.5.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERVIEW_HPP
#define _RENDERVIEW_HPP

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{	
	class RenderView
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
		uint32_t Bpp() const
		{
			return bpp_;
		}

		virtual void OnAttached(FrameBuffer& fb, uint32_t att) = 0;
		virtual void OnDetached(FrameBuffer& fb, uint32_t att) = 0;

	protected:
		uint32_t width_;
		uint32_t height_;
		uint32_t bpp_;
	};
}

#endif			// _RENDERVIEW_HPP
