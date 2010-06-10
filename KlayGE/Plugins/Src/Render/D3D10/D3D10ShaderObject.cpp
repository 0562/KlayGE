// D3D10ShaderObject.cpp
// KlayGE D3D10 shader������ ʵ���ļ�
// Ver 3.9.0
// ��Ȩ����(C) ������, 2008-2009
// Homepage: http://www.klayge.org
//
// 3.9.0
// ����Shader���� (2009.7.31)
//
// 3.8.0
// ֧��Gemoetry Shader (2009.2.5)
// ���ν��� (2008.9.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/COMPtr.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderEffect.hpp>

#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <boost/assert.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#ifndef BOOST_MEM_FN_ENABLE_STDCALL
#define BOOST_MEM_FN_ENABLE_STDCALL
#endif
#include <boost/mem_fn.hpp>

#include <KlayGE/D3D10/D3D10MinGWDefs.hpp>
#include <d3d10.h>
#include <d3dx10.h>

#include <KlayGE/D3D10/D3D10RenderEngine.hpp>
#include <KlayGE/D3D10/D3D10RenderStateObject.hpp>
#include <KlayGE/D3D10/D3D10Mapping.hpp>
#include <KlayGE/D3D10/D3D10Texture.hpp>
#include <KlayGE/D3D10/D3D10ShaderObject.hpp>

namespace
{
	using namespace KlayGE;

