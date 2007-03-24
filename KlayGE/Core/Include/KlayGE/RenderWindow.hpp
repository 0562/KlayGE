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

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/RenderTarget.hpp>

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

		void Resize(uint32_t width, uint32_t height);
		void Reposition(uint32_t left, uint32_t top);

		virtual bool Closed() const = 0;

		bool FullScreen() const;
		void FullScreen(bool fs);

		void OnBind()
		{
		}
		void OnUnbind()
		{
		}

	protected:
		virtual void DoResize(uint32_t width, uint32_t height) = 0;
		virtual void DoReposition(uint32_t left, uint32_t top) = 0;
		virtual void DoFullScreen(bool fs) = 0;

	protected:
		bool isFullScreen_;
	};
}

#endif			// _RENDERWINDOW_HPP
