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
	OGLVertexStream::OGLVertexStream(BufferUsage usage)
			: VertexStream(usage)
	{
		glGenBuffers(1, &vb_);
	}

	OGLVertexStream::~OGLVertexStream()
	{
		glDeleteBuffers(1, &vb_);
	}

	void OGLVertexStream::DoCreate()
	{
		BOOST_ASSERT(size_in_byte_ != 0);

		glBindBuffer(GL_ARRAY_BUFFER, vb_);
		glBufferData(GL_ARRAY_BUFFER,
				reinterpret_cast<GLsizeiptr>(size_in_byte_), NULL,
				(BU_Static == usage_) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void* OGLVertexStream::Map(BufferAccess ba)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vb_);

		GLenum flag = 0;
		switch (ba)
		{
		case BA_Read_Only:
			flag = GL_READ_ONLY;
			break;

		case BA_Write_Only:
			flag = GL_WRITE_ONLY;
			break;

		case BA_Read_Write:
			flag = GL_READ_WRITE;
			break;
		}

		return glMapBuffer(GL_ARRAY_BUFFER, flag);
	}

	void OGLVertexStream::Unmap()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vb_);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void OGLVertexStream::Active()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vb_);
	}
}
