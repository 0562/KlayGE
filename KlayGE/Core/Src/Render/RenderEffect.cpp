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

		void SetValue(const String& name, const void* data, UINT bytes)
			{ }
		void* GetValue(const String& name, UINT bytes) const
			{ return NULL; }

		void SetFloat(const String& name, float value)
			{ }
		float GetFloat(const String& name) const
			{ return 0; }
		void SetVector(const String& name, const Vector4& value)
			{ }
		Vector4 GetVector(const String& name) const
			{ return Vector4::Zero(); }
		void SetMatrix(const String& name, const Matrix4& value)
			{ }
		Matrix4 GetMatrix(const String& name) const
			{ return Matrix4::Identity(); }
		void SetMatrixArray(const String& name, const std::vector<Matrix4, alloc<Matrix4> >& matrices)
			{ }
		void GetMatrixArray(const String& name, std::vector<Matrix4, alloc<Matrix4> >& matrices)
			{ }
		void SetInt(const String& name, int value)
			{ }
		int GetInt(const String& name) const
			{ return 0; }
		void SetBool(const String& name, bool value)
			{ }
		bool GetBool(const String& name) const
			{ return false; }
		void SetString(const String& name, const String& value)
			{ }
		String GetString(const String& name) const
		{
			static String str;
			return str;
		}

		void SetTexture(const String& name, const TexturePtr& tex)
			{ }
		void SetVertexShader(const String& name, U32 vsHandle)
			{ }
		void SetPixelShader(const String& name, U32 psHandle)
			{ }

		void SetTechnique(const String& techName)
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

	RenderEffectPtr LoadRenderEffect(const String& effectName, bool fromPack)
	{
		VFilePtr file;
		if (fromPack)
		{
			SharedPtr<PackedFile> packedFile(new PackedFile);

			if (!packedFile->Open(effectName))
			{
				if (!packedFile->Open(String(_RENDERFXPATH_) + effectName))
				{
					return RenderEffect::NullObject();
				}
			}

			file = packedFile;
		}
		else
		{
			SharedPtr<DiskFile> diskFile(new DiskFile);

			if (!diskFile->Open(effectName, VFile::OM_Read))
			{
				if (!diskFile->Open(String(_RENDERFXPATH_) + effectName, VFile::OM_Read))
				{
					return RenderEffect::NullObject();
				}
			}

			file = diskFile;
		}

		std::vector<char, alloc<char> > data(file->Length());
		file->Read(&data[0], data.size());

		return Engine::RenderFactoryInstance().MakeRenderEffect(String(&data[0], data.size()));
	}
}