	template <typename SrcType, typename DstType>
	class SetD3D10ShaderParameter
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<DstType*>(target)), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			SrcType v;
			param_->Value(v);

			if (*target_ != static_cast<DstType>(v))
			{
				*target_ = static_cast<DstType>(v);
				*dirty_ = true;
			}
		}

	private:
		DstType* target_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <typename T, int N>
	class SetD3D10ShaderParameter<Vector_T<T, N>, T>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<Vector_T<T, N>*>(target)), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			Vector_T<T, N> v;
			param_->Value(v);

			if (*target_ != v)
			{
				*target_ = v;
				*dirty_ = true;
			}
		}

	private:
		Vector_T<T, N>* target_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <>
	class SetD3D10ShaderParameter<float4x4, float>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, uint32_t rows, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<float4*>(target)), size_(rows * sizeof(float4)), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			float4x4 v;
			param_->Value(v);

			v = MathLib::transpose(v);
			if (memcmp(target_, &v[0], size_))
			{
				memcpy(target_, &v[0], size_);
				*dirty_ = true;
			}
		}

	private:
		float4* target_;
		size_t size_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <typename SrcType, typename DstType>
	class SetD3D10ShaderParameter<SrcType*, DstType>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, uint32_t elements, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<DstType*>(target)), elements_(elements), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			std::vector<SrcType> v;
			param_->Value(v);

			for (size_t i = 0; i < v.size(); ++ i)
			{
				if (target_[i * 4] != static_cast<DstType>(v[i]))
				{
					target_[i * 4] = static_cast<DstType>(v[i]);
					*dirty_ = true;
				}
			}
		}

	private:
		DstType* target_;
		uint32_t elements_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <typename T>
	class SetD3D10ShaderParameter<Vector_T<T, 2>*, T>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, uint32_t elements, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<Vector_T<T, 4>*>(target)), size_(elements * sizeof(int4)), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			std::vector<Vector_T<T, 2> > v;
			param_->Value(v);

			if (!v.empty())
			{
				std::vector<Vector_T<T, 4> > v4(v.size());
				for (size_t i = 0; i < v.size(); ++ i)
				{
					v4[i] = Vector_T<T, 4>(v[i].x(), v[i].y(), 0, 0);
				}
				memcpy(target_, &v4[0], std::min(size_, v4.size() * sizeof(v4[0])));
			}
			*dirty_ = true;
		}

	private:
		Vector_T<T, 4>* target_;
		size_t size_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <typename T>
	class SetD3D10ShaderParameter<Vector_T<T, 3>*, T>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, uint32_t elements, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<Vector_T<T, 4>*>(target)), size_(elements * sizeof(int4)), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			std::vector<Vector_T<T, 3> > v;
			param_->Value(v);

			if (!v.empty())
			{
				std::vector<Vector_T<T, 4> > v4(v.size());
				for (size_t i = 0; i < v.size(); ++ i)
				{
					v4[i] = Vector_T<T, 4>(v[i].x(), v[i].y(), v[i].z(), 0);
				}
				memcpy(target_, &v4[0], std::min(size_, v4.size() * sizeof(v4[0])));
			}
			*dirty_ = true;
		}

	private:
		Vector_T<T, 4>* target_;
		size_t size_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <typename T>
	class SetD3D10ShaderParameter<Vector_T<T, 4>*, T>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, uint32_t elements, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<Vector_T<T, 4>*>(target)), size_(elements * sizeof(int4)), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			std::vector<Vector_T<T, 4> > v;
			param_->Value(v);

			if (!v.empty())
			{
				memcpy(target_, &v[0], std::min(size_, v.size() * sizeof(v[0])));
			}
			*dirty_ = true;
		}

	private:
		Vector_T<T, 4>* target_;
		size_t size_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <>
	class SetD3D10ShaderParameter<float4x4*, float>
	{
	public:
		SetD3D10ShaderParameter(uint8_t* target, size_t rows, RenderEffectParameterPtr const & param, char* dirty)
			: target_(reinterpret_cast<float4*>(target)), rows_(rows), param_(param), dirty_(dirty)
		{
		}

		void operator()()
		{
			std::vector<float4x4> v;
			param_->Value(v);


			size_t start = 0;
			BOOST_FOREACH(BOOST_TYPEOF(v)::reference mat, v)
			{
				mat = MathLib::transpose(mat);
				memcpy(&target_[start], &mat[0], rows_ * sizeof(float4));
				start += rows_;
			}
			*dirty_ = true;
		}

	private:
		float4* target_;
		size_t rows_;
		RenderEffectParameterPtr param_;
		char* dirty_;
	};

	template <>
	class SetD3D10ShaderParameter<SamplerStateObjectPtr, ID3D10SamplerState*>
	{
	public:
		SetD3D10ShaderParameter(ID3D10SamplerState*& sampler, RenderEffectParameterPtr const & param)
			: sampler_(&sampler), param_(param)
		{
		}

		void operator()()
		{
			SamplerStateObjectPtr sampler;
			param_->Value(sampler);
			if (sampler)
			{
				*sampler_ = checked_pointer_cast<D3D10SamplerStateObject>(sampler)->D3DSamplerState().get();
			}
		}

	private:
		ID3D10SamplerState** sampler_;
		RenderEffectParameterPtr param_;
	};

	template <>
	class SetD3D10ShaderParameter<TexturePtr, ID3D10ShaderResourceView*>
	{
	public:
		SetD3D10ShaderParameter(ID3D10ShaderResourceView*& texture, RenderEffectParameterPtr const & param)
			: texture_(&texture), param_(param)
		{
		}

		void operator()()
		{
			TexturePtr tex;
			param_->Value(tex);
			if (tex)
			{
				*texture_ = checked_pointer_cast<D3D10Texture>(tex)->D3DShaderResourceView().get();
			}
		}

	private:
		ID3D10ShaderResourceView** texture_;
		RenderEffectParameterPtr param_;
	};

	template <>
	class SetD3D10ShaderParameter<GraphicsBufferPtr, ID3D10ShaderResourceView*>
	{
	public:
		SetD3D10ShaderParameter(ID3D10ShaderResourceView*& buffer, RenderEffectParameterPtr const & param)
			: buffer_(&buffer), param_(param)
		{
		}

		void operator()()
		{
			GraphicsBufferPtr buf;
			param_->Value(buf);
			if (buf)
			{
				*buffer_ = checked_pointer_cast<D3D10GraphicsBuffer>(buf)->D3DShaderResourceView().get();
			}
		}

	private:
		ID3D10ShaderResourceView** buffer_;
		RenderEffectParameterPtr param_;
	};

	boost::function<void(ID3D10Device*, UINT, UINT, ID3D10ShaderResourceView * const *)> SetShaderResources_[ShaderObject::ST_NumShaderTypes] =
	{
		boost::mem_fn(&ID3D10Device::VSSetShaderResources),
		boost::mem_fn(&ID3D10Device::PSSetShaderResources),
		boost::mem_fn(&ID3D10Device::GSSetShaderResources),
		NULL,
		NULL,
		NULL
	};

	boost::function<void(ID3D10Device*, UINT, UINT, ID3D10SamplerState * const *)> SetSamplers_[ShaderObject::ST_NumShaderTypes] =
	{
		boost::mem_fn(&ID3D10Device::VSSetSamplers),
		boost::mem_fn(&ID3D10Device::PSSetSamplers),
		boost::mem_fn(&ID3D10Device::GSSetSamplers),
		NULL,
		NULL,
		NULL
	};

	boost::function<void(ID3D10Device*, UINT, UINT, ID3D10Buffer * const *)> SetConstantBuffers_[ShaderObject::ST_NumShaderTypes] =
	{
		boost::mem_fn(&ID3D10Device::VSSetConstantBuffers),
		boost::mem_fn(&ID3D10Device::PSSetConstantBuffers),
		boost::mem_fn(&ID3D10Device::GSSetConstantBuffers),
		NULL,
		NULL,
		NULL
	};
}

