// OGLFrameBuffer.cpp
// KlayGE OpenGL��Ⱦ�������� ʵ���ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2005-2006
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

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Color.hpp>

#include <glloader/glloader.h>

#include <KlayGE/OpenGL/OGLRenderEngine.hpp>
#include <KlayGE/OpenGL/OGLRenderView.hpp>
#include <KlayGE/OpenGL/OGLFrameBuffer.hpp>

namespace KlayGE
{
	OGLFrameBuffer::OGLFrameBuffer(bool off_screen)
	{
		left_ = 0;
		top_ = 0;

		if (off_screen)
		{
			glGenFramebuffersEXT(1, &fbo_);
		}
		else
		{
			fbo_ = 0;
		}
	}

	OGLFrameBuffer::~OGLFrameBuffer()
	{
		if (fbo_ != 0)
		{
			glDeleteFramebuffersEXT(1, &fbo_);
		}
	}

	std::wstring const & OGLFrameBuffer::Description() const
	{
		static std::wstring const desc(L"OpenGL Frame Buffer Object");
		return desc;
	}

	void OGLFrameBuffer::OnBind()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			THR(boost::system::posix_error::not_supported);
		}
	}

	void OGLFrameBuffer::Clear(uint32_t flags, Color const & clr, float depth, int32_t stencil)
	{
		OGLRenderEngine& re = *checked_cast<OGLRenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());

		GLint old_fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &old_fbo);

		if (static_cast<GLuint>(old_fbo) != fbo_)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_);
		}

		GLbitfield ogl_flags = 0;
		if (flags & CBM_Color)
		{
			ogl_flags |= GL_COLOR_BUFFER_BIT;
			re.ClearColor(clr.r(), clr.g(), clr.b(), clr.a());
		}
		bool depth_mask_changed = false;
		if (flags & CBM_Depth)
		{
			ogl_flags |= GL_DEPTH_BUFFER_BIT;
			re.ClearDepth(depth);

			GLint m;
			glGetIntegerv(GL_DEPTH_WRITEMASK, &m);
			if (GL_FALSE == m)
			{
				depth_mask_changed = true;
				glDepthMask(GL_TRUE);
			}
		}
		bool stencil_mask_changed = false;
		if (flags & CBM_Stencil)
		{
			ogl_flags |= GL_STENCIL_BUFFER_BIT;
			re.ClearStencil(stencil);

			GLint m;
			glGetIntegerv(GL_STENCIL_WRITEMASK, &m);
			if (GL_FALSE == m)
			{
				stencil_mask_changed = true;
				glStencilMask(GL_TRUE);
			}
		}

		glClear(ogl_flags);

		if (depth_mask_changed)
		{
			glDepthMask(GL_FALSE);
		}
		if (stencil_mask_changed)
		{
			glStencilMask(GL_FALSE);
		}

		if (static_cast<GLuint>(old_fbo) != fbo_)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, old_fbo);
		}
	}
}
