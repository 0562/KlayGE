#ifndef _RENDERFACTORY_HPP
#define _RENDERFACTORY_HPP

#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderBuffer.hpp>
#include <KlayGE/Font.hpp>

#include <boost/utility.hpp>

namespace KlayGE
{
	class RenderFactory
	{
	public:
		virtual ~RenderFactory()
			{ }

		virtual std::wstring const & Name() const = 0;

		virtual RenderEngine& RenderEngineInstance() = 0;
		virtual TexturePtr MakeTexture(uint32_t width, uint32_t height, uint16_t numMipMaps,
			PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default) = 0;
		virtual RenderTexturePtr MakeRenderTexture(uint32_t width, uint32_t height) = 0;

		FontPtr MakeFont(std::string const & fontName, uint32_t fontHeight = 12, uint32_t flags = 0)
		{
			return FontPtr(new Font(fontName, fontHeight, flags));
		}

		virtual RenderEffectPtr MakeRenderEffect(std::string const & srcData, uint32_t flags = 0) = 0;

		// sizeElement��ʾ����ÿ��Ԫ�صĴ�С������Position����size(float)
		// numElement��ʾһ�������м���Ԫ�ر�ʾ������Position������(x, y, z)��ɣ�����Ϊ3
		virtual VertexStreamPtr MakeVertexStream(VertexStreamType type,
			uint8_t sizeElement, uint8_t numElement, bool staticStream = false) = 0;
		virtual IndexStreamPtr MakeIndexStream(bool staticStream = false) = 0;
	};

	template <typename RenderEngineType, typename TextureType, typename RenderTextureType,
		typename RenderEffectType>
	class ConcreteRenderFactory : boost::noncopyable, public RenderFactory
	{
	public:
		ConcreteRenderFactory(std::wstring const & name)
				: name_(name)
			{ }
		virtual ~ConcreteRenderFactory()
			{ }

		std::wstring const & Name() const
			{ return name_; }

		RenderEngine& RenderEngineInstance()
		{
			static RenderEngineType renderEngine;
			return renderEngine;
		}

		TexturePtr MakeTexture(uint32_t width, uint32_t height, uint16_t numMipMaps,
				PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default)
			{ return TexturePtr(new TextureType(width, height, numMipMaps, format, usage)); }

		RenderTexturePtr MakeRenderTexture(uint32_t width, uint32_t height)
			{ return RenderTexturePtr(new RenderTextureType(width, height)); }

		RenderEffectPtr MakeRenderEffect(std::string const & srcData, uint32_t flags)
			{ return RenderEffectPtr(new RenderEffectType(srcData, flags)); }

	private:
		std::wstring const name_;
	};
}

#endif			// _RENDERFACTORY_HPP