namespace KlayGE
{
	D3D10ShaderObject::D3D10ShaderObject()
	{
		is_shader_validate_.assign(true);
	}

	std::string D3D10ShaderObject::GenShaderText(RenderEffect const & effect, ShaderType cur_type) const
	{
		std::stringstream ss;

		for (uint32_t i = 0; i < effect.NumMacros(); ++ i)
		{
			std::pair<std::string, std::string> const & name_value = effect.MacroByIndex(i);
			ss << "#define " << name_value.first << " " << name_value.second << std::endl;
		}
		ss << std::endl;

		BOOST_AUTO(cbuffers, effect.CBuffers());
		BOOST_FOREACH(BOOST_TYPEOF(cbuffers)::const_reference cbuff, cbuffers)
		{
			ss << "cbuffer " << cbuff.first << std::endl;
			ss << "{" << std::endl;

			BOOST_FOREACH(BOOST_TYPEOF(cbuff.second)::const_reference param_index, cbuff.second)
			{
				RenderEffectParameter& param = *effect.ParameterByIndex(param_index);
				switch (param.type())
				{
				case REDT_texture1D:
				case REDT_texture2D:
				case REDT_texture3D:
				case REDT_textureCUBE:
				case REDT_texture1DArray:
				case REDT_texture2DArray:
				case REDT_texture3DArray:
				case REDT_textureCUBEArray:
				case REDT_sampler:
				case REDT_buffer:
				case REDT_structured_buffer:
				case REDT_byte_address_buffer:
				case REDT_rw_buffer:
				case REDT_rw_structured_buffer:
				case REDT_rw_texture1D:
				case REDT_rw_texture2D:
				case REDT_rw_texture3D:
				case REDT_rw_texture1DArray:
				case REDT_rw_texture2DArray:
				case REDT_rw_byte_address_buffer:
				case REDT_append_structured_buffer:
				case REDT_consume_structured_buffer:
					break;

				default:
					ss << effect.TypeName(param.type()) << " " << *param.Name();
					if (param.ArraySize())
					{
						ss << "[" << *param.ArraySize() << "]";
					}
					ss << ";" << std::endl;
					break;
				}
			}

			ss << "};" << std::endl;
		}

		for (uint32_t i = 0; i < effect.NumParameters(); ++ i)
		{
			RenderEffectParameter& param = *effect.ParameterByIndex(i);

			switch (param.type())
			{
			case REDT_texture1D:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "Texture1D<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_texture2D:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "Texture2D<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_texture3D:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "Texture3D<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_textureCUBE:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "TextureCube<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_texture1DArray:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "Texture1DArray<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_texture2DArray:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "Texture2DArray<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_buffer:
				{
					std::string elem_type;
					param.var()->Value(elem_type);
					ss << "Buffer<" << elem_type << "> " << *param.Name() << ";" << std::endl;
				}
				break;

			case REDT_sampler:
				ss << "sampler " << *param.Name() << ";" << std::endl;
				break;

			default:
				break;
			}
		}

		RenderDeviceCaps const & caps = Context::Instance().RenderFactoryInstance().RenderEngineInstance().DeviceCaps();
		for (uint32_t i = 0; i < effect.NumShaders(); ++ i)
		{
			RenderShaderFunc const & effect_shader = effect.ShaderByIndex(i);
			ShaderType shader_type = effect_shader.Type();
			if ((ST_NumShaderTypes == shader_type) || (cur_type == shader_type))
			{
				if (caps.max_shader_model >= effect_shader.Version())
				{
					ss << effect_shader.str() << std::endl;
				}
			}
		}

		return ss.str();
	}

