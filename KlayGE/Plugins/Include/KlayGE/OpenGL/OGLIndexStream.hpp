// OGLIndexStream.cpp
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

#ifndef _OGLINDEXSTREAM_HPP
#define _OGLINDEXSTREAM_HPP

#include <vector>

#include <KlayGE/RenderBuffer.hpp>

namespace KlayGE
{
	class OGLIndexStream : public IndexStream
	{
	public:
		OGLIndexStream();

		bool IsStatic() const
			{ return false; }

		void Assign(const void* src, size_t numIndices);
		
		size_t NumIndices() const
			{ return buffer_.size(); }

		const std::vector<U16, alloc<U16> >& OGLBuffer() const
			{ return buffer_; }

	protected:
		std::vector<U16, alloc<U16> > buffer_;
	};
}

#endif			// _OGLINDEXSTREAM_HPP
