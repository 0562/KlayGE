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
	OGLIndexStream::OGLIndexStream(BufferUsage usage)
		: IndexStream(usage)
	{
		glGenBuffers(1, &ib_);
	}

	OGLIndexStream::~OGLIndexStream()
	{
		glDeleteBuffers(1, &ib_);
	}

	void OGLIndexStream::DoCreate()
	{
		BOOST_ASSERT(size_in_byte_ != 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				reinterpret_cast<GLsizeiptr>(size_in_byte_), NULL,
				(BU_Static == usage_) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void* OGLIndexStream::Map(BufferAccess ba)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);

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

		return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, flag);
	}

	void OGLIndexStream::Unmap()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	void OGLIndexStream::Active()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);
	}
}
