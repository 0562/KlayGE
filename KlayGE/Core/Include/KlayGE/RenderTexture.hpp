// RenderTexture.hpp
// KlayGE ��Ⱦ������ ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ������IsTexture��SwapBuffers (2005.3.6)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERTEXTURE_HPP
#define _RENDERTEXTURE_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderTarget.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{	
	class RenderTexture : public RenderTarget
	{
	public:
		TexturePtr const & GetTexture() const
			{ return privateTex_; }
		TexturePtr& GetTexture()
			{ return privateTex_; }

		void SwapBuffers()
		{
		}

		bool IsTexture() const
		{
			return true;
		}

	protected:
		TexturePtr privateTex_;
	};
}

#endif			// _RENDERTEXTURE_HPP