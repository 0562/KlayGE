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
		void Attach(uint32_t /*att*/, RenderViewPtr /*view*/)
		{
		}
		void Detach(uint32_t /*att*/)
		{
		}
	};

	FrameBufferPtr FrameBuffer::NullObject()
	{
		static FrameBufferPtr obj(new NullFrameBuffer);
		return obj;
	}
}
