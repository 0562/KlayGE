// RenderLayout.hpp
// KlayGE ��Ⱦ�������� ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.1.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERLAYOUT_HPP
#define _RENDERLAYOUT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <vector>
#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>
#include <KlayGE/GraphicsBuffer.hpp>

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
		vertex_element()
		{
		}
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

	enum IndexFormat
	{
		IF_Index32,
		IF_Index16
	};


	class RenderLayout
	{
	public:
		enum buffer_type
		{
			BT_PointList,
			BT_LineList,
			BT_LineStrip,
			BT_TriangleList,
			BT_TriangleStrip,
			BT_TriangleFan
		};

		enum stream_type
		{
			ST_Geometry,
			ST_Instance
		};

		explicit RenderLayout(buffer_type type);
		virtual ~RenderLayout() = 0;

		static RenderLayoutPtr NullObject();

		buffer_type Type() const;

		uint32_t NumVertices() const;

		template <typename tuple_type>
		void BindVertexStream(GraphicsBufferPtr buffer, tuple_type const & vertex_elems,
			stream_type type = ST_Geometry, uint32_t freq = 1)
		{
			this->BindVertexStream(buffer, Tuple2Vector(vertex_elems), type, freq);
		}
		void BindVertexStream(GraphicsBufferPtr buffer, vertex_elements_type const & vet,
			stream_type type = ST_Geometry, uint32_t freq = 1);

		uint32_t NumVertexStreams() const
		{
			return static_cast<uint32_t>(vertex_streams_.size());
		}
		GraphicsBufferPtr GetVertexStream(uint32_t index) const
		{
			return vertex_streams_[index].stream;
		}
		vertex_elements_type const & VertexStreamFormat(uint32_t index) const
		{
			return vertex_streams_[index].format;
		}
		uint32_t VertexSize(uint32_t index) const
		{
			return vertex_streams_[index].vertex_size;
		}
		stream_type VertexStreamType(uint32_t index) const
		{
			return vertex_streams_[index].type;
		}
		uint32_t VertexStreamFrequency(uint32_t index) const
		{
			return vertex_streams_[index].freq;
		}
		void VertexStreamFrequencyDivider(uint32_t index, stream_type type, uint32_t freq)
		{
			vertex_streams_[index].type = type;
			vertex_streams_[index].freq = freq;
		}

		bool UseIndices() const;
		uint32_t NumIndices() const;

		void BindIndexStream(GraphicsBufferPtr index_stream, IndexFormat format);
		GraphicsBufferPtr GetIndexStream() const;
		IndexFormat IndexStreamFormat() const
		{
			return index_format_;
		}

		GraphicsBufferPtr InstanceStream() const;
		vertex_elements_type const & InstanceStreamFormat() const
		{
			return instance_stream_.format;
		}
		uint32_t InstanceSize() const
		{
			return instance_stream_.vertex_size;
		}
		uint32_t NumInstance() const;

		void ExpandInstance(GraphicsBufferPtr& hint, uint32_t inst_no) const;

	private:
		template <typename tuple_type>
		vertex_elements_type Tuple2Vector(tuple_type const & t)
		{
			vertex_elements_type ret;
			ret.push_back(boost::tuples::get<0>(t));

			vertex_elements_type tail(Tuple2Vector(t.get_tail()));
			ret.insert(ret.end(), tail.begin(), tail.end());

			return ret;
		}
		template <>
		vertex_elements_type Tuple2Vector<boost::tuples::null_type>(boost::tuples::null_type const & /*t*/)
		{
			return vertex_elements_type();
		}

	protected:
		buffer_type type_;

		struct StreamUnit
		{
			GraphicsBufferPtr stream;
			vertex_elements_type format;
			uint32_t vertex_size;

			stream_type type;
			uint32_t freq;
		};

		std::vector<StreamUnit> vertex_streams_;
		StreamUnit instance_stream_;

		GraphicsBufferPtr index_stream_;
		IndexFormat index_format_;
	};
}

#endif		// _RENDERLAYOUT_HPP
