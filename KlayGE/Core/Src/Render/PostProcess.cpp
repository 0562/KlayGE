// PostProcess.cpp
// KlayGE ���ڴ����� ʵ���ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2006-2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ʹ��InputPin��OutputPin��ָ��������� (2010.3.23)
//
// 3.6.0
// ������BlurPostProcess (2007.3.24)
//
// 3.5.0
// ������GammaCorrectionProcess (2007.1.22)
//
// 3.3.0
// ���ν��� (2006.6.23)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/XMLDom.hpp>

#include <cstring>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

#include <KlayGE/PostProcess.hpp>

namespace KlayGE
{
	PostProcess::PostProcess(std::wstring const & name)
			: RenderableHelper(name),
				num_bind_output_(0)
	{
		this->CreateVB();
	}

	PostProcess::PostProcess(std::wstring const & name,
		std::vector<std::string> const & param_names,
		std::vector<std::string> const & input_pin_names,
		std::vector<std::string> const & output_pin_names,
		RenderTechniquePtr const & tech)
			: RenderableHelper(name),
				input_pins_(input_pin_names.size()),
				output_pins_(output_pin_names.size()),
				num_bind_output_(0),
				params_(param_names.size()),
				input_pins_ep_(input_pin_names.size())
	{
		this->CreateVB();

		for (size_t i = 0; i < input_pin_names.size(); ++ i)
		{
			input_pins_[i].first = input_pin_names[i];
		}
		for (size_t i = 0; i < output_pin_names.size(); ++ i)
		{
			output_pins_[i].first = output_pin_names[i];
		}
		for (size_t i = 0; i < param_names.size(); ++ i)
		{
			params_[i].first = param_names[i];
		}
		this->Technique(tech);
	}

	void PostProcess::Technique(RenderTechniquePtr const & tech)
	{
		technique_ = tech;
		this->UpdateBinds();
	}

	void PostProcess::UpdateBinds()
	{
		if (technique_)
		{
			flipping_ep_ = technique_->Effect().ParameterByName("flipping");
			*flipping_ep_ = static_cast<int32_t>(frame_buffer_->RequiresFlipping() ? -1 : +1);

			input_pins_ep_.resize(input_pins_.size());
			for (size_t i = 0; i < input_pins_.size(); ++ i)
			{
				input_pins_ep_[i] = technique_->Effect().ParameterByName(input_pins_[i].first);
			}

			output_pins_ep_.resize(output_pins_.size());
			for (size_t i = 0; i < output_pins_.size(); ++ i)
			{
				output_pins_ep_[i] = technique_->Effect().ParameterByName(output_pins_[i].first);
			}

			for (size_t i = 0; i < params_.size(); ++ i)
			{
				params_[i].second = technique_->Effect().ParameterByName(params_[i].first);
			}
		}
	}

	uint32_t PostProcess::NumParams() const
	{
		return static_cast<uint32_t>(params_.size());
	}

	uint32_t PostProcess::ParamByName(std::string const & name) const
	{
		for (size_t i = 0; i < params_.size(); ++ i)
		{
			if (params_[i].first == name)
			{
				return static_cast<uint32_t>(i);
			}
		}
		return 0xFFFFFFFF;
	}

	std::string const & PostProcess::ParamName(uint32_t index) const
	{
		return params_[index].first;
	}

