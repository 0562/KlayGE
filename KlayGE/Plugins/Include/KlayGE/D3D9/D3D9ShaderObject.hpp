// D3D9ShaderObject.hpp
// KlayGE D3D9 shader������ ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2006-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ��Ϊֱ�Ӵ���RenderEffect (2008.7.4)
//
// 3.5.0
// ���ν��� (2006.11.2)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9SHADEROBJECT_HPP
#define _D3D9SHADEROBJECT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ShaderObject.hpp>
#include <KlayGE/MapVector.hpp>

#include <boost/function.hpp>

#include <KlayGE/D3D9/D3D9Typedefs.hpp>

namespace KlayGE
{
#ifdef KLAYGE_PLATFORM_WINDOWS
#pragma pack(push, 2)
#endif
	struct D3D9ShaderParameterHandle
	{
		uint8_t shader_type;
		uint8_t register_set;

		uint16_t register_index;
		uint16_t register_count;

		uint8_t rows;
		uint8_t columns;
	};
#ifdef KLAYGE_PLATFORM_WINDOWS
#pragma pack(pop)
#endif

	class D3D9ShaderObject : public ShaderObject
	{
	public:
		D3D9ShaderObject();

		void SetShader(RenderEffect& effect, ShaderType type, boost::shared_ptr<std::vector<shader_desc> > const & shader_descs,
			boost::shared_ptr<std::string> const & shader_text);
		ShaderObjectPtr Clone(RenderEffect& effect);

		void Active();

	private:
		void SetBool(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetInt(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat2(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat3(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat4(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat4x4(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetBoolArray(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetIntArray(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloatArray(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat4Array(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetFloat4x4Array(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);
		void SetSampler(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);

		struct parameter_bind_t
		{
			RenderEffectParameterPtr param;
			D3D9ShaderParameterHandle p_handle;
			boost::function<void(D3D9ShaderParameterHandle const &, RenderEffectParameterPtr const &)> func;
		};
		typedef std::vector<parameter_bind_t> parameter_binds_t;

		parameter_bind_t GetBindFunc(D3D9ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);

	private:
		boost::array<parameter_binds_t, ST_NumShaderTypes> param_binds_;
		boost::array<bool, ST_NumShaderTypes> is_shader_validate_;

		ID3D9VertexShaderPtr vertex_shader_;
		ID3D9PixelShaderPtr pixel_shader_;

		boost::array<uint32_t, ST_NumShaderTypes> bool_start_;
		boost::array<uint32_t, ST_NumShaderTypes> int_start_;
		boost::array<uint32_t, ST_NumShaderTypes> float_start_;
		boost::array<std::vector<BOOL>, ST_NumShaderTypes> bool_registers_;
		boost::array<std::vector<int>, ST_NumShaderTypes> int_registers_;
		boost::array<std::vector<float>, ST_NumShaderTypes> float_registers_;
		boost::array<std::vector<SamplerPtr>, ST_NumShaderTypes> samplers_;
	};

	typedef boost::shared_ptr<D3D9ShaderObject> D3D9ShaderObjectPtr;
}

#endif			// _D3D9SHADEROBJECT_HPP
