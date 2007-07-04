// OGLTexture.hpp
// KlayGE OpenGL������ ͷ�ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2003-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ��pbo���� (2007.3.13)
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

#ifndef _OGLTEXTURE_HPP
#define _OGLTEXTURE_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_OpenGL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/Texture.hpp>

#include <windows.h>
#include <glloader/glloader.h>

namespace KlayGE
{
	class OGLTexture : public Texture
	{
	public:
		OGLTexture(TextureType type);
		virtual ~OGLTexture();

		std::wstring const & Name() const;

		virtual uint32_t Width(int level) const;
		virtual uint32_t Height(int level) const;
		virtual uint32_t Depth(int level) const;

		virtual void CopyToTexture1D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset);
		virtual void CopyToTexture2D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);
		virtual void CopyToTexture3D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset);
		virtual void CopyToTextureCube(Texture& target, CubeFaces face, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);

		virtual void CopyToMemory1D(int level, void* data);
		virtual void CopyToMemory2D(int level, void* data);
		virtual void CopyToMemory3D(int level, void* data);
		virtual void CopyToMemoryCube(CubeFaces face, int level, void* data);

		virtual void CopyMemoryToTexture1D(int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset);
		virtual void CopyMemoryToTexture2D(int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset,
			uint32_t src_row_pitch);
		virtual void CopyMemoryToTexture3D(int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset,
			uint32_t src_row_pitch, uint32_t src_slice_pitch);
		virtual void CopyMemoryToTextureCube(CubeFaces face, int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset,
			uint32_t src_row_pitch);

		void BuildMipSubLevels();

		using Texture::Usage;
		void Usage(TextureUsage usage);

		void GLBindTexture();
		GLuint GLTexture() const
		{
			return texture_;
		}
		GLenum GLType() const
		{
			return target_type_;
		}

	protected:
		ElementFormat SRGBToRGB(ElementFormat pf);

	protected:
		GLuint texture_;
		GLuint pbo_;
		GLenum target_type_;
	};

	typedef boost::shared_ptr<OGLTexture> OGLTexturePtr;


	class OGLTexture1D : public OGLTexture
	{
	public:
		OGLTexture1D(uint32_t width, uint16_t numMipMaps, ElementFormat format);

		uint32_t Width(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTexture1D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset);

		void CopyToMemory1D(int level, void* data);

		void CopyMemoryToTexture1D(int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset);

	private:
		void UpdateParams();

	private:
		std::vector<uint32_t> widths_;
	};

	class OGLTexture2D : public OGLTexture
	{
	public:
		OGLTexture2D(uint32_t width, uint32_t height, uint16_t numMipMaps, ElementFormat format);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTexture2D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);

		void CopyToMemory2D(int level, void* data);

		void CopyMemoryToTexture2D(int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset,
			uint32_t src_row_pitch);

	private:
		void UpdateParams();

	private:
		std::vector<uint32_t> widths_;
		std::vector<uint32_t> heights_;
	};

	class OGLTexture3D : public OGLTexture
	{
	public:
		OGLTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint16_t numMipMaps, ElementFormat format);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;
		uint32_t Depth(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTexture3D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset);

		void CopyToMemory3D(int level, void* data);

		void CopyMemoryToTexture3D(int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset,
			uint32_t src_row_pitch, uint32_t src_slice_pitch);

	private:
		void UpdateParams();

	private:
		std::vector<uint32_t> widths_;
		std::vector<uint32_t> heights_;
		std::vector<uint32_t> depths_;
	};

	class OGLTextureCube : public OGLTexture
	{
	public:
		OGLTextureCube(uint32_t size, uint16_t numMipMaps, ElementFormat format);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTextureCube(Texture& target, CubeFaces face, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);

		void CopyToMemoryCube(CubeFaces face, int level, void* data);

		void CopyMemoryToTextureCube(CubeFaces face, int level, void const * data, ElementFormat pf,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset,
			uint32_t src_row_pitch);

	private:
		void UpdateParams();

	private:
		std::vector<uint32_t> widths_;
	};
}

#endif			// _OGLTEXTURE_HPP
