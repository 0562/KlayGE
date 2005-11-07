// RenderVertexStream.cpp
// KlayGE ��Ⱦ���������� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/RenderVertexStream.hpp>

namespace KlayGE
{
	class NullRenderVertexStream : public RenderVertexStream
	{
	public:
		void Attach(VertexStreamPtr vs)
		{
		}
		void Detach()
		{
		}

		void CustomAttribute(std::string const & /*name*/, void* /*data*/)
		{
		}
		bool RequiresTextureFlipping() const
		{
			return false;
		}
	};

	RenderVertexStreamPtr RenderVertexStream::NullObject()
	{
		static RenderVertexStreamPtr obj(new NullRenderVertexStream);
		return obj;
	}
}
