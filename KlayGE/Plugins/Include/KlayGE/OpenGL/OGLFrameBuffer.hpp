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

		void AttachTexture2D(uint32_t n, TexturePtr texture2D);
		void AttachTextureCube(uint32_t n, TexturePtr textureCube, Texture::CubeFaces face);

		void AttachGraphicsBuffer(uint32_t n, GraphicsBufferPtr gb,
			uint32_t width, uint32_t height);

		void Detach(boost::uint32_t n);

		GLuint OGLFbo() const
		{
			return fbo_;
		}

	private:
		void UpdateParams(uint32_t n, TexturePtr texture);
		void UpdateParams(uint32_t n, GraphicsBufferPtr gb, uint32_t width, uint32_t height);

	private:
		std::vector<GLuint> texs_;
		GLuint fbo_;
	};

	typedef boost::shared_ptr<OGLFrameBuffer> OGLFrameBufferPtr;
}

#endif			// _OGLFRAMEBUFFER_HPP
