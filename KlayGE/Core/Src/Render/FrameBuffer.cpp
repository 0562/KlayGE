// FrameBuffer.cpp
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
#include <KlayGE/FrameBuffer.hpp>

namespace KlayGE
{
	class NullFrameBuffer : public FrameBuffer
	{
	public:
		void AttachTexture2D(uint32_t /*n*/, TexturePtr /*texture2D*/)
		{
		}
		void AttachTextureCube(uint32_t /*n*/, TexturePtr /*textureCube*/, Texture::CubeFaces /*face*/)
		{
		}
		void AttachGraphicsBuffer(uint32_t /*n*/, GraphicsBufferPtr /*gb*/,
			uint32_t /*width*/, uint32_t /*height*/)
		{
		}
		void Detach(uint32_t /*n*/)
		{
		}
	};

	FrameBufferPtr FrameBuffer::NullObject()
	{
		static FrameBufferPtr obj(new NullFrameBuffer);
		return obj;
	}
}
