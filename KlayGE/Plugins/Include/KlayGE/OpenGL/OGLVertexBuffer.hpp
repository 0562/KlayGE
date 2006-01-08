// OGLVertexBuffer.cpp
// KlayGE OpenGL���㻺������ ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ���ν��� (2005.9.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLVERTEXBUFFER_HPP
#define _OGLVERTEXBUFFER_HPP

#include <KlayGE/VertexBuffer.hpp>

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

#endif			// _OGLVERTEXBUFFER_HPP
