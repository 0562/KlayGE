// RenderEffect.hpp
// KlayGE ��ȾЧ���� ͷ�ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ������Parameter (2004.5.26)
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

#ifndef _RENDEREFFECT_HPP
#define _RENDEREFFECT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <vector>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class RenderEffect
	{
	public:
		virtual ~RenderEffect()
			{ }

		static RenderEffectPtr NullObject();

		virtual RenderEffectPtr Clone() const = 0;

		virtual void Desc(UINT& parameters, UINT& techniques, UINT& functions) = 0;

		virtual RenderEffectParameterPtr Parameter(UINT index) = 0;
		virtual RenderEffectParameterPtr ParameterByName(const std::string& name) = 0;
		virtual RenderEffectParameterPtr ParameterBySemantic(const std::string& semantic) = 0;

		virtual void SetTechnique(const std::string& technique) = 0;
		virtual void SetTechnique(UINT technique) = 0;

		virtual UINT Begin(UINT flags = 0) = 0;
		virtual void Pass(UINT passNum) = 0;
		virtual void End() = 0;
	};

	class RenderEffectParameter
	{
	public:
		virtual ~RenderEffectParameter()
			{ }

		static RenderEffectParameterPtr NullObject();

		virtual RenderEffectParameter& operator=(float value) = 0;
		virtual RenderEffectParameter& operator=(const Vector4& value) = 0;
		virtual RenderEffectParameter& operator=(const Matrix4& value) = 0;
		virtual RenderEffectParameter& operator=(int value) = 0;
		virtual RenderEffectParameter& operator=(const TexturePtr& tex) = 0;

		virtual operator float() const = 0;
		virtual operator Vector4() const = 0;
		virtual operator Matrix4() const = 0;
		virtual operator int() const = 0;

		virtual void SetFloatArray(const float* value, size_t count) = 0;
		virtual void GetFloatArray(float* value, size_t count) = 0;
		virtual void SetVectorArray(const Vector4* value, size_t count) = 0;
		virtual void GetVectorArray(Vector4* value, size_t count) = 0;
		virtual void SetMatrixArray(const Matrix4* matrices, size_t count) = 0;
		virtual void GetMatrixArray(Matrix4* matrices, size_t count) = 0;
		virtual void SetIntArray(const int* value, size_t count) = 0;
		virtual void GetIntArray(int* value, size_t count) = 0;
	};

	RenderEffectPtr LoadRenderEffect(const std::string& effectName);
}

#endif		// _RENDEREFFECT_HPP
