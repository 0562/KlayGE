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
	};
}

#endif		// _OGLRENDEREFFECT_HPP
