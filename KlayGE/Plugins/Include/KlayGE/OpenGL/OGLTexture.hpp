// OGLTexture.hpp
// KlayGE OpenGL������ ͷ�ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.3.0
// ������CopyToMemory (2005.2.6)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLTEXTURE_HPP
#define _OGLTEXTURE_HPP

#include <KlayGE/Texture.hpp>

#define NOMINMAX
#include <windows.h>
#include <gl/gl.h>

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLTexture : public Texture
	{
	public:
		OGLTexture(uint32_t width, uint32_t height, uint16_t numMipMaps, PixelFormat format, TextureUsage usage = TU_Default);
		~OGLTexture();

		std::wstring const & Name() const;

		void CustomAttribute(std::string const & name, void* pData);

		void CopyToTexture(Texture& target);
		void CopyToMemory(int level, void* data);
		void CopyMemoryToTexture(int level, void* data, PixelFormat pf,
			uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset);

		void BuildMipSubLevels();

		GLenum GLTexture() const
			{ return texture_; }

	private:
		GLenum texture_;
	};

	typedef boost::shared_ptr<OGLTexture> OGLTexturePtr;
}

#endif			// _OGLTEXTURE_HPP
