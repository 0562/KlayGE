// ShaderObject.hpp
// KlayGE shader������ ͷ�ļ�
// Ver 3.5.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
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
	class ShaderObject
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

		virtual void SetShader(ShaderType type, std::string const & profile, std::string const & name, std::string const & text) = 0;

		virtual bool HasParameter(ShaderType type, std::string const & name) const = 0;

		virtual void SetParameter(std::string const & name, bool value) = 0;
		virtual void SetParameter(std::string const & name, int value) = 0;
		virtual void SetParameter(std::string const & name, float value) = 0;
		virtual void SetParameter(std::string const & name, float4 const & value) = 0;
		virtual void SetParameter(std::string const & name, float4x4 const & value) = 0;
		virtual void SetParameter(std::string const & name, SamplerPtr const & value) = 0;
		virtual void SetParameter(std::string const & name, std::vector<bool> const & value) = 0;
		virtual void SetParameter(std::string const & name, std::vector<int> const & value) = 0;
		virtual void SetParameter(std::string const & name, std::vector<float> const & value) = 0;
		virtual void SetParameter(std::string const & name, std::vector<float4> const & value) = 0;
		virtual void SetParameter(std::string const & name, std::vector<float4x4> const & value) = 0;

		bool Validate() const
		{
			return is_validate_;
		}

	protected:
		bool is_validate_;
	};
}

#endif			// _SHADEROBJECT_HPP
