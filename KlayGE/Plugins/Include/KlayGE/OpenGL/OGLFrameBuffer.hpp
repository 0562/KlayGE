// OGLFrameBuffer.hpp
// KlayGE OpenGL��Ⱦ�������� ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ��ΪFrameBuffer (2006.5.30)
//
// 2.8.0
// ���ν��� (2005.8.1)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLFRAMEBUFFER_HPP
#define _OGLFRAMEBUFFER_HPP

#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/RenderView.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLFrameBuffer : public FrameBuffer
	{
	public:
		OGLFrameBuffer();
		~OGLFrameBuffer();

		void Attach(uint32_t att, RenderViewPtr view);
		void Detach(boost::uint32_t att);

		GLuint OGLFbo() const
		{
			return fbo_;
		}

	private:
		GLuint fbo_;
		GLuint depth_rb_;
	};

	typedef boost::shared_ptr<OGLFrameBuffer> OGLFrameBufferPtr;
}

#endif			// _OGLFRAMEBUFFER_HPP
