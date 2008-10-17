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

namespace KlayGE
{
	class OGLFrameBuffer : public FrameBuffer
	{
	public:
		explicit OGLFrameBuffer(bool off_screen);
		virtual ~OGLFrameBuffer();

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

	typedef boost::shared_ptr<OGLFrameBuffer> OGLFrameBufferPtr;
}

#endif			// _OGLFRAMEBUFFER_HPP
