// OGLRenderEffect.hpp
// KlayGE OpenGL��ȾЧ���� ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ���ν��� (2004.4.4)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDEREFFECT_HPP
#define _OGLRENDEREFFECT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderEffect.hpp>

#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")

namespace KlayGE
{
	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class OGLRenderEffect : public RenderEffect
	{
	public:
		OGLRenderEffect(const std::string& srcData, UINT flags = 0);
		OGLRenderEffect(const OGLRenderEffect& rhs);

		RenderEffectPtr Clone() const;

		void Desc(UINT& parameters, UINT& techniques, UINT& functions);

		RenderEffectParameterPtr Parameter(UINT index);
		RenderEffectParameterPtr ParameterByName(const std::string& name);
		RenderEffectParameterPtr ParameterBySemantic(const std::string& semantic);

		void SetTechnique(const std::string& technique);
		void SetTechnique(UINT technique);

		UINT Begin(UINT flags = 0);
		void Pass(UINT passNum);
		void End();
	};

	class OGLRenderEffectParameter : public RenderEffectParameter
	{
	public:
		void SetFloat(float value);
		float GetFloat() const;
		void SetVector(const Vector4& value);
		Vector4 GetVector() const;
		void SetMatrix(const Matrix4& value);
		Matrix4 GetMatrix() const;
		void SetMatrixArray(const Matrix4* matrices, size_t count);
		void GetMatrixArray(Matrix4* matrices, size_t count);
		void SetInt(int value);
		int GetInt() const;
		void SetBool(bool value);
		bool GetBool() const;
		void SetString(const std::string& value);
		std::string GetString() const;

		void SetTexture(const TexturePtr& tex);
	};
}

#endif		// _OGLRENDEREFFECT_HPP
