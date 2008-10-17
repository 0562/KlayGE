// ShaderObject.hpp
// KlayGE shader������ ͷ�ļ�
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

#ifndef _SHADEROBJECT_HPP
#define _SHADEROBJECT_HPP

#include <KlayGE/PreDeclare.hpp>

namespace KlayGE
{
	struct shader_desc
	{
		std::string profile;
		std::string func_name;

		friend bool operator==(shader_desc const & lhs, shader_desc const & rhs)
		{
			return (lhs.profile == rhs.profile) && (lhs.func_name == rhs.func_name);
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

			ST_NumShaderTypes
		};

	public:
		virtual ~ShaderObject()
		{
		}

		static ShaderObjectPtr NullObject();

		virtual void SetShader(RenderEffect& effect, ShaderType type, boost::shared_ptr<std::vector<shader_desc> > const & shader_descs,
			boost::shared_ptr<std::string> const & shader_text) = 0;
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
