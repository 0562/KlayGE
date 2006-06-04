// OGLTexture.hpp
// KlayGE OpenGL������ ʵ���ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// ������AddressingMode, Filtering��Anisotropy (2005.6.27)
// ������MaxMipLevel��MipMapLodBias (2005.6.28)
//
// 2.3.0
// ������CopyToMemory (2005.2.6)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/Context.hpp>

#include <cstring>

#include <glloader/glloader.h>
#include <gl/glu.h>

#include <KlayGE/OpenGL/OGLMapping.hpp>
#include <KlayGE/OpenGL/OGLTexture.hpp>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

namespace KlayGE
{
	OGLTexture::OGLTexture(TextureType type)
					: Texture(type)
	{
		switch (type_)
		{
		case TT_1D:
			target_type_ = GL_TEXTURE_1D;

		case TT_2D:
			target_type_ = GL_TEXTURE_2D;

		case TT_3D:
			target_type_ = GL_TEXTURE_3D;

		case TT_Cube:
			target_type_ = GL_TEXTURE_CUBE_MAP;

		default:
			BOOST_ASSERT(false);
			target_type_ = GL_TEXTURE_1D;
		}
	}

	OGLTexture::~OGLTexture()
	{
		glDeleteTextures(1, &texture_);
	}

	std::wstring const & OGLTexture::Name() const
	{
		static const std::wstring name(L"OpenGL Texture");
		return name;
	}

	uint32_t OGLTexture::Width(int /*level*/) const
	{
		BOOST_ASSERT(false);
		return 0;
	}
	
	uint32_t OGLTexture::Height(int /*level*/) const
	{
		BOOST_ASSERT(false);
		return 0;
	}

	uint32_t OGLTexture::Depth(int /*level*/) const
	{
		BOOST_ASSERT(false);
		return 0;
	}

	void OGLTexture::CopyToMemory1D(int /*level*/, void* /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyToMemory2D(int /*level*/, void* /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyToMemory3D(int /*level*/, void* /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyToMemoryCube(CubeFaces /*face*/, int /*level*/, void* /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyMemoryToTexture1D(int /*level*/, void* /*data*/, PixelFormat /*pf*/,
		uint32_t /*dst_width*/, uint32_t /*dst_xOffset*/, uint32_t /*src_width*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyMemoryToTexture2D(int /*level*/, void* /*data*/, PixelFormat /*pf*/,
		uint32_t /*dst_width*/, uint32_t /*dst_height*/, uint32_t /*dst_xOffset*/, uint32_t /*dst_yOffset*/,
		uint32_t /*src_width*/, uint32_t /*src_height*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyMemoryToTexture3D(int /*level*/, void* /*data*/, PixelFormat /*pf*/,
			uint32_t /*dst_width*/, uint32_t /*dst_height*/, uint32_t /*dst_depth*/,
			uint32_t /*dst_xOffset*/, uint32_t /*dst_yOffset*/, uint32_t /*dst_zOffset*/,
			uint32_t /*src_width*/, uint32_t /*src_height*/, uint32_t /*src_depth*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::CopyMemoryToTextureCube(CubeFaces /*face*/, int /*level*/, void* /*data*/, PixelFormat /*pf*/,
			uint32_t /*dst_width*/, uint32_t /*dst_height*/, uint32_t /*dst_xOffset*/, uint32_t /*dst_yOffset*/,
			uint32_t /*src_width*/, uint32_t /*src_height*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::BuildMipSubLevels()
	{
		if (glloader_GL_EXT_framebuffer_object())
		{
			this->GLBindTexture();
			glGenerateMipmapEXT(target_type_);
		}
		else
		{
			THR(E_FAIL);
		}
	}

	void OGLTexture::CustomAttribute(std::string const & /*name*/, void* /*data*/)
	{
		BOOST_ASSERT(false);
	}

	void OGLTexture::GLBindTexture()
	{
		glBindTexture(target_type_, texture_);
	}

	void OGLTexture::Usage(Texture::TextureUsage usage)
	{
		usage_ = usage;
	}

	PixelFormat OGLTexture::SRGBToRGB(PixelFormat pf)
	{
		switch (pf)
		{
		case PF_ARGB8_SRGB:
			return PF_ARGB8;

		case PF_DXT1_SRGB:
			return PF_DXT1;

		case PF_DXT3_SRGB:
			return PF_DXT3;

		case PF_DXT5_SRGB:
			return PF_DXT5;

		default:
			return pf;
		}
	}
}
