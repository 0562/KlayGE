// VertexBuffer.hpp
// KlayGE ���㻺������ ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ֧���ڵ����д洢���ֳ�Ա (2005.10.15)
//
// 2.8.0
// ������CopyToMemory (2005.7.24)
//
// 2.4.0
// ����ΪVertexBuffer (2005.3.7)
//
// 2.0.4
// �޸����������� (2004.3.16)
//
// 2.0.3
// ȥ����VO_2D (2004.3.1)
// ����vector������� (2004.3.13)
//
// 2.0.0
// ���ν��� (2003.8.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _VERTEXBUFFER_HPP
#define _VERTEXBUFFER_HPP

#include <KlayGE/PreDeclare.hpp>
#include <vector>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	enum VertexStreamType
	{
		// vertex positions
		VST_Positions = 1,
		// vertex normals included (for lighting)
		VST_Normals = 2,
		// Vertex colors - diffuse
		VST_Diffuses = 3,
		// Vertex colors - specular
		VST_Speculars = 4,
		// Vertex blend weights
		VST_BlendWeights = 5,
		// Vertex blend indices
		VST_BlendIndices = 6,
		// at least one set of texture coords (exact number specified in class)
		VST_TextureCoords0 = 7,
		VST_TextureCoords1 = 8,
		VST_TextureCoords2 = 9,
		VST_TextureCoords3 = 10,
		VST_TextureCoords4 = 11,
		VST_TextureCoords5 = 12,
		VST_TextureCoords6 = 13,
		VST_TextureCoords7 = 14
	};

	struct vertex_element
	{
		vertex_element(VertexStreamType type, uint8_t component_size, uint8_t num_components)
			: type(type), component_size(component_size), num_components(num_components)
		{
		}

		VertexStreamType type;
		// ��ʾԪ����ÿ���ɷֵĴ�С������PositionԪ�صĳɷ���size(float)
		uint8_t component_size;
		// ��ʾһ��Ԫ���м����ɷֱ�ʾ������PositionԪ������(x, y, z)��ɣ�����Ϊ3
		uint8_t num_components;

		uint16_t element_size() const
		{
			return component_size * num_components;
		}
	};
	typedef std::vector<vertex_element> vertex_elements_type;

	class VertexStream
	{
	public:
		VertexStream(vertex_elements_type const & vertex_elems);
		virtual ~VertexStream();

		virtual bool IsStatic() const = 0;

		virtual void Assign(void const * src, uint32_t numVertex) = 0;
		virtual void CopyToMemory(void* data) = 0;

		virtual uint32_t NumVertices() const = 0;

		uint32_t NumElements() const;
		vertex_element const & Element(uint32_t index) const;

		uint16_t VertexSize() const;
		uint32_t StreamSize() const;

	protected:
		vertex_elements_type vertex_elems_; 
	};

	class IndexStream
	{
	public:
		virtual ~IndexStream();

		virtual uint32_t NumIndices() const = 0;

		virtual bool IsStatic() const = 0;
		virtual void Assign(void const * src, uint32_t numIndices) = 0;
		virtual void CopyToMemory(void* data) = 0;

		uint32_t StreamSize() const;
	};


	class VertexBuffer
	{
	public:
		enum BufferType
		{
			BT_PointList,
			BT_LineList,
			BT_LineStrip,
			BT_TriangleList,
			BT_TriangleStrip,
			BT_TriangleFan
		};

		typedef std::vector<VertexStreamPtr> VertexStreamsType;
		typedef VertexStreamsType::iterator VertexStreamIterator;
		typedef VertexStreamsType::const_iterator VertexStreamConstIterator;

		explicit VertexBuffer(BufferType type);
		virtual ~VertexBuffer() = 0;

		BufferType Type() const;

		uint32_t NumVertices() const;

		void AddVertexStream(VertexStreamPtr vertex_stream);

		VertexStreamIterator VertexStreamBegin();
		VertexStreamIterator VertexStreamEnd();
		VertexStreamConstIterator VertexStreamBegin() const;
		VertexStreamConstIterator VertexStreamEnd() const;


		bool UseIndices() const;
		uint32_t NumIndices() const;

		void SetIndexStream(IndexStreamPtr index_stream);
		IndexStreamPtr GetIndexStream() const;

	protected:
		BufferType type_;

		VertexStreamsType vertexStreams_;
		IndexStreamPtr indexStream_;
	};
}

#endif		// _VERTEXBUFFER_HPP