	void D3D10ShaderObject::SetShader(RenderEffect& effect, boost::shared_ptr<std::vector<uint32_t> > const & shader_desc_ids,
		uint32_t tech_index, uint32_t pass_index)
	{
		D3D10RenderEngine const & render_eng = *checked_cast<D3D10RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D10DevicePtr const & d3d_device = render_eng.D3DDevice();

		is_validate_ = true;
		for (size_t type = 0; type < ShaderObject::ST_NumShaderTypes; ++ type)
		{
			shader_desc& sd = effect.GetShaderDesc((*shader_desc_ids)[type]);
			if (sd.tech_pass != 0xFFFFFFFF)
			{
				D3D10ShaderObjectPtr so = checked_pointer_cast<D3D10ShaderObject>(effect.TechniqueByIndex(sd.tech_pass >> 16)->Pass(sd.tech_pass & 0xFFFF)->GetShaderObject());

				is_shader_validate_[type] = so->is_shader_validate_[type];
				switch (type)
				{
				case ST_VertexShader:
					vertex_shader_ = so->vertex_shader_;
					vs_code_ = so->vs_code_;
					geometry_shader_ = so->geometry_shader_;
					break;

				case ST_PixelShader:
					pixel_shader_ = so->pixel_shader_;
					break;

				case ST_GeometryShader:
					geometry_shader_ = so->geometry_shader_;
					break;

				default:
					is_shader_validate_[type] = false;
					break;
				}

				samplers_[type].resize(so->samplers_[type].size(), NULL);
				srvs_[type].resize(so->srvs_[type].size(), NULL);

				cbufs_[type] = so->cbufs_[type];
				dirty_[type] = so->dirty_[type];

				d3d_cbufs_[type].resize(so->d3d_cbufs_[type].size());
				D3D10_BUFFER_DESC desc;
				desc.Usage = D3D10_USAGE_DYNAMIC;
				desc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
				desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
				desc.MiscFlags = 0;
				for (size_t j = 0; j < so->d3d_cbufs_[type].size(); ++ j)
				{
					desc.ByteWidth = static_cast<UINT>(so->cbufs_[type][j].size());
					ID3D10Buffer* tmp_buf;
					TIF(d3d_device->CreateBuffer(&desc, NULL, &tmp_buf));
					d3d_cbufs_[type][j] = MakeCOMPtr(tmp_buf);
				}

				param_binds_[type].reserve(so->param_binds_[type].size());
				BOOST_FOREACH(BOOST_TYPEOF(so->param_binds_[type])::const_reference pb, so->param_binds_[type])
				{
					param_binds_[type].push_back(this->GetBindFunc(pb.p_handle, effect.ParameterByName(*(pb.param->Name()))));
				}
			}
			else
			{
				if (!sd.profile.empty())
				{
					std::string shader_text = this->GenShaderText(effect, static_cast<ShaderType>(type));

					is_shader_validate_[type] = true;

					std::string shader_profile = sd.profile;
					switch (type)
					{
					case ST_VertexShader:
						if ("auto" == shader_profile)
						{
							shader_profile = render_eng.D3D10GetVertexShaderProfile(d3d_device.get());
						}
						break;

					case ST_PixelShader:
						if ("auto" == shader_profile)
						{
							shader_profile = render_eng.D3D10GetPixelShaderProfile(d3d_device.get());
						}
						break;

					case ST_GeometryShader:
						if ("auto" == shader_profile)
						{
							shader_profile = render_eng.D3D10GetGeometryShaderProfile(d3d_device.get());
						}
						break;

					default:
						is_shader_validate_[type] = false;
						break;
					}

					ID3D10Blob* code = NULL;
					if (is_shader_validate_[type])
					{
						ID3D10Blob* err_msg;
						D3D10_SHADER_MACRO macros[] = { { "CONSTANT_BUFFER", "1" }, { "KLAYGE_D3D10", "1" }, { NULL, NULL } };
						D3DX10CompileFromMemory(shader_text.c_str(), static_cast<UINT>(shader_text.size()), NULL, macros,
							NULL, sd.func_name.c_str(), shader_profile.c_str(),
							0, 0, NULL, &code, &err_msg, NULL);
						if (err_msg != NULL)
						{
#ifdef KLAYGE_DEBUG
							std::istringstream iss(shader_text);
							std::string s;
							int line = 1;
							while (iss)
							{
								std::getline(iss, s);
								std::cerr << line << " " << s << std::endl;
								++ line;
							}
							std::cerr << static_cast<char*>(err_msg->GetBufferPointer()) << std::endl;
#endif

							err_msg->Release();
						}
					}

					ID3D10BlobPtr code_blob;
					if (NULL == code)
					{
						is_shader_validate_[type] = false;
					}
					else
					{
						code_blob = MakeCOMPtr(code);
						switch (type)
						{
						case ST_VertexShader:
							{
								ID3D10VertexShader* vs;
								if (FAILED(d3d_device->CreateVertexShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), &vs)))
								{
									is_shader_validate_[type] = false;
								}
								else
								{
									vertex_shader_ = MakeCOMPtr(vs);
								}
							}
							vs_code_ = code_blob;

							if (!sd.so_decl.empty())
							{
								std::vector<D3D10_SO_DECLARATION_ENTRY> d3d10_decl(sd.so_decl.size());
								for (size_t i = 0; i < sd.so_decl.size(); ++ i)
								{
									d3d10_decl[i] = D3D10Mapping::Mapping(sd.so_decl[i], static_cast<uint8_t>(i));
								}

								ID3D10GeometryShader* gs;
								if (FAILED(d3d_device->CreateGeometryShaderWithStreamOutput(code_blob->GetBufferPointer(), code_blob->GetBufferSize(),
									&d3d10_decl[0], static_cast<UINT>(d3d10_decl.size()), 0, &gs)))
								{
									is_shader_validate_[type] = false;
								}
								else
								{
									geometry_shader_ = MakeCOMPtr(gs);
								}
							}
							break;

						case ST_PixelShader:
							{
								ID3D10PixelShader* ps;
								if (FAILED(d3d_device->CreatePixelShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), &ps)))
								{
									is_shader_validate_[type] = false;
								}
								else
								{
									pixel_shader_ = MakeCOMPtr(ps);
								}
							}
							break;

						case ST_GeometryShader:
							if (!sd.so_decl.empty())
							{
								std::vector<D3D10_SO_DECLARATION_ENTRY> d3d10_decl(sd.so_decl.size());
								for (size_t i = 0; i < sd.so_decl.size(); ++ i)
								{
									d3d10_decl[i] = D3D10Mapping::Mapping(sd.so_decl[i], static_cast<uint8_t>(i));
								}

								ID3D10GeometryShader* gs;
								if (FAILED(d3d_device->CreateGeometryShaderWithStreamOutput(vs_code_->GetBufferPointer(), code_blob->GetBufferSize(),
									&d3d10_decl[0], static_cast<UINT>(d3d10_decl.size()), 0, &gs)))
								{
									is_shader_validate_[type] = false;
								}
								else
								{
									geometry_shader_ = MakeCOMPtr(gs);
								}
							}
							else
							{
								ID3D10GeometryShader* gs;
								if (FAILED(d3d_device->CreateGeometryShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), &gs)))
								{
									is_shader_validate_[type] = false;
								}
								else
								{
									geometry_shader_ = MakeCOMPtr(gs);
								}
							}
							break;

