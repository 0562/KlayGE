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
#include <KlayGE/Memory.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Engine.hpp>

#include <KlayGE/OpenGL/OGLIndexStream.hpp>

namespace KlayGE
{
	OGLIndexStream::OGLIndexStream(bool staticStream)
						: staticStream_(staticStream)
	{
	}

	void OGLIndexStream::Assign(const void* src, size_t numIndices)
	{
		buffer_.resize(numIndices);
		Engine::MemoryInstance().Cpy(&buffer_[0], src, numIndices * sizeof(U16));
	}
}
