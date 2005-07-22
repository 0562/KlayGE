// OGLRenderVertexStream.hpp
// KlayGE OpenGL��Ⱦ���������� ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <glloader/glloader.h>

#include <KlayGE/OpenGL/OGLVertexStream.hpp>
#include <KlayGE/OpenGL/OGLRenderVertexStream.hpp>

namespace KlayGE
{
	OGLRenderVertexStream::OGLRenderVertexStream()
	{
		left_ = 0;
		top_ = 0;
	}

	void OGLRenderVertexStream::CustomAttribute(std::string const & /*name*/, void* /*data*/)
	{
		assert(false);
	}

	void OGLRenderVertexStream::Attach(VertexStreamPtr vs)
	{
		assert(false);
	}

	void OGLRenderVertexStream::Detach()
	{
		assert(false);
	}
}
