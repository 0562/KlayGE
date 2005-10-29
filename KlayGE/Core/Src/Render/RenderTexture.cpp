// RenderTexture.cpp
// KlayGE ��Ⱦ�������� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderTexture.hpp>

namespace KlayGE
{
	class NullRenderTexture : public RenderTexture
	{
	public:
		void AttachTexture2D(TexturePtr texture2D)
		{
		}
		void AttachTextureCube(TexturePtr textureCube, Texture::CubeFaces face)
		{
		}
		void DetachTexture()
		{
		}

		void CustomAttribute(std::string const & name, void* pData)
		{
		}
		bool RequiresTextureFlipping() const
		{
			return true;
		}
	};

	RenderTexturePtr RenderTexture::NullObject()
	{
		static RenderTexturePtr obj(new NullRenderTexture);
		return obj;
	}
}
