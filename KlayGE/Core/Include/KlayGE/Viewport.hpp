// Viewport.hpp
// KlayGE ��Ⱦ�ӿ��� ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// camera��Ϊָ�� (2005.8.18)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _VIEWPORT_HPP
#define _VIEWPORT_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Camera.hpp>

namespace KlayGE
{
	struct Viewport
	{
		Viewport()
			: camera(new Camera)
		{
		}
		Viewport(int _left, int _top, int _width, int _height)
			: left(_left), top(_top),
				width(_width), height(_height),
				camera(new Camera)
		{
		}

		int left;
		int top;
		int width;
		int height;

		CameraPtr camera;
	};
}

#endif			// _VIEWPORT_HPP
