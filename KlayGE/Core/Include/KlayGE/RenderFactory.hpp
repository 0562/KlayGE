// RenderFactory.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ������1D/2D/3D/cube��֧�� (2005.3.8)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERFACTORY_HPP
#define _RENDERFACTORY_HPP

#include <KlayGE/Texture.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/RenderEffect.hpp>
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
		virtual TexturePtr MakeTexture1D(uint32_t width, uint16_t numMipMaps,
			PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default) = 0;
		virtual TexturePtr MakeTexture2D(uint32_t width, uint32_t height, uint16_t numMipMaps,
			PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default) = 0;
		virtual TexturePtr MakeTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint16_t numMipMaps,
			PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default) = 0;
		virtual TexturePtr MakeTextureCube(uint32_t size, uint16_t numMipMaps,
			PixelFormat format, Texture::TextureUsage usage = Texture::TU_Default) = 0;
		virtual RenderTexturePtr MakeRenderTexture() = 0;

		FontPtr MakeFont(std::string const & fontName, uint32_t fontHeight = 12, uint32_t flags = 0)
		{
			return FontPtr(new Font(fontName, fontHeight, flags));
		}

		virtual RenderEffectPtr MakeRenderEffect(std::string const & srcData) = 0;

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

	private:
		std::wstring const name_;
	};
}

#endif			// _RENDERFACTORY_HPP
