// OGLRenderTexture.hpp
// KlayGE OpenGL��Ⱦ�������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.8.1)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERTEXTURE_HPP
#define _OGLRENDERTEXTURE_HPP

#include <KlayGE/RenderTexture.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLRenderTexture : public RenderTexture
	{
	public:
		OGLRenderTexture();

		void AttachTexture2D(TexturePtr texture2D);
		void AttachTextureCube(TexturePtr textureCube, Texture::CubeFaces face);
		void DetachTexture();

		virtual void CustomAttribute(std::string const & name, void* pData);

		bool RequiresTextureFlipping() const
			{ return true; }

	private:
		glIsRenderbufferEXTFUNC glIsRenderbufferEXT_;
		glBindRenderbufferEXTFUNC glBindRenderbufferEXT_;
		glDeleteRenderbuffersEXTFUNC glDeleteRenderbuffersEXT_;
		glGenRenderbuffersEXTFUNC glGenRenderbuffersEXT_;
		glRenderbufferStorageEXTFUNC glRenderbufferStorageEXT_;
		glGetRenderbufferParameterivEXTFUNC glGetRenderbufferParameterivEXT_;
		glIsFramebufferEXTFUNC glIsFramebufferEXT_;
		glBindFramebufferEXTFUNC glBindFramebufferEXT_;
		glDeleteFramebuffersEXTFUNC glDeleteFramebuffersEXT_;
		glGenFramebuffersEXTFUNC glGenFramebuffersEXT_;
		glCheckFramebufferStatusEXTFUNC glCheckFramebufferStatusEXT_;
		glFramebufferTexture1DEXTFUNC glFramebufferTexture1DEXT_;
		glFramebufferTexture2DEXTFUNC glFramebufferTexture2DEXT_;
		glFramebufferTexture3DEXTFUNC glFramebufferTexture3DEXT_;
		glFramebufferRenderbufferEXTFUNC glFramebufferRenderbufferEXT_;
		glGetFramebufferAttachmentParameterivEXTFUNC glGetFramebufferAttachmentParameterivEXT_;
		glGenerateMipmapEXTFUNC glGenerateMipmapEXT_;

	private:
		GLuint fbo_;
	};

	typedef boost::shared_ptr<OGLRenderTexture> OGLRenderTexturePtr;
}

#endif			// _OGLRENDERTEXTURE_HPP