						default:
							is_shader_validate_[type] = false;
							break;
						}

						ID3D10ShaderReflection* reflection;
						render_eng.D3D10ReflectShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), &reflection);
						if (reflection != NULL)
						{
							D3D10_SHADER_DESC desc;
							reflection->GetDesc(&desc);

							dirty_[type].resize(desc.ConstantBuffers);
							d3d_cbufs_[type].resize(desc.ConstantBuffers);
							cbufs_[type].resize(desc.ConstantBuffers);
							for (UINT c = 0; c < desc.ConstantBuffers; ++ c)
							{
								ID3D10ShaderReflectionConstantBuffer* reflection_cb = reflection->GetConstantBufferByIndex(c);

								D3D10_SHADER_BUFFER_DESC cb_desc;
								reflection_cb->GetDesc(&cb_desc);
								cbufs_[type][c].resize(cb_desc.Size);

								for (UINT v = 0; v < cb_desc.Variables; ++ v)
								{
									ID3D10ShaderReflectionVariable* reflection_var = reflection_cb->GetVariableByIndex(v);

									D3D10_SHADER_VARIABLE_DESC var_desc;
									reflection_var->GetDesc(&var_desc);
									if (var_desc.uFlags & D3D10_SVF_USED)
									{
										RenderEffectParameterPtr const & p = effect.ParameterByName(var_desc.Name);
										if (p)
										{
											D3D10_SHADER_TYPE_DESC type_desc;
											reflection_var->GetType()->GetDesc(&type_desc);

											D3D10ShaderParameterHandle p_handle;
											p_handle.shader_type = static_cast<uint8_t>(type);
											p_handle.param_type = type_desc.Type;
											p_handle.cbuff = c;
											p_handle.offset = var_desc.StartOffset;
											p_handle.elements = type_desc.Elements;
											p_handle.rows = static_cast<uint8_t>(type_desc.Rows);
											p_handle.columns = static_cast<uint8_t>(type_desc.Columns);

											param_binds_[type].push_back(this->GetBindFunc(p_handle, p));
										}
									}
								}

								D3D10_BUFFER_DESC buf_desc;
								buf_desc.ByteWidth = (cb_desc.Size + 15) / 16 * 16;
								buf_desc.Usage = D3D10_USAGE_DYNAMIC;
								buf_desc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
								buf_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
								buf_desc.MiscFlags = 0;
								ID3D10Buffer* tmp_buf;
								TIF(d3d_device->CreateBuffer(&buf_desc, NULL, &tmp_buf));
								d3d_cbufs_[type][c] = MakeCOMPtr(tmp_buf);
							}

							int num_samplers = -1;
							int num_srvs = -1;
							for (uint32_t i = 0; i < desc.BoundResources; ++ i)
							{
								D3D10_SHADER_INPUT_BIND_DESC si_desc;
								reflection->GetResourceBindingDesc(i, &si_desc);

								switch (si_desc.Type)
								{
								case D3D10_SIT_SAMPLER:
									num_samplers = std::max(num_samplers, static_cast<int>(si_desc.BindPoint));
									break;

								case D3D10_SIT_TEXTURE:
									num_srvs = std::max(num_srvs, static_cast<int>(si_desc.BindPoint));
									break;

								default:
									break;
								}
							}

							samplers_[type].resize(num_samplers + 1, NULL);
							srvs_[type].resize(num_srvs + 1, NULL);

							for (uint32_t i = 0; i < desc.BoundResources; ++ i)
							{
								D3D10_SHADER_INPUT_BIND_DESC si_desc;
								reflection->GetResourceBindingDesc(i, &si_desc);

								if ((D3D10_SIT_TEXTURE == si_desc.Type) || (D3D10_SIT_SAMPLER == si_desc.Type))
								{
									RenderEffectParameterPtr const & p = effect.ParameterByName(si_desc.Name);
									if (p)
									{
										D3D10ShaderParameterHandle p_handle;
										p_handle.shader_type = static_cast<uint8_t>(type);
										if (D3D10_SIT_TEXTURE == si_desc.Type)
										{
											if (D3D10_SRV_DIMENSION_BUFFER == si_desc.Dimension)
											{
												p_handle.param_type = D3D10_SVT_BUFFER;
											}
											else
											{
												p_handle.param_type = D3D10_SVT_TEXTURE;
											}
										}
										else
										{
											p_handle.param_type = D3D10_SVT_SAMPLER;
										}
										p_handle.offset = si_desc.BindPoint;
										p_handle.elements = 1;
										p_handle.rows = 0;
										p_handle.columns = 1;

										param_binds_[type].push_back(this->GetBindFunc(p_handle, p));
									}
								}
							}

							reflection->Release();
						}
					}

					sd.tech_pass = (tech_index << 16) + pass_index;
				}
			}

			is_validate_ &= is_shader_validate_[type];
		}
	}

	ShaderObjectPtr D3D10ShaderObject::Clone(RenderEffect& effect)
	{
		ID3D10DevicePtr const & d3d_device = checked_cast<D3D10RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance())->D3DDevice();

		D3D10ShaderObjectPtr ret = MakeSharedPtr<D3D10ShaderObject>();
		ret->is_validate_ = is_validate_;
		ret->is_shader_validate_ = is_shader_validate_;
		ret->vertex_shader_ = vertex_shader_;
		ret->pixel_shader_ = pixel_shader_;
		ret->geometry_shader_ = geometry_shader_;
		ret->vs_code_ = vs_code_;
		for (size_t i = 0; i < ST_NumShaderTypes; ++ i)
		{
			ret->samplers_[i].resize(samplers_[i].size(), NULL);
			ret->srvs_[i].resize(srvs_[i].size(), NULL);

			ret->cbufs_[i] = cbufs_[i];
			ret->dirty_[i] = dirty_[i];

			ret->d3d_cbufs_[i].resize(d3d_cbufs_[i].size());
			D3D10_BUFFER_DESC desc;
			desc.Usage = D3D10_USAGE_DYNAMIC;
			desc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			for (size_t j = 0; j < d3d_cbufs_[i].size(); ++ j)
			{
				desc.ByteWidth = static_cast<UINT>((cbufs_[i][j].size() + 15) / 16 * 16);
				ID3D10Buffer* tmp_buf;
				TIF(d3d_device->CreateBuffer(&desc, NULL, &tmp_buf));
				ret->d3d_cbufs_[i][j] = MakeCOMPtr(tmp_buf);
			}

			ret->param_binds_[i].reserve(param_binds_[i].size());
			BOOST_FOREACH(BOOST_TYPEOF(param_binds_[i])::const_reference pb, param_binds_[i])
			{
				ret->param_binds_[i].push_back(ret->GetBindFunc(pb.p_handle, effect.ParameterByName(*(pb.param->Name()))));
			}
		}

		return ret;
	}

	D3D10ShaderObject::parameter_bind_t D3D10ShaderObject::GetBindFunc(D3D10ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param)
	{
		parameter_bind_t ret;
		ret.param = param;
		ret.p_handle = p_handle;

		switch (param->type())
		{
		case REDT_bool:
			if (param->ArraySize())
			{
				switch (p_handle.param_type)
				{
				case D3D10_SVT_BOOL:
					ret.func = SetD3D10ShaderParameter<bool*, BOOL>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_UINT:
					ret.func = SetD3D10ShaderParameter<bool*, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_INT:
					ret.func = SetD3D10ShaderParameter<bool*, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_FLOAT:
					ret.func = SetD3D10ShaderParameter<bool*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			else
			{
				switch (p_handle.param_type)
				{
				case D3D10_SVT_BOOL:
					ret.func = SetD3D10ShaderParameter<bool, BOOL>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_UINT:
					ret.func = SetD3D10ShaderParameter<bool, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_INT:
					ret.func = SetD3D10ShaderParameter<bool, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_FLOAT:
					ret.func = SetD3D10ShaderParameter<bool, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			break;

		case REDT_uint:
			if (param->ArraySize())
			{
				switch (p_handle.param_type)
				{
				case D3D10_SVT_BOOL:
					ret.func = SetD3D10ShaderParameter<uint32_t*, BOOL>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_UINT:
					ret.func = SetD3D10ShaderParameter<uint32_t*, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_INT:
					ret.func = SetD3D10ShaderParameter<uint32_t*, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_FLOAT:
					ret.func = SetD3D10ShaderParameter<uint32_t*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			else
			{
				switch (p_handle.param_type)
				{
				case D3D10_SVT_BOOL:
					ret.func = SetD3D10ShaderParameter<uint32_t, BOOL>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_UINT:
					ret.func = SetD3D10ShaderParameter<uint32_t, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_INT:
					ret.func = SetD3D10ShaderParameter<uint32_t, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_FLOAT:
					ret.func = SetD3D10ShaderParameter<uint32_t, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			break;

		case REDT_int:
			if (param->ArraySize())
			{
				switch (p_handle.param_type)
				{
				case D3D10_SVT_BOOL:
					ret.func = SetD3D10ShaderParameter<int32_t*, BOOL>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_UINT:
					ret.func = SetD3D10ShaderParameter<int32_t*, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_INT:
					ret.func = SetD3D10ShaderParameter<int32_t*, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_FLOAT:
					ret.func = SetD3D10ShaderParameter<int32_t*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			else
			{
				switch (p_handle.param_type)
				{
				case D3D10_SVT_BOOL:
					ret.func = SetD3D10ShaderParameter<int32_t, BOOL>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_UINT:
					ret.func = SetD3D10ShaderParameter<int32_t, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_INT:
					ret.func = SetD3D10ShaderParameter<int32_t, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				case D3D10_SVT_FLOAT:
					ret.func = SetD3D10ShaderParameter<int32_t, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
			break;

		case REDT_float:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<float*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<float, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_uint2:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<uint2*, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<uint2, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_uint3:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<uint3*, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<uint3, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_uint4:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<uint4*, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<uint4, uint32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_int2:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<int2*, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<int2, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_int3:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<int3*, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<int3, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_int4:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<int4*, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<int4, int32_t>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_float2:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<float2*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<float2, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_float3:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<float3*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<float3, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_float4:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<float4*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.elements, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<float4, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_float4x4:
			if (param->ArraySize())
			{
				ret.func = SetD3D10ShaderParameter<float4x4*, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.rows, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			else
			{
				ret.func = SetD3D10ShaderParameter<float4x4, float>(&cbufs_[p_handle.shader_type][p_handle.cbuff][p_handle.offset], p_handle.rows, param, &dirty_[p_handle.shader_type][p_handle.cbuff]);
			}
			break;

		case REDT_sampler:
			ret.func = SetD3D10ShaderParameter<SamplerStateObjectPtr, ID3D10SamplerState*>(samplers_[p_handle.shader_type][p_handle.offset], param);
			break;

		case REDT_texture1D:
		case REDT_texture2D:
		case REDT_texture3D:
		case REDT_textureCUBE:
		case REDT_texture1DArray:
		case REDT_texture2DArray:
		case REDT_texture3DArray:
		case REDT_textureCUBEArray:
			ret.func = SetD3D10ShaderParameter<TexturePtr, ID3D10ShaderResourceView*>(srvs_[p_handle.shader_type][p_handle.offset], param);
			break;

		case REDT_buffer:
			ret.func = SetD3D10ShaderParameter<GraphicsBufferPtr, ID3D10ShaderResourceView*>(srvs_[p_handle.shader_type][p_handle.offset], param);
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		return ret;
	}

	void D3D10ShaderObject::Bind()
	{
		D3D10RenderEngine& re = *checked_cast<D3D10RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D10DevicePtr const & d3d_device = re.D3DDevice();

		re.VSSetShader(vertex_shader_);
		re.GSSetShader(geometry_shader_);
		re.PSSetShader(pixel_shader_);

		for (size_t st = 0; st < ST_NumShaderTypes; ++ st)
		{
			BOOST_FOREACH(BOOST_TYPEOF(param_binds_[st])::reference pb, param_binds_[st])
			{
				pb.func();
			}
		}

		for (size_t i = 0; i < d3d_cbufs_.size(); ++ i)
		{
			for (size_t j = 0; j < d3d_cbufs_[i].size(); ++ j)
			{
				if (dirty_[i][j])
				{
					void* mapped;
					d3d_cbufs_[i][j]->Map(D3D10_MAP_WRITE_DISCARD, 0, &mapped);
					memcpy(mapped, &cbufs_[i][j][0], static_cast<UINT>(cbufs_[i][j].size()));
					d3d_cbufs_[i][j]->Unmap();

					dirty_[i][j] = false;
				}
			}
		}

		for (size_t st = 0; st < ST_NumShaderTypes; ++ st)
		{
			if (!srvs_[st].empty())
			{
				SetShaderResources_[st](d3d_device.get(), 0, static_cast<UINT>(srvs_[st].size()), &srvs_[st][0]);
			}

			if (!samplers_[st].empty())
			{
				SetSamplers_[st](d3d_device.get(), 0, static_cast<UINT>(samplers_[st].size()), &samplers_[st][0]);
			}

			if (!d3d_cbufs_[st].empty())
			{
				std::vector<ID3D10Buffer*> cb(d3d_cbufs_[st].size());
				for (size_t i = 0; i < cb.size(); ++ i)
				{
					cb[i] = d3d_cbufs_[st][i].get();
				}
				SetConstantBuffers_[st](d3d_device.get(), 0, static_cast<UINT>(cb.size()), &cb[0]);
			}
		}
	}

	void D3D10ShaderObject::Unbind()
	{
		D3D10RenderEngine& re = *checked_cast<D3D10RenderEngine*>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance());
		ID3D10DevicePtr const & d3d_device = re.D3DDevice();

		std::vector<ID3D10ShaderResourceView*> srvs;
		for (size_t st = 0; st < ST_NumShaderTypes; ++ st)
		{
			if (!srvs_[st].empty())
			{
				srvs.resize(srvs_[st].size(), NULL);
				SetShaderResources_[st](d3d_device.get(), 0, static_cast<UINT>(srvs.size()), &srvs[0]);
			}
		}
	}
}
