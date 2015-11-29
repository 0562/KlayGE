// RenderFactory.cpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2003-2008
// Homepage: http://www.klayge.org
//
// 3.8.0
// ������MakeSamplerStateObject (2008.9.21)
//
// 2.8.0
// ������LoadEffect (2005.7.25)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KFL/Util.hpp>
#include <KFL/Math.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/Query.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/RenderStateObject.hpp>
#include <KlayGE/ShaderObject.hpp>
#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/Fence.hpp>

#include <KlayGE/RenderFactory.hpp>

namespace KlayGE
{
	class NullRenderFactory : public RenderFactory
	{
		std::wstring const & Name() const
		{
			static std::wstring const name(L"Null Render Factory");
			return name;
		}

		TexturePtr MakeDelayCreationTexture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		{
			KFL_UNUSED(width);
			KFL_UNUSED(num_mip_maps);
			KFL_UNUSED(array_size);
			KFL_UNUSED(format);
			KFL_UNUSED(sample_count);
			KFL_UNUSED(sample_quality);
			KFL_UNUSED(access_hint);
			return Texture::NullObject();
		}
		TexturePtr MakeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		{
			KFL_UNUSED(width);
			KFL_UNUSED(height);
			KFL_UNUSED(num_mip_maps);
			KFL_UNUSED(array_size);
			KFL_UNUSED(format);
			KFL_UNUSED(sample_count);
			KFL_UNUSED(sample_quality);
			KFL_UNUSED(access_hint);
			return Texture::NullObject();
		}
		TexturePtr MakeDelayCreationTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		{
			KFL_UNUSED(width);
			KFL_UNUSED(height);
			KFL_UNUSED(depth);
			KFL_UNUSED(num_mip_maps);
			KFL_UNUSED(array_size);
			KFL_UNUSED(format);
			KFL_UNUSED(sample_count);
			KFL_UNUSED(sample_quality);
			KFL_UNUSED(access_hint);
			return Texture::NullObject();
		}
		TexturePtr MakeDelayCreationTextureCube(uint32_t size, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		{
			KFL_UNUSED(size);
			KFL_UNUSED(num_mip_maps);
			KFL_UNUSED(array_size);
			KFL_UNUSED(format);
			KFL_UNUSED(sample_count);
			KFL_UNUSED(sample_quality);
			KFL_UNUSED(access_hint);
			return Texture::NullObject();
		}
		FrameBufferPtr MakeFrameBuffer()
		{
			return FrameBuffer::NullObject();
		}

		RenderLayoutPtr MakeRenderLayout()
		{
			return RenderLayout::NullObject();
		}

		virtual GraphicsBufferPtr MakeDelayCreationVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_bytes, ElementFormat fmt) KLAYGE_OVERRIDE
		{
			KFL_UNUSED(usage);
			KFL_UNUSED(access_hint);
			KFL_UNUSED(size_in_bytes);
			KFL_UNUSED(fmt);
			return GraphicsBuffer::NullObject();
		}
		virtual GraphicsBufferPtr MakeDelayCreationIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_bytes, ElementFormat fmt) KLAYGE_OVERRIDE
		{
			KFL_UNUSED(usage);
			KFL_UNUSED(access_hint);
			KFL_UNUSED(size_in_bytes);
			KFL_UNUSED(fmt);
			return GraphicsBuffer::NullObject();
		}
		virtual GraphicsBufferPtr MakeDelayCreationConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_bytes, ElementFormat fmt) KLAYGE_OVERRIDE
		{
			KFL_UNUSED(usage);
			KFL_UNUSED(access_hint);
			KFL_UNUSED(size_in_bytes);
			KFL_UNUSED(fmt);
			return GraphicsBuffer::NullObject();
		}

		QueryPtr MakeOcclusionQuery()
		{
			return Query::NullObject();
		}

		QueryPtr MakeConditionalRender()
		{
			return Query::NullObject();
		}

		QueryPtr MakeTimerQuery()
		{
			return Query::NullObject();
		}

		virtual FencePtr MakeFence() KLAYGE_OVERRIDE
		{
			return Fence::NullObject();
		}

		RenderViewPtr Make1DRenderView(Texture& /*texture*/, int /*first_array_index*/, int /*array_size*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DRenderView(Texture& /*texture*/, int /*first_array_index*/, int /*array_size*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DRenderView(Texture& /*texture*/, int /*array_index*/, Texture::CubeFaces /*face*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DRenderView(Texture& /*texture*/, int /*array_index*/, uint32_t /*slice*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr MakeCubeRenderView(Texture& /*texture*/, int /*array_index*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make3DRenderView(Texture& /*texture*/, int /*array_index*/, uint32_t /*first_slice*/, uint32_t /*num_slices*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr MakeGraphicsBufferRenderView(GraphicsBuffer& /*gbuffer*/, uint32_t /*width*/, uint32_t /*height*/, ElementFormat /*pf*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DDepthStencilRenderView(uint32_t /*width*/, uint32_t /*height*/, ElementFormat /*pf*/,
			uint32_t /*sample_count*/, uint32_t /*sample_quality*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make1DDepthStencilRenderView(Texture& /*texture*/, int /*first_array_index*/, int /*array_size*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DDepthStencilRenderView(Texture& /*texture*/, int /*first_array_index*/, int /*array_size*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DDepthStencilRenderView(Texture& /*texture*/, int /*array_index*/, Texture::CubeFaces /*face*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make2DDepthStencilRenderView(Texture& /*texture*/, int /*array_index*/, uint32_t /*slice*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr MakeCubeDepthStencilRenderView(Texture& /*texture*/, int /*array_index*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		RenderViewPtr Make3DDepthStencilRenderView(Texture& /*texture*/, int /*array_index*/, uint32_t /*first_slice*/, uint32_t /*num_slices*/, int /*level*/)
		{
			return RenderView::NullObject();
		}

		UnorderedAccessViewPtr Make1DUnorderedAccessView(Texture& /*texture*/, int /*first_array_index*/, int /*array_size*/, int /*level*/)
		{
			return UnorderedAccessView::NullObject();
		}

		UnorderedAccessViewPtr Make2DUnorderedAccessView(Texture& /*texture*/, int /*first_array_index*/, int /*array_size*/, int /*level*/)
		{
			return UnorderedAccessView::NullObject();
		}

		UnorderedAccessViewPtr Make2DUnorderedAccessView(Texture& /*texture*/, int /*array_index*/, Texture::CubeFaces /*face*/, int /*level*/)
		{
			return UnorderedAccessView::NullObject();
		}

		UnorderedAccessViewPtr Make2DUnorderedAccessView(Texture& /*texture*/, int /*array_index*/, uint32_t /*slice*/, int /*level*/)
		{
			return UnorderedAccessView::NullObject();
		}

		UnorderedAccessViewPtr MakeCubeUnorderedAccessView(Texture& /*texture*/, int /*array_index*/, int /*level*/)
		{
			return UnorderedAccessView::NullObject();
		}

		UnorderedAccessViewPtr Make3DUnorderedAccessView(Texture& /*texture*/, int /*array_index*/, uint32_t /*first_slice*/, uint32_t /*num_slices*/, int /*level*/)
		{
			return UnorderedAccessView::NullObject();
		}

		UnorderedAccessViewPtr MakeGraphicsBufferUnorderedAccessView(GraphicsBuffer& /*gbuffer*/, ElementFormat /*pf*/)
		{
			return UnorderedAccessView::NullObject();
		}

		ShaderObjectPtr MakeShaderObject()
		{
			return ShaderObject::NullObject();
		}

		RenderEnginePtr DoMakeRenderEngine()
		{
			return RenderEngine::NullObject();
		}

		RasterizerStateObjectPtr DoMakeRasterizerStateObject(RasterizerStateDesc const & /*desc*/)
		{
			return RasterizerStateObject::NullObject();
		}

		DepthStencilStateObjectPtr DoMakeDepthStencilStateObject(DepthStencilStateDesc const & /*desc*/)
		{
			return DepthStencilStateObject::NullObject();
		}

		BlendStateObjectPtr DoMakeBlendStateObject(BlendStateDesc const & /*desc*/)
		{
			return BlendStateObject::NullObject();
		}

		SamplerStateObjectPtr DoMakeSamplerStateObject(SamplerStateDesc const & /*desc*/)
		{
			return SamplerStateObject::NullObject();
		}

	private:
		virtual void DoSuspend() KLAYGE_OVERRIDE
		{
		}
		virtual void DoResume() KLAYGE_OVERRIDE
		{
		}
	};


	RenderFactory::~RenderFactory()
	{
		for (auto& rs : rs_pool_)
		{
			rs.second.reset();
		}
		for (auto& dss : dss_pool_)
		{
			dss.second.reset();
		}
		for (auto& bs : bs_pool_)
		{
			bs.second.reset();
		}
		for (auto& ss : ss_pool_)
		{
			ss.second.reset();
		}

		re_.reset();
	}

	RenderFactoryPtr RenderFactory::NullObject()
	{
		static RenderFactoryPtr obj = MakeSharedPtr<NullRenderFactory>();
		return obj;
	}

	RenderEngine& RenderFactory::RenderEngineInstance()
	{
		if (!re_)
		{
			re_ = this->DoMakeRenderEngine();
		}

		return *re_;
	}
	
	void RenderFactory::Suspend()
	{
		if (re_)
		{
			re_->Suspend();
		}
		this->DoSuspend();
	}
	
	void RenderFactory::Resume()
	{
		this->DoResume();
		if (re_)
		{
			re_->Resume();
		}
	}

	TexturePtr RenderFactory::MakeTexture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size,
		ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData const * init_data)
	{
		TexturePtr ret = this->MakeDelayCreationTexture1D(width, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
		ret->CreateHWResource(init_data);
		return ret;
	}

	TexturePtr RenderFactory::MakeTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size,
		ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData const * init_data)
	{
		TexturePtr ret = this->MakeDelayCreationTexture2D(width, height, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
		ret->CreateHWResource(init_data);
		return ret;
	}

	TexturePtr RenderFactory::MakeTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip_maps, uint32_t array_size,
		ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData const * init_data)
	{
		TexturePtr ret = this->MakeDelayCreationTexture3D(width, height, depth, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
		ret->CreateHWResource(init_data);
		return ret;
	}

	TexturePtr RenderFactory::MakeTextureCube(uint32_t size, uint32_t num_mip_maps, uint32_t array_size,
		ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData const * init_data)
	{
		TexturePtr ret = this->MakeDelayCreationTextureCube(size, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
		ret->CreateHWResource(init_data);
		return ret;
	}

	GraphicsBufferPtr RenderFactory::MakeVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->MakeDelayCreationVertexBuffer(usage, access_hint, size_in_byte, fmt);
		ret->CreateHWResource(init_data);
		return ret;
	}

	GraphicsBufferPtr RenderFactory::MakeIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->MakeDelayCreationIndexBuffer(usage, access_hint, size_in_byte, fmt);
		ret->CreateHWResource(init_data);
		return ret;
	}

	GraphicsBufferPtr RenderFactory::MakeConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->MakeDelayCreationConstantBuffer(usage, access_hint, size_in_byte, fmt);
		ret->CreateHWResource(init_data);
		return ret;
	}

	RasterizerStateObjectPtr RenderFactory::MakeRasterizerStateObject(RasterizerStateDesc const & desc)
	{
		RasterizerStateObjectPtr ret;

		auto iter = rs_pool_.find(desc);
		if (iter == rs_pool_.end())
		{
			ret = this->DoMakeRasterizerStateObject(desc);
			KLAYGE_EMPLACE(rs_pool_, desc, ret);
		}
		else
		{
			ret = iter->second;
		}

		return ret;
	}

	DepthStencilStateObjectPtr RenderFactory::MakeDepthStencilStateObject(DepthStencilStateDesc const & desc)
	{
		DepthStencilStateObjectPtr ret;

		auto iter = dss_pool_.find(desc);
		if (iter == dss_pool_.end())
		{
			ret = this->DoMakeDepthStencilStateObject(desc);
			KLAYGE_EMPLACE(dss_pool_, desc, ret);
		}
		else
		{
			ret = iter->second;
		}

		return ret;
	}

	BlendStateObjectPtr RenderFactory::MakeBlendStateObject(BlendStateDesc const & desc)
	{
		BlendStateObjectPtr ret;

		auto iter = bs_pool_.find(desc);
		if (iter == bs_pool_.end())
		{
			ret = this->DoMakeBlendStateObject(desc);
			KLAYGE_EMPLACE(bs_pool_, desc, ret);
		}
		else
		{
			ret = iter->second;
		}

		return ret;
	}

	SamplerStateObjectPtr RenderFactory::MakeSamplerStateObject(SamplerStateDesc const & desc)
	{
		SamplerStateObjectPtr ret;

		auto iter = ss_pool_.find(desc);
		if (iter == ss_pool_.end())
		{
			ret = this->DoMakeSamplerStateObject(desc);
			KLAYGE_EMPLACE(ss_pool_, desc, ret);
		}
		else
		{
			ret = iter->second;
		}

		return ret;
	}
}
