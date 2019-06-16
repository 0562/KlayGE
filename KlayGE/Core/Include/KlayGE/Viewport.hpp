// Viewport.hpp
// KlayGE ��Ⱦ�ӿ��� ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
//
// 3.0.0
// camera��Ϊָ�� (2005.8.18)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _VIEWPORT_HPP
#define _VIEWPORT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API Viewport : boost::noncopyable
	{
	public:
		Viewport();
		Viewport(int left, int top, int width, int height);

		void Left(int left)
		{
			left_ = left;
		}
		int Left() const
		{
			return left_;
		}
		void Top(int top)
		{
			top_ = top;
		}
		int Top() const
		{
			return top_;
		}
		void Width(int width)
		{
			width_ = width;
		}
		int Width() const
		{
			return width_;
		}
		void Height(int height)
		{
			height_ = height;
		}
		int Height() const
		{
			return height_;
		}

		void Camera(CameraPtr const& camera)
		{
			camera_ = camera;
		}
		CameraPtr const& Camera() const
		{
			return camera_;
		}

	private:
		int left_;
		int top_;
		int width_;
		int height_;

		CameraPtr camera_;
	};
}

#endif			// _VIEWPORT_HPP
