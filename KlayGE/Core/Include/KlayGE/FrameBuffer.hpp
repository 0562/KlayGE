// FrameBuffer.hpp
// KlayGE ��Ⱦ�������� ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ��ΪFrameBuffer (2006.5.30)
//
// 2.8.0
// ȥ����GetTexture (2005.7.19)
//
// 2.4.0
// ������IsTexture��SwapBuffers (2005.3.6)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _FRAMEBUFFER_HPP
#define _FRAMEBUFFER_HPP

#include <KlayGE/PreDeclare.hpp>
#include <vector>
#include <KlayGE/RenderTarget.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{	
	class FrameBuffer : public RenderTarget
	{
	public:
		static FrameBufferPtr NullObject();

		virtual void AttachTexture2D(uint32_t n, TexturePtr texture2D) = 0;
		virtual void AttachTextureCube(uint32_t n, TexturePtr textureCube, Texture::CubeFaces face) = 0;

		virtual void AttachGraphicsBuffer(uint32_t n, GraphicsBufferPtr gb,
			uint32_t width, uint32_t height) = 0;

		virtual void Detach(uint32_t n) = 0;

		void SwapBuffers()
		{
		}

	protected:
		std::vector<TexturePtr> privateTexs_;
		std::vector<Texture::CubeFaces> faces_;

		std::vector<GraphicsBufferPtr> gbuffers_;
	};
}

#endif			// _FRAMEBUFFER_HPP