	void PostProcess::SetParam(uint32_t index, bool const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, uint32_t const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, int32_t const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, float const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, uint2 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, uint3 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, uint4 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, int2 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, int3 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, int4 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, float2 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, float3 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, float4 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, float4x4 const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<bool> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<uint32_t> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<int32_t> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<float> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<uint2> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<uint3> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<uint4> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<int2> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<int3> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<int4> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<float2> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<float3> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<float4> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::SetParam(uint32_t index, std::vector<float4x4> const & value)
	{
		*params_[index].second = value;
	}

	void PostProcess::GetParam(uint32_t index, bool& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, uint32_t& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, int32_t& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, float& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, uint2& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, uint3& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, uint4& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, int2& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, int3& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, int4& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, float2& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, float3& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, float4& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, float4x4& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<bool>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<uint32_t>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<int32_t>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<float>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<uint2>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<uint3>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<uint4>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<int2>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<int3>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<int4>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<float2>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<float3>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<float4>& value)
	{
		params_[index].second->Value(value);
	}

	void PostProcess::GetParam(uint32_t index, std::vector<float4x4>& value)
	{
		params_[index].second->Value(value);
	}

	uint32_t PostProcess::NumInputPins() const
	{
		return static_cast<uint32_t>(input_pins_.size());
	}

	uint32_t PostProcess::InputPinByName(std::string const & name) const
	{
		for (size_t i = 0; i < input_pins_.size(); ++ i)
		{
			if (input_pins_[i].first == name)
			{
				return static_cast<uint32_t>(i);
			}
		}
		return 0xFFFFFFFF;
	}

	std::string const & PostProcess::InputPinName(uint32_t index) const
	{
		return input_pins_[index].first;
	}

	void PostProcess::InputPin(uint32_t index, TexturePtr const & tex)
	{
		input_pins_[index].second = tex;
		*(input_pins_ep_[index]) = tex;
	}

	TexturePtr const & PostProcess::InputPin(uint32_t index) const
	{
		BOOST_ASSERT(index < input_pins_.size());
		return input_pins_[index].second;
	}

	uint32_t PostProcess::NumOutputPins() const
	{
		return static_cast<uint32_t>(output_pins_.size());
	}

	uint32_t PostProcess::OutputPinByName(std::string const & name) const
	{
		for (size_t i = 0; i < output_pins_.size(); ++ i)
		{
			if (output_pins_[i].first == name)
			{
				return static_cast<uint32_t>(i);
			}
		}
		return 0xFFFFFFFF;
	}

	std::string const & PostProcess::OutputPinName(uint32_t index) const
	{
		return output_pins_[index].first;
	}

	void PostProcess::OutputPin(uint32_t index, TexturePtr const & tex, int level, int array_index, int face)
	{
		if (!output_pins_[index].second && tex)
		{
			++ num_bind_output_;
		}
		if (output_pins_[index].second && !tex)
		{
			-- num_bind_output_;
		}

		output_pins_[index].second = tex;
		if (tex)
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();
			RenderViewPtr view;
			if (Texture::TT_2D == tex->Type())
			{
				view = rf.Make2DRenderView(*tex, array_index, 1, level);
			}
			else
			{
				BOOST_ASSERT(Texture::TT_Cube == tex->Type());
				view = rf.Make2DRenderView(*tex, array_index, static_cast<Texture::CubeFaces>(face), level);
			}
			frame_buffer_->Attach(FrameBuffer::ATT_Color0 + index, view);

			if (output_pins_ep_[index])
			{
				*(output_pins_ep_[index]) = tex;
			}
		}
	}

	TexturePtr const & PostProcess::OutputPin(uint32_t index) const
	{
		BOOST_ASSERT(index < output_pins_.size());
		return output_pins_[index].second;
	}

