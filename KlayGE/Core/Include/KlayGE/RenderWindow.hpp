// RenderWindow.hpp
// KlayGE ��Ⱦ������ ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ������IsTexture (2005.3.6)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERWINDOW_HPP
#define _RENDERWINDOW_HPP

#include <KlayGE/RenderTarget.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class RenderWindow : public RenderTarget
	{
	public:
		RenderWindow();
		virtual ~RenderWindow();

		static RenderWindowPtr NullObject();

		virtual std::wstring const & Description() const = 0;

		virtual void Destroy() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Reposition(uint32_t left, uint32_t top) = 0;

		virtual bool Closed() const = 0;

		bool FullScreen() const;

		bool IsTexture() const
		{
			return false;
		}

	protected:
		bool isFullScreen_;
	};
}

#endif			// _RENDERWINDOW_HPP
