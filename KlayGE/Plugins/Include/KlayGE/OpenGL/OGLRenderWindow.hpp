// OGLRenderWindow.hpp
// KlayGE OpenGL��Ⱦ������ ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2004-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ʵ���Ե�linux֧�� (2008.5.19)
//
// 3.6.0
// ֧�ֶ�̬�л�ȫ��/����ģʽ (2007.3.24)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERWINDOW_HPP
#define _OGLRENDERWINDOW_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/OpenGL/OGLFrameBuffer.hpp>

namespace KlayGE
{
	struct RenderSettings;

	class OGLRenderWindow : public OGLFrameBuffer
	{
	public:
		OGLRenderWindow(std::string const & name, RenderSettings const & settings);
		~OGLRenderWindow();

		void Destroy();

		bool Closed() const;

		bool Ready() const;
		void Ready(bool ready);

		void SwapBuffers();

		std::wstring const & Description() const;

		void Resize(uint32_t width, uint32_t height);
		void Reposition(uint32_t left, uint32_t top);

		bool FullScreen() const;
		void FullScreen(bool fs);

		// Method for dealing with resize / move & 3d library
		void WindowMovedOrResized();

		bool RequiresFlipping() const
		{
			return false;
		}

	private:
		void OnActive(Window const & win, bool active);
		void OnPaint(Window const & win);
		void OnEnterSizeMove(Window const & win);
		void OnExitSizeMove(Window const & win);
		void OnSize(Window const & win, bool active);
		void OnClose(Window const & win);

	private:
		std::string	name_;

#if defined KLAYGE_PLATFORM_WINDOWS
		HWND	hWnd_;
		HGLRC	hRC_;
		HDC		hDC_;
#elif defined KLAYGE_PLATFORM_LINUX
		::Display* x_display_;
		::Window x_window_;
		::GLXContext x_context_;
		::GLXFBConfig* fbc_;
#endif

		bool	ready_;				// Is ready i.e. available for update
		bool	closed_;
		bool	isFullScreen_;

		std::wstring			description_;

		uint32_t				fs_color_depth_;
	};
}

#endif			// _OGLRENDERWINDOW_HPP
