// Shader.hpp
// KlayGE ��Ⱦ���� ʵ���ļ�
// Ver 2.1.0
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.0
// ���ν��� (2004.4.18)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SHADER_HPP
#define _SHADER_HPP

namespace KlayGE
{
	class ShaderParameter
	{
	public:
		virtual ~ShaderParameter()
			{ }

		static ShaderParameterPtr NullObject();

		virtual void SetFloat(float f) = 0;
		virtual void SetFloatArray(const float* f, size_t count) = 0;
		virtual void SetMatrix(const Matrix4& mat) = 0;
		virtual void SetMatrixArray(const Matrix4* mat, size_t count) = 0;
		virtual void SetVector(const Vector4& vec) = 0;
		virtual void SetVectorArray(const Vector4* vec, size_t count) = 0;
	};

	class VertexShader
	{
	public:
		virtual ~VertexShader()
			{ }

		static VertexShaderPtr NullObject();

		virtual void Active() = 0;
		virtual ShaderParameterPtr GetNamedParameter(const std::string& name) = 0;
	};

	class PixelShader
	{
	public:
		virtual ~PixelShader()
			{ }

		static PixelShaderPtr NullObject();

		virtual void Active() = 0;
		virtual ShaderParameterPtr GetNamedParameter(const std::string& name) = 0;
	};

	VertexShaderPtr LoadVertexShader(const std::string& shaderFileName,
		const std::string& functionName, const std::string& profile,
		bool fromPack = false);
	PixelShaderPtr LoadPixelShader(const std::string& shaderFileName,
		const std::string& functionName, const std::string& profile,
		bool fromPack = false);
}

#endif		// _SHADER_HPP
