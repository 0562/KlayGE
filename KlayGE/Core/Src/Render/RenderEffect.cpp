// RenderEffect.cpp
// KlayGE ��ȾЧ���� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ������Parameter (2004.5.26)
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
#include <KlayGE/Context.hpp>
#include <KlaygE/RenderFactory.hpp>

#include <KlayGE/PackedFile/PackedFile.hpp>
#include <KlayGE/DiskFile/DiskFile.hpp>

#include <KlayGE/RenderEffect.hpp>

namespace KlayGE
{
	class NullRenderEffect : public RenderEffect
	{
	public:
		void Desc(UINT& parameters, UINT& techniques, UINT& functions)
		{
			parameters = 0;
			techniques = 0;
			functions = 0;
		}

		RenderEffectPtr Clone() const
			{ return RenderEffect::NullObject(); }

		RenderEffectParameterPtr Parameter(UINT index)
			{ return RenderEffectParameter::NullObject(); }
		RenderEffectParameterPtr ParameterByName(const std::string& name)
			{ return RenderEffectParameter::NullObject(); }
		RenderEffectParameterPtr ParameterBySemantic(const std::string& semantic)
			{ return RenderEffectParameter::NullObject(); }

		void SetTechnique(const std::string& techName)
			{ }
		void SetTechnique(UINT tech)
			{ }

		UINT Begin(UINT flags = 0)
			{ return 1; }
		void Pass(UINT passNum)
			{ }
		void End()
			{ }
	};

	RenderEffectPtr RenderEffect::NullObject()
	{
		static RenderEffectPtr obj(new NullRenderEffect);
		return obj;
	}


	class NullRenderEffectParameter : public RenderEffectParameter
	{
	public:
		void SetFloat(float value)
			{ }
		void SetFloatArray(const float* matrices, size_t count)
			{ }
		float GetFloat() const
			{ return 0; }
		void GetFloatArray(float* matrices, size_t count)
			{ }

		void SetVector(const Vector4& value)
			{ }
		void SetVectorArray(const Vector4* matrices, size_t count)
			{ }
		Vector4 GetVector() const
			{ return Vector4::Zero(); }
		void GetVectorArray(Vector4* matrices, size_t count)
			{ }

		void SetMatrix(const Matrix4& value)
			{ }
		void SetMatrixArray(const Matrix4* matrices, size_t count)
			{ }
		Matrix4 GetMatrix() const
			{ return Matrix4::Identity(); }
		void GetMatrixArray(Matrix4* matrices, size_t count)
			{ }

		void SetInt(int value)
			{ }
		void SetIntArray(const int* matrices, size_t count)
			{ }
		int GetInt() const
			{ return 0; }
		void GetIntArray(int* matrices, size_t count)
			{ }

		void SetTexture(const TexturePtr& tex)
			{ }
	};

	RenderEffectParameterPtr RenderEffectParameter::NullObject()
	{
		static RenderEffectParameterPtr obj(new NullRenderEffectParameter);
		return obj;
	}


	RenderEffectPtr LoadRenderEffect(const std::string& effectName)
	{
		VFilePtr file(ResLocator::Instance().Locate(effectName)->Load());

		std::vector<char> data(file->Length());
		file->Read(&data[0], data.size());

		return Context::Instance().RenderFactoryInstance().MakeRenderEffect(std::string(&data[0], data.size()));
	}
}
