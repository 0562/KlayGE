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
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <algorithm>

#include <KlayGE/OpenGL/OGLVertexStream.hpp>

namespace KlayGE
{
	OGLVertexStream::OGLVertexStream(VertexStreamType type, uint8 sizeElement, uint8 numElement)
			: VertexStream(type, sizeElement, numElement)
	{
	}

	void OGLVertexStream::Assign(void const * src, size_t numVertices, size_t stride)
	{
		size_t const vertexSize(this->sizeElement() * this->ElementsPerVertex());
		size_t const size(vertexSize * numVertices);

		buffer_.resize(size);

		uint8* destPtr(&buffer_[0]);
		uint8 const * srcPtr(static_cast<uint8 const *>(src));

		if (stride != 0)
		{
			for (size_t i = 0; i < numVertices; ++ i)
			{
				std::copy(srcPtr, srcPtr + vertexSize, destPtr);

				destPtr += vertexSize;
				srcPtr += vertexSize + stride;
			}
		}
		else
		{
			std::copy(srcPtr, srcPtr + size, destPtr);
		}
	}
}
