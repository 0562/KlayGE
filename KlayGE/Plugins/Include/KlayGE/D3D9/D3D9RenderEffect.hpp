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
		D3D9RenderEffect(const String& srcData, UINT flags = 0);

		const COMPtr<ID3DXEffect>& D3DXEffect() const
			{ return effect_; }

		void Desc(UINT& parameters, UINT& techniques, UINT& functions);

		void SetValue(const String& name, const void* data, UINT bytes);
		void* GetValue(const String& name, UINT bytes) const;

		void SetFloat(const String& name, float value);
		float GetFloat(const String& name) const;
		void SetVector(const String& name, const Vector4& value);
		Vector4 GetVector(const String& name) const;
		void SetMatrix(const String& name, const Matrix4& value);
		Matrix4 GetMatrix(const String& name) const;
		void SetMatrixArray(const String& name, const std::vector<Matrix4, alloc<Matrix4> >& matrices);
		void GetMatrixArray(const String& name, std::vector<Matrix4, alloc<Matrix4> >& matrices);
		void SetInt(const String& name, int value);
		int GetInt(const String& name) const;
		void SetBool(const String& name, bool value);
		bool GetBool(const String& name) const;
		void SetString(const String& name, const String& value);
		String GetString(const String& name) const;

		void SetTexture(const String& name, const TexturePtr& tex);
		void SetVertexShader(const String& name, U32 vsHandle);
		void SetPixelShader(const String& name, U32 psHandle);

		RenderTechniquePtr GetTechnique(const RenderEffectPtr& effect, const String& technique);
		RenderTechniquePtr GetTechnique(const RenderEffectPtr& effect, UINT technique);

		bool Validate(const RenderTechniquePtr& technique);

	private:
		COMPtr<ID3DXEffect> effect_;
	};

	class D3D9RenderTechnique : public RenderTechnique
	{
	public:
		D3D9RenderTechnique(D3DXHANDLE tech, const RenderEffectPtr& effect)
			: d3dxTechnique_(tech)
			{ effect_ = effect; }

		D3DXHANDLE D3DXHandle() const
			{ return d3dxTechnique_; }

		void SetAsCurrent();

		UINT Begin(UINT flags = 0);
		void Pass(UINT passNum);
		void End();

	private:
		D3DXHANDLE d3dxTechnique_;
	};
}

#endif		// _D3D9RENDEREFFECT_HPP
