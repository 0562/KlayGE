// OGLES2FrameBuffer.cpp
// KlayGE OpenGL ES 2��Ⱦ�������� ʵ���ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ���ν��� (2010.1.22)
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

#include <KlayGE/OpenGLES2/OGLES2RenderEngine.hpp>
#include <KlayGE/OpenGLES2/OGLES2RenderView.hpp>
#include <KlayGE/OpenGLES2/OGLES2FrameBuffer.hpp>

namespace KlayGE
{
	OGLES2FrameBuffer::OGLES2FrameBuffer(bool off_screen)
	{
		left_ = 0;
		top_ = 0;

		if (off_screen)
		{
			glGenFramebuffers(1, &fbo_);
		}
		else
		{
			fbo_ = 0;
		}
	}

	OGLES2FrameBuffer::~OGLES2FrameBuffer()
	{
		if (fbo_ != 0)
		{
			if (Context::Instance().RenderFactoryValid())
			{
				OGLES2RenderEngine& re = *checked_cast<OGLES2RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
				re.DeleteFramebuffers(1, &fbo_);
			}
			else
			{
				glDeleteFramebuffers(1, &fbo_);
			}
		}
	}

	std::wstring const & OGLES2FrameBuffer::Description() const
	{
		static std::wstring const desc(L"OpenGL Frame Buffer Object");
		return desc;
	}

	void OGLES2FrameBuffer::OnBind()
	{
		OGLES2RenderEngine& re = *checked_cast<OGLES2RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		re.BindFramebuffer(fbo_);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			THR(boost::system::posix_error::not_supported);
		}
	}

	void OGLES2FrameBuffer::Clear(uint32_t flags, Color const & clr, float depth, int32_t stencil)
	{
		OGLES2RenderEngine& re = *checked_cast<OGLES2RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());

		GLuint old_fbo = re.BindFramebuffer();
		re.BindFramebuffer(fbo_);

		DepthStencilStateDesc const & ds_desc = re.CurDSSObj()->GetDesc();
		BlendStateDesc const & blend_desc = re.CurBSObj()->GetDesc();

		if (flags & CBM_Color)
		{
			if (blend_desc.color_write_mask[0] != CMASK_All)
			{
				glColorMask(true, true, true, true);
			}
		}
		if (flags & CBM_Depth)
		{
			if (!ds_desc.depth_write_mask)
			{
				glDepthMask(GL_TRUE);
			}
		}
		if (flags & CBM_Stencil)
		{
			if (!ds_desc.front_stencil_write_mask)
			{
				glStencilMaskSeparate(GL_FRONT, GL_TRUE);
			}
			if (!ds_desc.back_stencil_write_mask)
			{
				glStencilMaskSeparate(GL_BACK, GL_TRUE);
			}
		}

		GLbitfield ogl_flags = 0;
		if (flags & CBM_Color)
		{
			ogl_flags |= GL_COLOR_BUFFER_BIT;
			re.ClearColor(clr.r(), clr.g(), clr.b(), clr.a());
		}
		if (flags & CBM_Depth)
		{
			ogl_flags |= GL_DEPTH_BUFFER_BIT;
			re.ClearDepth(depth);
		}
		if (flags & CBM_Stencil)
		{
			ogl_flags |= GL_STENCIL_BUFFER_BIT;
			re.ClearStencil(stencil);
		}

		glClear(ogl_flags);

		if (flags & CBM_Color)
		{
			if (blend_desc.color_write_mask[0] != CMASK_All)
			{
				glColorMask((blend_desc.color_write_mask[0] & CMASK_Red) != 0,
						(blend_desc.color_write_mask[0] & CMASK_Green) != 0,
						(blend_desc.color_write_mask[0] & CMASK_Blue) != 0,
						(blend_desc.color_write_mask[0] & CMASK_Alpha) != 0);
			}
		}
		if (flags & CBM_Depth)
		{
			if (!ds_desc.depth_write_mask)
			{
				glDepthMask(GL_FALSE);
			}
		}
		if (flags & CBM_Stencil)
		{
			if (!ds_desc.front_stencil_write_mask)
			{
				glStencilMaskSeparate(GL_FRONT, GL_FALSE);
			}
			if (!ds_desc.back_stencil_write_mask)
			{
				glStencilMaskSeparate(GL_BACK, GL_FALSE);
			}
		}

		re.BindFramebuffer(old_fbo);
	}
}
