// OGLVertexStream.h
// KlayGE OpenGL������������ ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ���ν��� (2004.4.3)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLVERTEXSTREAM_HPP
#define _OGLVERTEXSTREAM_HPP

#include <vector>

#include <KlayGE/RenderBuffer.hpp>

namespace KlayGE
{
	class OGLVertexStream : public VertexStream
	{
	public:
		OGLVertexStream(VertexStreamType type, U8 sizeElement, U8 numElement);

		bool IsStatic() const
			{ return false; }
		size_t NumVertices() const
			{ return buffer_.size() / this->sizeElement() / this->ElementsPerVertex(); }

		void Assign(void const * src, size_t numVertices, size_t stride = 0);

		std::vector<U8> const & OGLBuffer() const
			{ return buffer_; }

	protected:
		std::vector<U8> buffer_;
	};
}

#endif			// _OGLVERTEXSTREAM_HPP
