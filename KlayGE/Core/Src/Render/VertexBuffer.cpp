// VertexBuffer.cpp
// KlayGE ���㻺������ ʵ���ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ֧��32λ��IndexStream (2006.1.4)
//
// 3.1.0
// ������ʵ���ͼ����� (2005.10.31)
//
// 3.0.0
// ֧��instancing (2005.10.22)
//
// 2.4.0
// ����ΪVertexBuffer (2005.3.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/MapVector.hpp>

#include <boost/bind.hpp>

#include <KlayGE/VertexBuffer.hpp>

namespace KlayGE
{
	class NullVertexStream : public VertexStream
	{
	public:
		NullVertexStream()
			: VertexStream(BU_Static)
		{
		}

		void* Map(BufferAccess /*ba*/)
		{
			return NULL;
		}

		void Unmap()
		{
		}

		void DoCreate()
		{
		}
	};

	VertexStream::VertexStream(BufferUsage usage)
			: usage_(usage), size_in_byte_(0)
	{
	}

	VertexStream::~VertexStream()
	{
	}

	VertexStreamPtr VertexStream::NullObject()
	{
		static VertexStreamPtr obj(new NullVertexStream);
		return obj;
	}

	void VertexStream::Resize(uint32_t size_in_byte)
	{
		size_in_byte_ = size_in_byte;
		this->DoCreate();
	}

	void VertexStream::CopyToStream(VertexStream& rhs)
	{
		VertexStream::Mapper lhs_mapper(*this, BA_Read_Only);
		VertexStream::Mapper rhs_mapper(rhs, BA_Write_Only);
		std::copy(lhs_mapper.Pointer<uint8_t>(), lhs_mapper.Pointer<uint8_t>() + size_in_byte_,
			rhs_mapper.Pointer<uint8_t>());
	}


	class NullIndexStream : public IndexStream
	{
	public:
		NullIndexStream()
			: IndexStream(BU_Static)
		{
		}

		void* Map(BufferAccess /*ba*/)
		{
			return NULL;
		}
		void Unmap()
		{
		}

	private:
		void DoCreate()
		{
		}
	};

	IndexStream::IndexStream(BufferUsage usage)
		: usage_(usage)
	{
	}

	IndexStream::~IndexStream()
	{
	}

	IndexStreamPtr IndexStream::NullObject()
	{
		static IndexStreamPtr obj(new NullIndexStream);
		return obj;
	}

	void IndexStream::Resize(uint32_t size_in_byte)
	{
		size_in_byte_ = size_in_byte;
		this->DoCreate();
	}

	void IndexStream::CopyToStream(IndexStream& rhs)
	{
		IndexStream::Mapper lhs_mapper(*this, BA_Read_Only);
		IndexStream::Mapper rhs_mapper(rhs, BA_Write_Only);
		std::copy(lhs_mapper.Pointer<uint8_t>(), lhs_mapper.Pointer<uint8_t>() + size_in_byte_,
			rhs_mapper.Pointer<uint8_t>());
	}


	class NullRenderLayout : public RenderLayout
	{
	public:
		NullRenderLayout()
			: RenderLayout(BT_PointList)
		{
		}
	};

	RenderLayout::RenderLayout(buffer_type type)
			: type_(type)
	{
		vertex_streams_.reserve(8);
	}

	RenderLayout::~RenderLayout()
	{
	}

	RenderLayoutPtr RenderLayout::NullObject()
	{
		static RenderLayoutPtr obj(new NullRenderLayout);
		return obj;
	}

	RenderLayout::buffer_type RenderLayout::Type() const
	{
		return type_;
	}

	uint32_t RenderLayout::NumVertices() const
	{
		return vertex_streams_.empty() ? 0 : (vertex_streams_[0].stream->Size() / vertex_streams_[0].vertex_size);
	}

	void RenderLayout::AddVertexStream(VertexStreamPtr vertex_stream, vertex_elements_type const & vet,
		stream_type type, uint32_t freq)
	{
		BOOST_ASSERT(vertex_stream);

		uint32_t size = 0;
		for (size_t i = 0; i < vet.size(); ++ i)
		{
			size += vet[i].element_size();
		}

		if (ST_Geometry == type)
		{
			VertexStreamUnit vs;
			vs.stream = vertex_stream;
			vs.format = vet;
			vs.vertex_size = size;
			vs.type = type;
			vs.freq = freq;
			vertex_streams_.push_back(vs);
		}
		else
		{
			BOOST_ASSERT(!instance_stream_);
			instance_stream_.stream = vertex_stream;
			instance_stream_.format = vet;
			instance_stream_.vertex_size = size;
			instance_stream_.type = type;
			instance_stream_.freq = freq;
		}
	}

	bool RenderLayout::UseIndices() const
	{
		return this->NumIndices() != 0;
	}

	uint32_t RenderLayout::NumIndices() const
	{
		if (index_stream_)
		{
			if (IF_Index16 == index_format_)
			{
				return index_stream_->Size() / sizeof(uint16_t);
			}
			else
			{
				return index_stream_->Size() / sizeof(uint32_t);
			}
		}
		else
		{
			return 0;
		}
	}

	void RenderLayout::SetIndexStream(IndexStreamPtr index_stream, IndexFormat format)
	{
		index_stream_ = index_stream;
		index_format_ = format;
	}

	IndexStreamPtr RenderLayout::GetIndexStream() const
	{
		BOOST_ASSERT(index_stream_);
		return index_stream_;
	}

	VertexStreamPtr RenderLayout::InstanceStream() const
	{
		return instance_stream_.stream;
	}

	uint32_t RenderLayout::NumInstance() const
	{
		return vertex_streams_[0].freq;
	}

	void RenderLayout::ExpandInstance(VertexStreamPtr& hint, uint32_t inst_no) const
	{
		BOOST_ASSERT(instance_stream_);

		uint32_t num_instance = this->NumInstance();
		BOOST_ASSERT(inst_no < num_instance);

		uint32_t num_vertices = this->NumVertices();
		
		if (!hint)
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();
			hint = rf.MakeVertexStream(BU_Dynamic);
		}

		std::vector<uint8_t> instance_buffer(instance_stream_.stream->Size());
		{
			VertexStream::Mapper mapper(*instance_stream_.stream, BA_Read_Only);
			std::copy(mapper.Pointer<uint8_t>(), mapper.Pointer<uint8_t>() + instance_stream_.stream->Size(),
				instance_buffer.begin());
		}
		hint->Resize(instance_stream_.vertex_size * num_vertices);
		VertexStream::Mapper dst_mapper(*hint, BA_Write_Only);

		for (uint32_t i = 0; i < num_vertices; ++ i)
		{
			std::copy(&instance_buffer[0] + inst_no * instance_stream_.vertex_size,
				&instance_buffer[0] + (inst_no + 1) * instance_stream_.vertex_size,
				dst_mapper.Pointer<uint8_t>() + i * instance_stream_.vertex_size);
		}
	}
}
