// ShaderObject.hpp
// KlayGE shader������ ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2006-2009
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ֧��Gemoetry Shader (2009.2.5)
//
// 3.7.0
// ��Ϊֱ�Ӵ���RenderEffect (2008.7.4)
//
// 3.5.0
// ���ν��� (2006.11.2)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SHADEROBJECT_HPP
#define _SHADEROBJECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderLayout.hpp>

namespace KlayGE
{
	struct shader_desc
	{
		shader_desc()
			: tech_pass(0xFFFFFFFF)
		{
		}

		std::string profile;
		std::string func_name;

		uint32_t tech_pass;

		struct stream_output_decl
		{
			VertexElementUsage usage;
			uint8_t usage_index;
			uint8_t start_component;
			uint8_t component_count;

			friend bool operator==(stream_output_decl const & lhs, stream_output_decl const & rhs)
			{
				return (lhs.usage == rhs.usage) && (lhs.usage_index == rhs.usage_index)
					&& (lhs.start_component == rhs.start_component) && (lhs.component_count == rhs.component_count);
			}
			friend bool operator!=(stream_output_decl const & lhs, stream_output_decl const & rhs)
			{
				return !(lhs == rhs);
			}
		};
		std::vector<stream_output_decl> so_decl;

		friend bool operator==(shader_desc const & lhs, shader_desc const & rhs)
		{
			return (lhs.profile == rhs.profile) && (lhs.func_name == rhs.func_name) && (lhs.so_decl == rhs.so_decl);
		}
		friend bool operator!=(shader_desc const & lhs, shader_desc const & rhs)
		{
			return !(lhs == rhs);
		}
	};

	class KLAYGE_CORE_API ShaderObject
	{
	public:
		enum ShaderType
		{
			ST_VertexShader,
			ST_PixelShader,
			ST_GeometryShader,

			ST_NumShaderTypes
		};

	public:
		virtual ~ShaderObject()
		{
		}

		static ShaderObjectPtr NullObject();

		virtual void SetShader(RenderEffect& effect, boost::shared_ptr<std::vector<uint32_t> > const & shader_desc_ids,
			uint32_t tech_index, uint32_t pass_index) = 0;
		virtual ShaderObjectPtr Clone(RenderEffect& effect) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		bool Validate() const
		{
			return is_validate_;
		}

	protected:
		bool is_validate_;
	};
}

#endif			// _SHADEROBJECT_HPP
