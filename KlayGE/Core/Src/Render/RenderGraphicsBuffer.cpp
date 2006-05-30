// RenderGraphicsBuffer.hpp
// KlayGE ��Ⱦͼ�λ������� ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.4.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/RenderGraphicsBuffer.hpp>

namespace KlayGE
{
	class NullRenderGraphicsBuffer : public RenderGraphicsBuffer
	{
	public:
		void Attach(GraphicsBufferPtr vs)
		{
		}
		void Detach()
		{
		}

		bool RequiresTextureFlipping() const
		{
			return false;
		}
	};

	RenderGraphicsBufferPtr RenderGraphicsBuffer::NullObject()
	{
		static RenderGraphicsBufferPtr obj(new NullRenderGraphicsBuffer);
		return obj;
	}
}
