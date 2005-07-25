// D3D9RenderEffect.hpp
// KlayGE D3D9��ȾЧ���� ͷ�ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ȥ����Clone (2005.4.16)
//
// 2.4.0
// ��Ϊ������D3D9Resource (2005.3.3)
//
// 2.3.0
// ������OnLostDevice��OnResetDevice (2005.2.23)
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
#include <boost/smart_ptr.hpp>

#include <map>

#include <d3dx9effect.h>

#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9RenderEffect;
	class D3D9RenderEffectParameter;

	typedef boost::shared_ptr<D3D9RenderEffect> D3D9RenderEffectPtr;
	typedef boost::shared_ptr<D3D9RenderEffectParameter> D3D9RenderEffectParameterPtr;

	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class D3D9RenderEffect : public RenderEffect, public D3D9Resource
	{
	public:
		explicit D3D9RenderEffect(std::string const & srcData);

		boost::shared_ptr<ID3DXEffect> const & D3DXEffect() const
			{ return effect_; }

		uint32_t HashCode() const;

		void Desc(uint32_t& parameters, uint32_t& techniques, uint32_t& functions);

		bool SetTechnique(std::string const & technique);
		bool SetTechnique(uint32_t technique);

		uint32_t Begin(uint32_t flags);
		void End();
		void BeginPass(uint32_t passNum);
		void EndPass();

	private:
		bool Validate(D3DXHANDLE handle);

		void DoOnLostDevice();
		void DoOnResetDevice();

		std::string DoNameBySemantic(std::string const & semantic);
		RenderEffectParameterPtr DoParameterByName(std::string const & name);

		bool DoSetTechnique(D3DXHANDLE handle);

	private:
		boost::shared_ptr<ID3DXEffect> effect_;

		uint32_t crc32_;
	};

	class D3D9RenderEffectParameter : public RenderEffectParameter, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameter(RenderEffect& effect, std::string const & name);

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
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3D9RenderEffectParameter(D3D9RenderEffectParameter const & rhs);
		D3D9RenderEffectParameter& operator=(D3D9RenderEffectParameter const & rhs);
	};
}

#endif		// _D3D9RENDEREFFECT_HPP
