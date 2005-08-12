// OGLIndexStream.cpp
// KlayGE OpenGL������������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������CopyToMemory (2005.7.24)
// ֻ֧��vbo (2005.7.31)
// ֻ֧��OpenGL 1.5������ (2005.8.12)
//
// 2.7.0
// ֧��vertex_buffer_object (2005.6.19)
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
	OGLIndexStream::OGLIndexStream(bool staticStream)
		: static_stream_(staticStream)
	{
		glGenBuffers(1, &ib_);
	}

	OGLIndexStream::~OGLIndexStream()
	{
		glDeleteBuffers(1, &ib_);
	}

	void OGLIndexStream::Assign(void const * src, size_t numIndices)
	{
		numIndices_ = numIndices;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				reinterpret_cast<GLsizeiptr>(numIndices * sizeof(uint16_t)), src,
				this->IsStatic() ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void OGLIndexStream::CopyToMemory(void* data)
	{
		uint16_t* destPtr = static_cast<uint16_t*>(data);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);

		uint16_t* srcPtr = static_cast<uint16_t*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,
				GL_READ_ONLY | (this->IsStatic() ? GL_STATIC_READ : GL_DYNAMIC_READ)));

		std::copy(srcPtr, srcPtr + this->NumIndices(), destPtr);

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	void OGLIndexStream::Active()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);
	}
}
