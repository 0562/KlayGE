// D3D9RenderEffect.hpp
// KlayGE D3D9��ȾЧ���� ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ������D3D9RenderTechnique (2004.3.16)
//
// 2.0.3
// �޸���SetTexture�Ĳ��� (2004.3.6)
// ������SetMatrixArray/GetMatrixArray (2004.3.11)
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDEREFFECT_HPP
#define _D3D9RENDEREFFECT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/COMPtr.hpp>

#include <d3dx9effect.h>

#include <KlayGE/RenderEffect.hpp>

#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")

namespace KlayGE
{
	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class D3D9RenderEffect : public RenderEffect
	{
	public:
		D3D9RenderEffect(const std::string& srcData, UINT flags = 0);
		D3D9RenderEffect(const D3D9RenderEffect& rhs);

		const COMPtr<ID3DXEffect>& D3DXEffect() const
			{ return effect_; }

		RenderEffectPtr Clone() const;

		void Desc(UINT& parameters, UINT& techniques, UINT& functions);

		RenderEffectParameterPtr Parameter(UINT index);
		RenderEffectParameterPtr ParameterByName(const std::string& name);
		RenderEffectParameterPtr ParameterBySemantic(const std::string& semantic);

		void SetTechnique(const std::string& technique);
		void SetTechnique(UINT technique);

		bool Validate(D3DXHANDLE handle);

		UINT Begin(UINT flags = 0);
		void Pass(UINT passNum);
		void End();

	private:
		COMPtr<ID3DXEffect> effect_;
	};

	class D3D9RenderEffectParameter : public RenderEffectParameter
	{
	public:
		D3D9RenderEffectParameter(COMPtr<ID3DXEffect> effect, D3DXHANDLE parameter);

		void SetFloat(float value);
		void SetFloatArray(const float* value, size_t count);
		float GetFloat() const;
		void GetFloatArray(float* value, size_t count);

		void SetVector(const Vector4& value);
		void SetVectorArray(const Vector4* value, size_t count);
		Vector4 GetVector() const;
		void GetVectorArray(Vector4* value, size_t count);

		void SetMatrix(const Matrix4& value);
		void SetMatrixArray(const Matrix4* matrices, size_t count);
		Matrix4 GetMatrix() const;
		void GetMatrixArray(Matrix4* matrices, size_t count);

		void SetInt(int value);
		void SetIntArray(const int* value, size_t count);
		int GetInt() const;
		void GetIntArray(int* value, size_t count);

		void SetTexture(const TexturePtr& tex);

	private:
		COMPtr<ID3DXEffect> effect_;
		D3DXHANDLE parameter_;
	};
}

#endif		// _D3D9RENDEREFFECT_HPP
