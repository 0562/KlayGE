// OGLVertexStream.cpp
// KlayGE OpenGL������������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������CopyToMemory (2005.7.24)
// ֻ֧��vbo (2005.7.31)
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

#include <KlayGE/OpenGL/OGLVertexStream.hpp>

namespace KlayGE
{
	OGLVertexStream::OGLVertexStream(vertex_elements_type const & vertex_elems, bool staticStream)
			: VertexStream(vertex_elems),
				static_stream_(staticStream)
	{
		glGenBuffers(1, &vb_);
	}

	OGLVertexStream::~OGLVertexStream()
	{
		glDeleteBuffers(1, &vb_);
	}

	void OGLVertexStream::Assign(void const * src, uint32_t numVertices)
	{
		numVertices_ = numVertices;

		glBindBuffer(GL_ARRAY_BUFFER, vb_);
		glBufferData(GL_ARRAY_BUFFER,
			reinterpret_cast<GLsizeiptr>(this->StreamSize()), src, this->IsStatic() ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void OGLVertexStream::CopyToMemory(void* data)
	{
		uint8_t* destPtr = static_cast<uint8_t*>(data);

		glBindBuffer(GL_ARRAY_BUFFER, vb_);

		uint8_t* srcPtr = static_cast<uint8_t*>(glMapBuffer(GL_ARRAY_BUFFER,
			GL_READ_ONLY | (this->IsStatic() ? GL_STATIC_READ : GL_DYNAMIC_READ)));

		std::copy(srcPtr, srcPtr + this->StreamSize(), destPtr);

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void OGLVertexStream::Active()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vb_);
	}
}
