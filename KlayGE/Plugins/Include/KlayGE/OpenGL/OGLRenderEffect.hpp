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

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class OGLRenderEffect : public RenderEffect
	{
	public:
		OGLRenderEffect(std::string const & srcData, uint32_t flags = 0);
		OGLRenderEffect(OGLRenderEffect const & rhs);

		RenderEffectPtr Clone() const;

		void Desc(uint32_t& parameters, uint32_t& techniques, uint32_t& functions);

		RenderEffectParameterPtr Parameter(uint32_t index);
		RenderEffectParameterPtr ParameterByName(std::string const & name);
		RenderEffectParameterPtr ParameterBySemantic(std::string const & semantic);

		void SetTechnique(std::string const & technique);
		void SetTechnique(uint32_t technique);

		uint32_t Begin(uint32_t flags = 0);
		void BeginPass(uint32_t passNum);
		void EndPass();
		void End();
	};

	class OGLRenderEffectParameter : public RenderEffectParameter
	{
	public:
		RenderEffectParameter& operator=(float value);
		RenderEffectParameter& operator=(Vector4 const & value);
		RenderEffectParameter& operator=(Matrix4 const & value);
		RenderEffectParameter& operator=(int value);
		RenderEffectParameter& operator=(TexturePtr const & tex);

		operator float() const;
		operator Vector4() const;
		operator Matrix4() const;
		operator int() const;

		void SetFloatArray(float const * value, size_t count);
		void GetFloatArray(float* value, size_t count);
		void SetVectorArray(Vector4 const * value, size_t count);
		void GetVectorArray(Vector4* value, size_t count);
		void SetMatrixArray(Matrix4 const * matrices, size_t count);
		void GetMatrixArray(Matrix4* matrices, size_t count);
		void SetIntArray(int const * value, size_t count);
		void GetIntArray(int* value, size_t count);
	};
}

#endif		// _OGLRENDEREFFECT_HPP
