// OGLIndexStream.hpp
// KlayGE OpenGL������������ ͷ�ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// ֧��vertex_buffer_object (2005.6.19)
//
// 2.0.4
// ���ν��� (2004.4.3)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLINDEXSTREAM_HPP
#define _OGLINDEXSTREAM_HPP

#include <vector>
#include <glloader/glloader.h>

#include <KlayGE/VertexBuffer.hpp>

namespace KlayGE
{
	class OGLIndexStream : public IndexStream
	{
	public:
		OGLIndexStream(bool staticStream);
		~OGLIndexStream();

		bool IsStatic() const
		{
			return static_stream_;
		}
		bool UseVBO() const
		{
			return use_vbo_;
		}
		size_t NumIndices() const
		{
			return buffer_.size();
		}

		void Assign(void const * src, size_t numIndices);
		void Active();

		std::vector<uint16_t> const & OGLBuffer() const
		{
			return buffer_;
		}
		GLuint OGLvbo() const
		{
			return ib_;
		}

	protected:
		std::vector<uint16_t> buffer_;

		bool use_vbo_;
		bool static_stream_;

		glBindBufferARBFUNC glBindBuffer_;
		glBufferDataARBFUNC glBufferData_;
		glBufferSubDataARBFUNC glBufferSubData_;
		glDeleteBuffersARBFUNC glDeleteBuffers_;
		glGenBuffersARBFUNC glGenBuffers_;
		glGetBufferParameterivARBFUNC glGetBufferParameteriv_;
		glGetBufferPointervARBFUNC glGetBufferPointerv_;
		glGetBufferSubDataARBFUNC glGetBufferSubData_;
		glIsBufferARBFUNC glIsBuffer_;
		glMapBufferARBFUNC glMapBuffer_;
		glUnmapBufferARBFUNC glUnmapBuffer_;

		GLuint ib_;
	};
}

#endif			// _OGLINDEXSTREAM_HPP
