// VertexBuffer.hpp
// KlayGE ���㻺������ ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ֧���ڵ����д洢���ֳ�Ա (2005.10.15)
// ֧��instancing (2005.10.21)
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
	enum VertexElementUsage
	{
		// vertex positions
		VEU_Position,
		// vertex normals included (for lighting)
		VEU_Normal,
		// Vertex colors - diffuse
		VEU_Diffuse,
		// Vertex colors - specular
		VEU_Specular,
		// Vertex blend weights
		VEU_BlendWeight,
		// Vertex blend indices
		VEU_BlendIndex,
		// at least one set of texture coords (exact number specified in class)
		VEU_TextureCoord,
		// Vertex tangent
		VEU_Tangent,
		// Vertex binormal
		VEU_Binormal
	};

	struct vertex_element
	{
		vertex_element(VertexElementUsage usage, uint8_t usage_index, uint8_t component_size, uint8_t num_components)
			: usage(usage), usage_index(usage_index), component_size(component_size), num_components(num_components)
		{
		}

		VertexElementUsage usage;
		uint8_t usage_index;

		// ��ʾԪ����ÿ���ɷֵĴ�С������PositionԪ�صĳɷ���size(float)
		uint8_t component_size;
		// ��ʾһ��Ԫ���м����ɷֱ�ʾ������PositionԪ������(x, y, z)��ɣ�����Ϊ3
		uint8_t num_components;

		uint16_t element_size() const
		{
			return component_size * num_components;
		}

		friend bool
		operator==(vertex_element const & lhs, vertex_element const & rhs)
		{
			return (lhs.usage == rhs.usage)
				&& (lhs.usage_index == rhs.usage_index)
				&& (lhs.component_size == rhs.component_size)
				&& (lhs.num_components == rhs.num_components);
		}
	};
	typedef std::vector<vertex_element> vertex_elements_type;

	class VertexStream
	{
	public:
		enum stream_type
		{
			ST_Geometry,
			ST_Instance
		};

	public:
		VertexStream(vertex_elements_type const & vertex_elems);
		virtual ~VertexStream();

		virtual bool IsStatic() const = 0;

		virtual void Assign(void const * src, uint32_t numVertex) = 0;
		virtual void CopyToMemory(void* data) = 0;
		void CopyToMemory(void* data, vertex_elements_type const & rhs_vertex_elems);
		void CopyToStream(VertexStream& rhs);

		virtual uint32_t NumVertices() const = 0;

		uint32_t NumElements() const;
		vertex_element const & Element(uint32_t index) const;
		vertex_elements_type const & Elements() const;

		uint16_t VertexSize() const;
		uint32_t StreamSize() const;

		VertexStream& Combine(VertexStreamPtr rhs);
		VertexStream& Append(VertexStreamPtr rhs);

		void FrequencyDivider(stream_type type, uint32_t freq);
		stream_type StreamType() const;
		uint32_t Frequency() const;

	protected:
		void RefreshVertexSize();
		void CheckVertexElems();

	protected:
		vertex_elements_type vertex_elems_;
		uint16_t vertex_size_;

		stream_type type_;
		uint32_t freq_;
	};

	class IndexStream
	{
	public:
		virtual ~IndexStream();

		virtual uint32_t NumIndices() const = 0;

		virtual bool IsStatic() const = 0;
		virtual void Assign(void const * src, uint32_t numIndices) = 0;
		virtual void CopyToMemory(void* data) = 0;
		void CopyToStream(IndexStream& rhs);

		uint32_t StreamSize() const;

		IndexStream& Append(IndexStreamPtr rhs, uint16_t base_index);
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

		uint32_t NumInstance() const;

		VertexBufferPtr ExpandInstance(uint32_t inst_no) const;
		bool HasInstanceStream() const;

		VertexBuffer& Append(VertexBufferPtr rhs);

	protected:
		BufferType type_;

		VertexStreamsType vertexStreams_;
		IndexStreamPtr indexStream_;
	};
}

#endif		// _VERTEXBUFFER_HPP
