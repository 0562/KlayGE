// D3D9RenderEffect.hpp
// KlayGE D3D9��ȾЧ���� ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// �Ż���Sampler���� (2005.9.7)
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
#include <KlayGE/MapVector.hpp>

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
	class D3D9RenderEffectParameterSampler;

	typedef boost::shared_ptr<D3D9RenderEffect> D3D9RenderEffectPtr;

	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class D3D9RenderEffect : public RenderEffect, public D3D9Resource
	{
	public:
		explicit D3D9RenderEffect(std::string const & srcData);

		boost::shared_ptr<ID3DXEffect> const & D3DXEffect() const
			{ return d3dx_effect_; }

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

		std::string DoNameBySemantic(std::string const & semantic);
		RenderEffectParameterPtr DoParameterByName(std::string const & name);

		uint32_t DoBegin(uint32_t flags);
		void DoEnd();

		void DoActiveTechnique();

		RenderTechniquePtr MakeRenderTechnique(uint32_t n);

	private:
		boost::shared_ptr<ID3DXEffect> d3dx_effect_;
	};

	class D3D9RenderTechnique : public RenderTechnique
	{
	public:
		D3D9RenderTechnique(RenderEffect& effect, std::string const & name, D3DXHANDLE tech);

		bool Validate();
		void Active();

	private:
		RenderPassPtr MakeRenderPass(uint32_t n);

	private:
		D3DXHANDLE tech_;
	};

	class D3D9RenderPass : public RenderPass
	{
	public:
		D3D9RenderPass(RenderEffect& effect, uint32_t index, D3DXHANDLE pass);

		void Begin();
		void End();

	private:
		D3DXHANDLE pass_;

		boost::shared_ptr<ID3DXConstantTable> constant_table_[2];
		MapVector<RenderEffectParameterPtr, uint32_t> samplers_[2];
	};

	class D3D9RenderEffectParameterBool : public RenderEffectParameterConcrete<bool>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterBool(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<bool>(effect, name)
		{
		}

	private:
		void DoFlush(bool const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterBool(D3D9RenderEffectParameterBool const & rhs);
		D3D9RenderEffectParameterBool& operator=(D3D9RenderEffectParameterBool const & rhs);
	};

	class D3D9RenderEffectParameterInt : public RenderEffectParameterConcrete<int>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterInt(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<int>(effect, name)
		{
		}

	private:
		void DoFlush(int const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterInt(D3D9RenderEffectParameterInt const & rhs);
		D3D9RenderEffectParameterInt& operator=(D3D9RenderEffectParameterInt const & rhs);
	};

	class D3D9RenderEffectParameterFloat : public RenderEffectParameterConcrete<float>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterFloat(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<float>(effect, name)
		{
		}

	private:
		void DoFlush(float const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterFloat(D3D9RenderEffectParameterFloat const & rhs);
		D3D9RenderEffectParameterFloat& operator=(D3D9RenderEffectParameterFloat const & rhs);
	};

	class D3D9RenderEffectParameterVector2 : public RenderEffectParameterConcrete<Vector2>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterVector2(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<Vector2>(effect, name)
		{
		}

	private:
		void DoFlush(Vector2 const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterVector2(D3D9RenderEffectParameterVector2 const & rhs);
		D3D9RenderEffectParameterVector2& operator=(D3D9RenderEffectParameterVector2 const & rhs);
	};

	class D3D9RenderEffectParameterVector3 : public RenderEffectParameterConcrete<Vector3>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterVector3(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<Vector3>(effect, name)
		{
		}

	private:
		void DoFlush(Vector3 const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterVector3(D3D9RenderEffectParameterVector3 const & rhs);
		D3D9RenderEffectParameterVector3& operator=(D3D9RenderEffectParameterVector3 const & rhs);
	};

	class D3D9RenderEffectParameterVector4 : public RenderEffectParameterConcrete<Vector4>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterVector4(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<Vector4>(effect, name)
		{
		}

	private:
		void DoFlush(Vector4 const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterVector4(D3D9RenderEffectParameterVector4 const & rhs);
		D3D9RenderEffectParameterVector4& operator=(D3D9RenderEffectParameterVector4 const & rhs);
	};

	class D3D9RenderEffectParameterMatrix4 : public RenderEffectParameterConcrete<Matrix4>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterMatrix4(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<Matrix4>(effect, name)
		{
		}

	private:
		void DoFlush(Matrix4 const & value);

	private:
		void DoOnLostDevice()
		{
		}
		void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterMatrix4(D3D9RenderEffectParameterMatrix4 const & rhs);
		D3D9RenderEffectParameterMatrix4& operator=(D3D9RenderEffectParameterMatrix4 const & rhs);
	};

	class D3D9RenderEffectParameterSampler : public RenderEffectParameterConcrete<SamplerPtr>, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterSampler(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<SamplerPtr>(effect, name)
		{
		}

	private:
		void DoFlush(SamplerPtr const & value);

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3D9RenderEffectParameterSampler(D3D9RenderEffectParameterSampler const & rhs);
		D3D9RenderEffectParameterSampler& operator=(D3D9RenderEffectParameterSampler const & rhs);
	};

	class D3D9RenderEffectParameterBoolArray : public RenderEffectParameterConcrete<std::vector<bool> >, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterBoolArray(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<std::vector<bool> >(effect, name)
		{
		}

	private:
		void DoFlush(std::vector<bool> const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterBoolArray(D3D9RenderEffectParameterBoolArray const & rhs);
		D3D9RenderEffectParameterBoolArray& operator=(D3D9RenderEffectParameterBoolArray const & rhs);
	};

	class D3D9RenderEffectParameterIntArray : public RenderEffectParameterConcrete<std::vector<int> >, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterIntArray(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<std::vector<int> >(effect, name)
		{
		}

	private:
		void DoFlush(std::vector<int> const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterIntArray(D3D9RenderEffectParameterIntArray const & rhs);
		D3D9RenderEffectParameterIntArray& operator=(D3D9RenderEffectParameterIntArray const & rhs);
	};

	class D3D9RenderEffectParameterFloatArray : public RenderEffectParameterConcrete<std::vector<float> >, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterFloatArray(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<std::vector<float> >(effect, name)
		{
		}

	private:
		void DoFlush(std::vector<float> const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterFloatArray(D3D9RenderEffectParameterFloatArray const & rhs);
		D3D9RenderEffectParameterFloatArray& operator=(D3D9RenderEffectParameterFloatArray const & rhs);
	};

	class D3D9RenderEffectParameterVector4Array : public RenderEffectParameterConcrete<std::vector<Vector4> >, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterVector4Array(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<std::vector<Vector4> >(effect, name)
		{
		}

	private:
		void DoFlush(std::vector<Vector4> const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterVector4Array(D3D9RenderEffectParameterVector4Array const & rhs);
		D3D9RenderEffectParameterVector4Array& operator=(D3D9RenderEffectParameterVector4Array const & rhs);
	};

	class D3D9RenderEffectParameterMatrix4Array : public RenderEffectParameterConcrete<std::vector<Matrix4> >, public D3D9Resource
	{
	public:
		D3D9RenderEffectParameterMatrix4Array(RenderEffect& effect, std::string const & name)
			: RenderEffectParameterConcrete<std::vector<Matrix4> >(effect, name)
		{
		}

	private:
		void DoFlush(std::vector<Matrix4> const & value);

	private:
		virtual void DoOnLostDevice()
		{
		}
		virtual void DoOnResetDevice()
		{
		}

	private:
		D3D9RenderEffectParameterMatrix4Array(D3D9RenderEffectParameterMatrix4Array const & rhs);
		D3D9RenderEffectParameterMatrix4Array& operator=(D3D9RenderEffectParameterMatrix4Array const & rhs);
	};
}

#endif		// _D3D9RENDEREFFECT_HPP
