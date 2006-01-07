// Renderable.cpp
// KlayGE ����Ⱦ������ ʵ���ļ�
// Ver 2.7.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.7.0
// GetWorld����ΪGetModelMatrix (2005.6.17)
//
// 2.3.0
// ������Render (2005.1.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/SceneObject.hpp>

#include <KlayGE/Renderable.hpp>

namespace KlayGE
{
	Renderable::~Renderable()
	{
	}

	void Renderable::OnRenderBegin()
	{
	}

	void Renderable::OnRenderEnd()
	{
	}

	void Renderable::OnInstanceBegin(uint32_t /*id*/)
	{
	}

	void Renderable::OnInstanceEnd(uint32_t /*id*/)
	{
	}

	void Renderable::AddToRenderQueue()
	{
		Context::Instance().SceneManagerInstance().AddRenderable(this->shared_from_this());
	}

	void Renderable::Render()
	{
		this->UpdateInstanceStream();

		RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());

		VertexStreamPtr inst_stream = this->GetVertexBuffer()->InstanceStream();
		if (inst_stream)
		{
			this->OnRenderBegin();
			renderEngine.Render(*this->GetVertexBuffer());
			this->OnRenderEnd();
		}
		else
		{
			this->OnRenderBegin();
			if (instances_.empty())
			{
				renderEngine.Render(*this->GetVertexBuffer());
			}
			else
			{
				for (uint32_t i = 0; i < instances_.size(); ++ i)
				{
					this->OnInstanceBegin(i);
					renderEngine.Render(*this->GetVertexBuffer());
					this->OnInstanceEnd(i);
				}
			}
			this->OnRenderEnd();
		}
	}

	void Renderable::AddInstance(SceneObjectPtr obj)
	{
		instances_.push_back(boost::weak_ptr<SceneObject>(obj));
	}

	void Renderable::UpdateInstanceStream()
	{
		if (!instances_.empty() && !instances_[0].lock()->InstanceFormat().empty())
		{
			VertexStreamPtr inst_stream = this->GetVertexBuffer()->InstanceStream();
			if (!inst_stream)
			{
				RenderFactory& rf(Context::Instance().RenderFactoryInstance());

				inst_stream = rf.MakeVertexStream(BU_Dynamic);
				inst_stream->FrequencyDivider(VertexStream::ST_Instance, 1);
				this->GetVertexBuffer()->AddVertexStream(inst_stream, instances_[0].lock()->InstanceFormat());
			}
			else
			{
				for (size_t i = 0; i < instances_.size(); ++ i)
				{
					BOOST_ASSERT(this->GetVertexBuffer()->InstanceStreamFormat() == instances_[i].lock()->InstanceFormat());
				}
			}

			uint32_t const size = this->GetVertexBuffer()->InstanceSize();

			inst_stream->Resize(size * instances_.size());
			{
				VertexStream::Mapper mapper(*inst_stream, BA_Write_Only);
				for (size_t i = 0; i < instances_.size(); ++ i)
				{
					uint8_t const * src = static_cast<uint8_t const *>(instances_[i].lock()->InstanceData());
					std::copy(src, src + size, mapper.Pointer<uint8_t>() + i * size);
				}
			}

			for (VertexBuffer::VertexStreamIterator iter = this->GetVertexBuffer()->VertexStreamBegin();
				iter != this->GetVertexBuffer()->VertexStreamEnd(); ++ iter)
			{
				(*iter)->FrequencyDivider(VertexStream::ST_Geometry, static_cast<uint32_t>(instances_.size()));
			}
		}
	}
}
