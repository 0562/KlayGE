// OGLIndexStream.cpp
// KlayGE OpenGL������������ ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ���ν��� (2004.4.3)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <algorithm>

#include <KlayGE/OpenGL/OGLIndexStream.hpp>

namespace KlayGE
{
	OGLIndexStream::OGLIndexStream()
	{
	}

	void OGLIndexStream::Assign(void const * src, size_t numIndices)
	{
		buffer_.assign(static_cast<U16 const *>(src),
			static_cast<U16 const *>(src) + numIndices);
	}
}
