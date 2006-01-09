// OGLRenderLayout.cpp
// KlayGE OpenGL��Ⱦ�ֲ��� ʵ���ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2005.1.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERLAYOUT_HPP
#define _OGLRENDERLAYOUT_HPP

#include <KlayGE/RenderLayout.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLRenderLayout : public RenderLayout
	{
	public:
		explicit OGLRenderLayout(buffer_type type)
			: RenderLayout(type)
		{
		}

		~OGLRenderLayout()
		{
		}
	};
}

#endif			// _OGLRENDERLAYOUT_HPP
