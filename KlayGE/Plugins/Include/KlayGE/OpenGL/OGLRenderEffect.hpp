// OGLRenderEffect.hpp
// KlayGE OpenGL��ȾЧ���� ͷ�ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ȥ����Clone (2005.4.16)
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

#ifdef KLAYGE_DEBUG
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
		explicit OGLRenderEffect(std::string const & srcData);

		uint32_t HashCode() const;

		void Desc(uint32_t& parameters, uint32_t& techniques, uint32_t& functions);

		bool SetTechnique(std::string const & technique);
		bool SetTechnique(uint32_t technique);

		uint32_t Begin(uint32_t flags);
		void End();
		void BeginPass(uint32_t passNum);
		void EndPass();

	private:
		std::string DoNameBySemantic(std::string const & semantic);
		RenderEffectParameterPtr DoParameterByName(std::string const & name);

	private:
		uint32_t crc32_;
	};

	class OGLRenderEffectParameter : public RenderEffectParameter
	{
	public:

	private:
		bool DoTestType(RenderEffectParameterType type);

		void DoFloat(float value);
		void DoVector4(Vector4 const & value);
		void DoMatrix4(Matrix4 const & value);
		void DoInt(int value);
		void DoTexture(TexturePtr const & value);

		void DoSetFloatArray(float const * value, size_t count);
		void DoSetVector4Array(Vector4 const * value, size_t count);
		void DoSetMatrix4Array(Matrix4 const * matrices, size_t count);
		void DoSetIntArray(int const * value, size_t count);

	private:
		OGLRenderEffectParameter(OGLRenderEffectParameter const & rhs);
		OGLRenderEffectParameter& operator=(OGLRenderEffectParameter const & rhs);
	};
}

#endif		// _OGLRENDEREFFECT_HPP
