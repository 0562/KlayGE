#ifndef _RENDERFACTORY_HPP
#define _RENDERFACTORY_HPP

#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderBuffer.hpp>

namespace KlayGE
{
	class RenderFactory
	{
	public:
		virtual ~RenderFactory()
			{ }

		virtual std::wstring const & Name() const = 0;

		virtual RenderEngine& RenderEngineInstance() = 0;
		virtual TexturePtr MakeTexture(U32 width, U32 height, U16 numMipMaps,
			PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default) = 0;
		virtual RenderTexturePtr MakeRenderTexture(U32 width, U32 height) = 0;

		virtual FontPtr MakeFont(std::wstring const & fontName, U32 fontHeight = 12, U32 flags = 0) = 0;

		virtual RenderEffectPtr MakeRenderEffect(std::string const & srcData, UINT flags = 0) = 0;

		// sizeElement��ʾ����ÿ��Ԫ�صĴ�С������Position����size(float)
		// numElement��ʾһ�������м���Ԫ�ر�ʾ������Position������(x, y, z)��ɣ�����Ϊ3
		virtual VertexStreamPtr MakeVertexStream(VertexStreamType type,
			U8 sizeElement, U8 numElement, bool staticStream = false) = 0;
		virtual IndexStreamPtr MakeIndexStream(bool staticStream = false) = 0;
	};

	template <typename RenderEngineType, typename TextureType, typename RenderTextureType,
		typename FontType, typename RenderEffectType>
	class ConcreteRenderFactory : public RenderFactory
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

		TexturePtr MakeTexture(U32 width, U32 height, U16 numMipMaps,
				PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default)
			{ return TexturePtr(new TextureType(width, height, numMipMaps, format, usage)); }

		RenderTexturePtr MakeRenderTexture(U32 width, U32 height)
			{ return RenderTexturePtr(new RenderTextureType(width, height)); }

		FontPtr MakeFont(std::wstring const & fontName, U32 fontHeight = 12, U32 flags = 0)
			{ return FontPtr(new FontType(fontName, fontHeight, flags)); }

		RenderEffectPtr MakeRenderEffect(std::string const & srcData, UINT flags = 0)
			{ return RenderEffectPtr(new RenderEffectType(srcData, flags)); }

	private:
		std::wstring const name_;
	};
}

#endif			// _RENDERFACTORY_HPP
