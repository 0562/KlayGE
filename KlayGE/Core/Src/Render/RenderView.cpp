// RenderView.cpp
// KlayGE ��Ⱦ��ͼ�� ʵ���ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ���ν��� (2006.5.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/RenderView.hpp>

namespace KlayGE
{
	class NullRenderView : public RenderView
	{
	public:
		void OnAttached(FrameBuffer& /*fb*/, uint32_t /*att*/)
		{
		}

		void OnDetached(FrameBuffer& /*fb*/, uint32_t /*att*/)
		{
		}
	};

	RenderViewPtr RenderView::NullObject()
	{
		static RenderViewPtr obj(new NullRenderView);
		return obj;
	}
}
