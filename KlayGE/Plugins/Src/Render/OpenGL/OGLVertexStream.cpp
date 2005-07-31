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
	OGLVertexStream::OGLVertexStream(VertexStreamType type, uint8_t sizeElement, uint8_t numElement, bool staticStream)
			: VertexStream(type, sizeElement, numElement),
				static_stream_(staticStream)
	{
		if (glloader_GL_VERSION_1_5())
		{
			glBindBuffer_			= glBindBuffer;
			glBufferData_			= glBufferData;
			glBufferSubData_		= glBufferSubData;
			glDeleteBuffers_		= glDeleteBuffers;
			glGenBuffers_			= glGenBuffers;
			glGetBufferParameteriv_	= glGetBufferParameteriv;
			glGetBufferPointerv_	= glGetBufferPointerv;
			glGetBufferSubData_		= glGetBufferSubData;
			glIsBuffer_				= glIsBuffer;
			glMapBuffer_			= glMapBuffer;
			glUnmapBuffer_			= glUnmapBuffer;
		}
		else
		{
			if (glloader_GL_ARB_vertex_buffer_object())
			{
				glBindBuffer_			= glBindBufferARB;
				glBufferData_			= glBufferDataARB;
				glBufferSubData_		= glBufferSubDataARB;
				glDeleteBuffers_		= glDeleteBuffersARB;
				glGenBuffers_			= glGenBuffersARB;
				glGetBufferParameteriv_	= glGetBufferParameterivARB;
				glGetBufferPointerv_	= glGetBufferPointervARB;
				glGetBufferSubData_		= glGetBufferSubDataARB;
				glIsBuffer_				= glIsBufferARB;
				glMapBuffer_			= glMapBufferARB;
				glUnmapBuffer_			= glUnmapBufferARB;
			}
			else
			{
				THR(E_FAIL);
			}
		}

		glGenBuffers_(1, &vb_);
	}

	OGLVertexStream::~OGLVertexStream()
	{
		glDeleteBuffers_(1, &vb_);
	}

	void OGLVertexStream::Assign(void const * src, size_t numVertices, size_t stride)
	{
		size_t const vertexSize(this->SizeOfElement() * this->ElementsPerVertex());
		size_t const size(vertexSize * numVertices);

		numVertices_ = numVertices;

		glBindBuffer_(GL_ARRAY_BUFFER, vb_);
		glBufferData_(GL_ARRAY_BUFFER,
			reinterpret_cast<GLsizeiptr>(size), src, this->IsStatic() ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void OGLVertexStream::CopyToMemory(void* data)
	{
		size_t const size(this->SizeOfElement() * this->ElementsPerVertex() * this->NumVertices());
		uint8_t* destPtr = static_cast<uint8_t*>(data);

		glBindBuffer_(GL_ARRAY_BUFFER, vb_);

		uint8_t* srcPtr = static_cast<uint8_t*>(glMapBuffer_(GL_ARRAY_BUFFER,
			GL_READ_ONLY | (this->IsStatic() ? GL_STATIC_READ : GL_DYNAMIC_READ)));

		std::copy(srcPtr, srcPtr + size, destPtr);

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void OGLVertexStream::Active()
	{
		glBindBuffer_(GL_ARRAY_BUFFER, vb_);
	}
}
