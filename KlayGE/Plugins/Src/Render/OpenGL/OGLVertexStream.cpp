// OGLVertexStream.cpp
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

#include <KlayGE/OpenGL/OGLVertexStream.hpp>

namespace KlayGE
{
	OGLVertexStream::OGLVertexStream(VertexStreamType type, U8 sizeElement, U8 numElement)
			: VertexStream(type, sizeElement, numElement)
	{
	}

	void OGLVertexStream::Assign(void const * src, size_t numVertices, size_t stride)
	{
		size_t const vertexSize(this->sizeElement() * this->ElementsPerVertex());
		size_t const size(vertexSize * numVertices);

		buffer_.resize(size);

		if (stride != 0)
		{
			U8* destPtr(&buffer_[0]);
			U8 const * srcPtr(static_cast<U8 const *>(static_cast<void const *>(src)));
			for (size_t i = 0; i < numVertices; ++ i)
			{
				MemoryLib::Copy(destPtr, srcPtr, vertexSize);

				destPtr += vertexSize;
				srcPtr += vertexSize + stride;
			}
		}
		else
		{
			MemoryLib::Copy(&buffer_[0], src, size);
		}
	}
}
