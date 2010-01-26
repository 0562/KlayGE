// OGLES2FrameBuffer.hpp
// KlayGE OpenGL ES 2��Ⱦ�������� ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// ���ν��� (2010.1.22)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLES2FRAMEBUFFER_HPP
#define _OGLES2FRAMEBUFFER_HPP

#pragma once

#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/RenderView.hpp>

namespace KlayGE
{
	class OGLES2FrameBuffer : public FrameBuffer
	{
	public:
		explicit OGLES2FrameBuffer(bool off_screen);
		virtual ~OGLES2FrameBuffer();

		virtual std::wstring const & Description() const;

		void OnBind();

		bool RequiresFlipping() const
		{
			return false;
		}

		void Clear(uint32_t flags, Color const & clr, float depth, int32_t stencil);

		GLuint OGLFbo() const
		{
			return fbo_;
		}

	protected:
		GLuint fbo_;
	};

	typedef boost::shared_ptr<OGLES2FrameBuffer> OGLES2FrameBufferPtr;
}

#endif			// _OGLES2FRAMEBUFFER_HPP
