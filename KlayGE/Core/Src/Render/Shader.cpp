// RenderEffect.hpp
// KlayGE ��ȾЧ���� ͷ�ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// ���ν��� (2003.3.2)
// �޸���SetTexture�Ĳ��� (2004.3.6)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Config.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Engine.hpp>
#include <KlaygE/RenderFactory.hpp>

#include <KlayGE/PackedFile/PackedFile.hpp>
#include <KlayGE/DiskFile/DiskFile.hpp>

#include <KlayGE/Shader.hpp>

namespace KlayGE
{
	class NullShaderParameter : public ShaderParameter
	{
	public:
		void SetBool(bool b)
			{ }
		void SetBoolArray(const bool* b, size_t count)
			{ }
		void SetFloat(float f)
			{ }
		void SetFloatArray(const float* f, size_t count)
			{ }
		void SetInt(int i)
			{ }
		void SetIntArray(const int* i, size_t count)
			{ }
		void SetMatrix(const Matrix4& mat)
			{ }
		void SetMatrixArray(const Matrix4* mat, size_t count)
			{ }
		void SetVector(const Vector4& vec)
			{ }
		void SetVectorArray(const Vector4* vec, size_t count)
			{ }
	};

	class NullVertexShader : public VertexShader
	{
	public:
		void Active()
			{ }
		ShaderParameterPtr GetNamedParameter(const String& /*name*/)
			{ return ShaderParameter::NullObject(); }
	};

	class NullPixelShader : public PixelShader
	{
	public:
		void Active()
			{ }
		ShaderParameterPtr GetNamedParameter(const String& /*name*/)
			{ return ShaderParameter::NullObject(); }
	};

	ShaderParameterPtr ShaderParameter::NullObject()
	{
		static ShaderParameterPtr obj(new NullShaderParameter);
		return obj;
	}

	VertexShaderPtr VertexShader::NullObject()
	{
		static VertexShaderPtr obj(new NullVertexShader);
		return obj;
	}

	PixelShaderPtr PixelShader::NullObject()
	{
		static PixelShaderPtr obj(new NullPixelShader);
		return obj;
	}

	VertexShaderPtr LoadVertexShader(const String& shaderFileName,
		const String& functionName, const String& profile,
		bool fromPack)
	{
		VFilePtr file;
		if (fromPack)
		{
			SharedPtr<PackedFile> packedFile(new PackedFile);

			if (!packedFile->Open(shaderFileName))
			{
				if (!packedFile->Open(String(_RENDERFXPATH_) + shaderFileName))
				{
					return VertexShader::NullObject();
				}
			}

			file = packedFile;
		}
		else
		{
			SharedPtr<DiskFile> diskFile(new DiskFile);

			if (!diskFile->Open(shaderFileName, VFile::OM_Read))
			{
				if (!diskFile->Open(String(_RENDERFXPATH_) + shaderFileName, VFile::OM_Read))
				{
					return VertexShader::NullObject();
				}
			}

			file = diskFile;
		}

		std::vector<char, alloc<char> > data(file->Length());
		file->Read(&data[0], data.size());

		return Engine::RenderFactoryInstance().MakeVertexShader(String(&data[0], data.size()), functionName, profile);
	}

	PixelShaderPtr LoadPixelShader(const String& shaderFileName,
		const String& functionName, const String& profile,
		bool fromPack)
	{
		VFilePtr file;
		if (fromPack)
		{
			SharedPtr<PackedFile> packedFile(new PackedFile);

			if (!packedFile->Open(shaderFileName))
			{
				if (!packedFile->Open(String(_RENDERFXPATH_) + shaderFileName))
				{
					return PixelShader::NullObject();
				}
			}

			file = packedFile;
		}
		else
		{
			SharedPtr<DiskFile> diskFile(new DiskFile);

			if (!diskFile->Open(shaderFileName, VFile::OM_Read))
			{
				if (!diskFile->Open(String(_RENDERFXPATH_) + shaderFileName, VFile::OM_Read))
				{
					return PixelShader::NullObject();
				}
			}

			file = diskFile;
		}

		std::vector<char, alloc<char> > data(file->Length());
		file->Read(&data[0], data.size());

		return Engine::RenderFactoryInstance().MakePixelShader(String(&data[0], data.size()), functionName, profile);
	}
}