	void PostProcess::Apply()
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		FrameBufferPtr const & fb = (0 == num_bind_output_) ? re.DefaultFrameBuffer() : frame_buffer_;
		re.BindFrameBuffer(fb);
		this->Render();
	}

	void PostProcess::OnRenderBegin()
	{
	}

	void PostProcess::CreateVB()
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();

		rl_ = rf.MakeRenderLayout();
		rl_->TopologyType(RenderLayout::TT_TriangleStrip);

		float2 pos[] =
		{
			float2(-1, +1),
			float2(+1, +1),
			float2(-1, -1),
			float2(+1, -1)
		};
		box_ = Box(float3(-1, -1, -1), float3(1, 1, 1));
		ElementInitData init_data;
		init_data.row_pitch = sizeof(pos);
		init_data.data = &pos[0];
		pos_vb_ = rf.MakeVertexBuffer(BU_Static, EAH_GPU_Read, &init_data);
		rl_->BindVertexStream(pos_vb_, boost::make_tuple(vertex_element(VEU_Position, 0, EF_GR32F)));

		frame_buffer_ = rf.MakeFrameBuffer();
		frame_buffer_->GetViewport().camera = rf.RenderEngineInstance().CurFrameBuffer()->GetViewport().camera;
	}


	PostProcessPtr LoadPostProcess(ResIdentifierPtr const & ppml, std::string const & pp_name)
	{
		XMLDocument doc;
		XMLNodePtr root = doc.Parse(ppml);

		std::wstring wname;
		std::vector<std::string> param_names;
		std::vector<std::string> input_pin_names;
		std::vector<std::string> output_pin_names;
		RenderTechniquePtr tech;

		for (XMLNodePtr pp_node = root->FirstNode("post_processor"); pp_node; pp_node = pp_node->NextSibling("post_processor"))
		{
			std::string name = pp_node->Attrib("name")->ValueString();
			if (pp_name == name)
			{
				Convert(wname, name);

				XMLNodePtr params_chunk = pp_node->FirstNode("params");
				if (params_chunk)
				{
					for (XMLNodePtr p_node = params_chunk->FirstNode("param"); p_node; p_node = p_node->NextSibling("param"))
					{
						param_names.push_back(p_node->Attrib("name")->ValueString());
					}
				}
				XMLNodePtr input_chunk = pp_node->FirstNode("input");
				if (input_chunk)
				{
					for (XMLNodePtr pin_node = input_chunk->FirstNode("pin"); pin_node; pin_node = pin_node->NextSibling("pin"))
					{
						input_pin_names.push_back(pin_node->Attrib("name")->ValueString());
					}
				}
				XMLNodePtr output_chunk = pp_node->FirstNode("output");
				if (output_chunk)
				{
					for (XMLNodePtr pin_node = output_chunk->FirstNode("pin"); pin_node; pin_node = pin_node->NextSibling("pin"))
					{
						output_pin_names.push_back(pin_node->Attrib("name")->ValueString());
					}
				}
				XMLNodePtr shader_chunk = pp_node->FirstNode("shader");
				if (shader_chunk)
				{
					std::string effect_name = shader_chunk->Attrib("effect")->ValueString();
					std::string tech_name = shader_chunk->Attrib("tech")->ValueString();
					tech = Context::Instance().RenderFactoryInstance().LoadEffect(effect_name)->TechniqueByName(tech_name);
				}
			}
		}

		return MakeSharedPtr<PostProcess>(wname, param_names, input_pin_names, output_pin_names, tech);
	}


	PostProcessChain::PostProcessChain(std::wstring const & name)
			: PostProcess(name)
	{
	}

	PostProcessChain::PostProcessChain(std::wstring const & name,
		std::vector<std::string> const & param_names,
		std::vector<std::string> const & input_pin_names,
		std::vector<std::string> const & output_pin_names,
		RenderTechniquePtr const & tech)
			: PostProcess(name, param_names, input_pin_names, output_pin_names, tech)
	{
	}

	void PostProcessChain::Append(PostProcessPtr const & pp)
	{
		pp_chain_.push_back(pp);
	}

	uint32_t PostProcessChain::NumParams() const
	{
		return pp_chain_.front()->NumParams();
	}

	uint32_t PostProcessChain::ParamByName(std::string const & name) const
	{
		return pp_chain_.front()->ParamByName(name);
	}

	std::string const & PostProcessChain::ParamName(uint32_t index) const
	{
		return pp_chain_.front()->ParamName(index);
	}

	void PostProcessChain::SetParam(uint32_t index, bool const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, uint32_t const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, int32_t const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, float const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, uint2 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, uint3 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, uint4 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, int2 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, int3 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, int4 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, float2 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, float3 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, float4 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, float4x4 const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<bool> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<uint32_t> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<int32_t> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<float> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<uint2> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<uint3> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<uint4> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<int2> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<int3> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<int4> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<float2> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<float3> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<float4> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::SetParam(uint32_t index, std::vector<float4x4> const & value)
	{
		pp_chain_.front()->SetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, bool& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, uint32_t& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, int32_t& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, float& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, uint2& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, uint3& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, uint4& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, int2& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, int3& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, int4& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, float2& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, float3& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, float4& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, float4x4& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<bool>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<uint32_t>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<int32_t>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<float>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<uint2>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<uint3>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<uint4>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<int2>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<int3>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<int4>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<float2>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<float3>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<float4>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	void PostProcessChain::GetParam(uint32_t index, std::vector<float4x4>& value)
	{
		pp_chain_.front()->GetParam(index, value);
	}

	uint32_t PostProcessChain::NumInputPins() const
	{
		return pp_chain_.front()->NumInputPins();
	}

	uint32_t PostProcessChain::InputPinByName(std::string const & name) const
	{
		return pp_chain_.front()->InputPinByName(name);
	}

	std::string const & PostProcessChain::InputPinName(uint32_t index) const
	{
		return pp_chain_.front()->InputPinName(index);
	}

	void PostProcessChain::InputPin(uint32_t index, TexturePtr const & tex)
	{
		pp_chain_.front()->InputPin(index, tex);
	}

	TexturePtr const & PostProcessChain::InputPin(uint32_t index) const
	{
		return pp_chain_.front()->InputPin(index);
	}

	uint32_t PostProcessChain::NumOutputPins() const
	{
		return pp_chain_.back()->NumOutputPins();
	}

	uint32_t PostProcessChain::OutputPinByName(std::string const & name) const
	{
		return pp_chain_.back()->OutputPinByName(name);
	}

	std::string const & PostProcessChain::OutputPinName(uint32_t index) const
	{
		return pp_chain_.back()->OutputPinName(index);
	}

	void PostProcessChain::OutputPin(uint32_t index, TexturePtr const & tex, int level, int array_index, int face)
	{
		pp_chain_.back()->OutputPin(index, tex, level, array_index, face);
	}

	TexturePtr const & PostProcessChain::OutputPin(uint32_t index) const
	{
		return pp_chain_.back()->OutputPin(index);
	}

	void PostProcessChain::Apply()
	{
		BOOST_FOREACH(BOOST_TYPEOF(pp_chain_)::reference pp, pp_chain_)
		{
			pp->Apply();
		}
	}


	SeparableBoxFilterPostProcess::SeparableBoxFilterPostProcess(RenderTechniquePtr const & tech, int kernel_radius, float multiplier, bool x_dir)
		: PostProcess(L"SeparableBoxFilter",
				std::vector<std::string>(),
				std::vector<std::string>(1, "src_tex"),
				std::vector<std::string>(1, "output"),
				tech ? tech : Context::Instance().RenderFactoryInstance().LoadEffect("Blur.fxml")->TechniqueByName(x_dir ? "BlurX" : "BlurY")),
			kernel_radius_(kernel_radius), multiplier_(multiplier), x_dir_(x_dir)
	{
		BOOST_ASSERT((kernel_radius > 0) && (kernel_radius <= 8));

		src_tex_size_ep_ = technique_->Effect().ParameterByName("src_tex_size");
		color_weight_ep_ = technique_->Effect().ParameterByName("color_weight");
		tex_coord_offset_ep_ = technique_->Effect().ParameterByName("tex_coord_offset");
	}

	SeparableBoxFilterPostProcess::~SeparableBoxFilterPostProcess()
	{
	}
	
	void SeparableBoxFilterPostProcess::InputPin(uint32_t index, TexturePtr const & tex)
	{
		PostProcess::InputPin(index, tex);
		if (0 == index)
		{
			this->CalSampleOffsets(x_dir_ ? tex->Width(0) : tex->Height(0));
		}
	}

	void SeparableBoxFilterPostProcess::CalSampleOffsets(uint32_t tex_size)
	{
		std::vector<float> color_weight(kernel_radius_ + 1, multiplier_ / (2 * kernel_radius_ + 1));
		std::vector<float> tex_coord_offset(kernel_radius_ + 1, 0);

		float const tu = 1.0f / tex_size;

		for (int i = 0; i < kernel_radius_; ++ i)
		{
			color_weight[i] *= 2;
			tex_coord_offset[i] = (i * 2 - kernel_radius_ + 0.5f) * tu;
		}
		tex_coord_offset[kernel_radius_] = kernel_radius_ * tu;

		*src_tex_size_ep_ = float2(static_cast<float>(tex_size), 1.0f / tex_size);
		*color_weight_ep_ = color_weight;
		*tex_coord_offset_ep_ = tex_coord_offset;
	}


	SeparableGaussianFilterPostProcess::SeparableGaussianFilterPostProcess(RenderTechniquePtr const & tech, int kernel_radius, float multiplier, bool x_dir)
			: PostProcess(L"SeparableGaussian",
					std::vector<std::string>(),
					std::vector<std::string>(1, "src_tex"),
					std::vector<std::string>(1, "output"),
					tech ? tech : Context::Instance().RenderFactoryInstance().LoadEffect("Blur.fxml")->TechniqueByName(x_dir ? "BlurX" : "BlurY")),
				kernel_radius_(kernel_radius), multiplier_(multiplier), x_dir_(x_dir)
	{
		BOOST_ASSERT((kernel_radius > 0) && (kernel_radius <= 8));

		src_tex_size_ep_ = technique_->Effect().ParameterByName("src_tex_size");
		color_weight_ep_ = technique_->Effect().ParameterByName("color_weight");
		tex_coord_offset_ep_ = technique_->Effect().ParameterByName("tex_coord_offset");
	}

	SeparableGaussianFilterPostProcess::~SeparableGaussianFilterPostProcess()
	{
	}

	void SeparableGaussianFilterPostProcess::InputPin(uint32_t index, TexturePtr const & tex)
	{
		PostProcess::InputPin(index, tex);
		if (0 == index)
		{
			this->CalSampleOffsets(x_dir_ ? tex->Width(0) : tex->Height(0), 3.0f);
		}
	}

	float SeparableGaussianFilterPostProcess::GaussianDistribution(float x, float y, float rho)
	{
		float g = 1.0f / sqrt(2.0f * PI * rho * rho);
		g *= exp(-(x * x + y * y) / (2 * rho * rho));
		return g;
	}

	void SeparableGaussianFilterPostProcess::CalSampleOffsets(uint32_t tex_size, float deviation)
	{
		std::vector<float> color_weight(kernel_radius_, 0);
		std::vector<float> tex_coord_offset(kernel_radius_, 0);

		std::vector<float> tmp_weights(kernel_radius_ * 2, 0);
		std::vector<float> tmp_offset(kernel_radius_ * 2, 0);

		float const tu = 1.0f / tex_size;

		float sum_weight = 0;
		for (int i = 0; i < 2 * kernel_radius_; ++ i)
		{
			float weight = this->GaussianDistribution(static_cast<float>(i - kernel_radius_), 0, kernel_radius_ / deviation);
			tmp_weights[i] = weight;
			sum_weight += weight;
		}
		for (int i = 0; i < 2 * kernel_radius_; ++ i)
		{
			tmp_weights[i] /= sum_weight;
		}

		// Fill the offsets
		for (int i = 0; i < kernel_radius_; ++ i)
		{
			tmp_offset[i]                  = static_cast<float>(i - kernel_radius_);
			tmp_offset[i + kernel_radius_] = static_cast<float>(i);
		}

		// Bilinear filtering taps
		// Ordering is left to right.
		for (int i = 0; i < kernel_radius_; ++ i)
		{
			float const scale = tmp_weights[i * 2] + tmp_weights[i * 2 + 1];
			float const frac = tmp_weights[i * 2] / scale;

			tex_coord_offset[i] = (tmp_offset[i * 2] + (1 - frac)) * tu;
			color_weight[i] = multiplier_ * scale;
		}

		*src_tex_size_ep_ = float2(static_cast<float>(tex_size), 1.0f / tex_size);
		*color_weight_ep_ = color_weight;
		*tex_coord_offset_ep_ = tex_coord_offset;
	}


	SeparableBilateralFilterPostProcess::SeparableBilateralFilterPostProcess(RenderTechniquePtr const & tech, int kernel_radius, float multiplier, bool x_dir)
		: PostProcess(L"SeparableBilateral",
				std::vector<std::string>(),
				std::vector<std::string>(1, "src_tex"),
				std::vector<std::string>(1, "out_tex"),
				tech ? tech : Context::Instance().RenderFactoryInstance().LoadEffect("BilateralBlur.fxml")->TechniqueByName(x_dir ? "BlurX" : "BlurY")),
			kernel_radius_(kernel_radius), multiplier_(multiplier), x_dir_(x_dir)
	{
		kernel_radius_ep_ = technique_->Effect().ParameterByName("kernel_radius");
		src_tex_size_ep_ = technique_->Effect().ParameterByName("src_tex_size");
		init_g_ep_ = technique_->Effect().ParameterByName("init_g");
		blur_factor_ep_ = technique_->Effect().ParameterByName("blur_factor");
		sharpness_factor_ep_ = technique_->Effect().ParameterByName("sharpness_factor");
	}

	SeparableBilateralFilterPostProcess::~SeparableBilateralFilterPostProcess()
	{
	}

	void SeparableBilateralFilterPostProcess::InputPin(uint32_t index, TexturePtr const & tex)
	{
		PostProcess::InputPin(index, tex);
		if (0 == index)
		{
			*kernel_radius_ep_ = static_cast<int32_t>(kernel_radius_);
			float tex_size = static_cast<float>(x_dir_ ? tex->Width(0) : tex->Height(0));
			*src_tex_size_ep_ = float2(tex_size, 1.0f / tex_size);
			
			float rho = kernel_radius_ / 4.0f;
			*init_g_ep_ = multiplier_ / std::sqrt(2.0f * PI * rho * rho);
			float f = 1 / (2 * rho * rho);
			*blur_factor_ep_ = f;
			*sharpness_factor_ep_ = f;
		}
	}
}
