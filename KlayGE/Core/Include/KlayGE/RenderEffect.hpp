// RenderEffect.hpp
// KlayGE ��ȾЧ���� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDEREFFECT_HPP
#define _RENDEREFFECT_HPP

#include <KlayGE/PreDeclare.hpp>

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

		virtual void Desc(UINT& parameters, UINT& techniques, UINT& functions) = 0;

		virtual void SetValue(const String& name, const void* data, UINT bytes) = 0;
		virtual void* GetValue(const String& name, UINT bytes) const = 0;

		virtual void SetFloat(const String& name, float value) = 0;
		virtual float GetFloat(const String& name) const = 0;
		virtual void SetVector(const String& name, const Vector4& value) = 0;
		virtual Vector4 GetVector(const String& name) const = 0;
		virtual void SetMatrix(const String& name, const Matrix4& value) = 0;
		virtual Matrix4 GetMatrix(const String& name) const = 0;
		virtual void SetInt(const String& name, int value) = 0;
		virtual int GetInt(const String& name) const = 0;
		virtual void SetBool(const String& name, bool value) = 0;
		virtual bool GetBool(const String& name) const = 0;
		virtual void SetString(const String& name, const String& value) = 0;
		virtual String GetString(const String& name) const = 0;

		virtual void SetTexture(const String& name, const Texture& tex) = 0;
		virtual void SetVertexShader(const String& name, U32 vsHandle) = 0;
		virtual void SetPixelShader(const String& name, U32 psHandle) = 0;

		virtual void Technique(const String& technique) = 0;
		virtual void Technique(UINT technique) = 0;
		virtual void Validate() = 0;

		virtual UINT Begin(UINT flags = 0) = 0;
		virtual void Pass(UINT passNum) = 0;
		virtual void End() = 0;
	};
}

#endif		// _RENDEREFFECT_HPP
