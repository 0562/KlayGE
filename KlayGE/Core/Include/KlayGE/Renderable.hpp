// Renderable.hpp
// KlayGE ����Ⱦ������ ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
//
// 3.1.0
// ���SceneObject (2005.11.2)
//
// 2.7.0
// GetWorld����ΪGetModelMatrix (2005.6.17)
//
// 2.3.0
// ������Render (2005.1.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERABLE_HPP
#define _RENDERABLE_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>
#include <vector>

namespace KlayGE
{
	enum PassType
	{
		PT_OpaqueDepth,
		PT_TransparencyBackDepth,
		PT_TransparencyFrontDepth,
		PT_OpaqueGBufferRT0,
		PT_TransparencyBackGBufferRT0,
		PT_TransparencyFrontGBufferRT0,
		PT_OpaqueGBufferRT1,
		PT_TransparencyBackGBufferRT1,
		PT_TransparencyFrontGBufferRT1,
		PT_OpaqueGBufferMRT,
		PT_TransparencyBackGBufferMRT,
		PT_TransparencyFrontGBufferMRT,
		PT_GenShadowMap,
		PT_GenShadowMapWODepthTexture,
		PT_GenReflectiveShadowMap,
		PT_Lighting,
		PT_IndirectLighting,
		PT_OpaqueShading,
		PT_TransparencyBackShading,
		PT_TransparencyFrontShading,
		PT_OpaqueSpecialShading,
		PT_TransparencyBackSpecialShading,
		PT_TransparencyFrontSpecialShading,
		PT_SimpleForward
	};

	typedef std::vector<std::pair<std::string, std::string> > TextureSlotsType;
	struct KLAYGE_CORE_API RenderMaterial
	{
		float3 ambient;
		float3 diffuse;
		float3 specular;
		float3 emit;
		float opacity;
		float specular_level;
		float shininess;

		TextureSlotsType texture_slots;
	};

	// Abstract class defining the interface all renderable objects must implement.
	class KLAYGE_CORE_API Renderable : public enable_shared_from_this<Renderable>
	{
	public:
		enum EffectAttribute
		{
			EA_SpecialShading = 1UL << 0,
			EA_TransparencyBack = 1UL << 1,
			EA_TransparencyFront = 1UL << 2,
			EA_AlphaTest = 1UL << 3,
			EA_Reflection = 1UL << 4,
			EA_SimpleForward = 1UL << 5
		};

	public:
		Renderable();
		virtual ~Renderable();

		virtual RenderTechniquePtr const & GetRenderTechnique() const
		{
			return technique_;
		}
		virtual RenderLayoutPtr const & GetRenderLayout() const = 0;
		virtual std::wstring const & Name() const = 0;

		virtual void OnRenderBegin();
		virtual void OnRenderEnd();

		// These two functions are used for non-instancing rendering
		virtual void OnInstanceBegin(uint32_t id);
		virtual void OnInstanceEnd(uint32_t id);

		virtual AABBox const & PosBound() const = 0;
		virtual AABBox const & TexcoordBound() const = 0;

		virtual void AddToRenderQueue();

		virtual void Render();

		template <typename Iterator>
		void AssignInstances(Iterator begin, Iterator end)
		{
			instances_.resize(0);
			for (Iterator iter = begin; iter != end; ++ iter)
			{
				this->AddInstance(*iter);
			}
		}
		void AddInstance(SceneObjectPtr const & obj);

		uint32_t NumInstances() const
		{
			return static_cast<uint32_t>(instances_.size());
		}
		SceneObjectPtr GetInstance(uint32_t index) const
		{
			return instances_[index].lock();
		}

		// For deferred only

		virtual void ModelMatrix(float4x4 const & mat);

		virtual void Pass(PassType type);

		virtual bool SpecialShading() const
		{
			return effect_attrs_ & EA_SpecialShading ? true : false;
		}
		virtual bool TransparencyBackFace() const
		{
			return effect_attrs_ & EA_TransparencyBack ? true : false;
		}
		virtual bool TransparencyFrontFace() const
		{
			return effect_attrs_ & EA_TransparencyFront ? true : false;
		}		
		virtual bool AlphaTest() const
		{
			return effect_attrs_ & EA_AlphaTest ? true : false;
		}
		virtual bool Reflection() const
		{
			return effect_attrs_ & EA_Reflection ? true : false;
		}
		virtual bool SimpleForward() const
		{
			return effect_attrs_ & EA_SimpleForward ? true : false;
		}

	protected:
		virtual void UpdateInstanceStream();

		// For deferred only
		virtual void BindDeferredEffect(RenderEffectPtr const & deferred_effect);
		virtual RenderTechniquePtr const & PassTech(PassType type) const;

	protected:
		std::vector<weak_ptr<SceneObject> > instances_;

		RenderTechniquePtr technique_;

		// For deferred only

		RenderEffectPtr deferred_effect_;

		RenderTechniquePtr depth_tech_;
		RenderTechniquePtr depth_alpha_test_tech_;
		RenderTechniquePtr depth_alpha_blend_back_tech_;
		RenderTechniquePtr depth_alpha_blend_front_tech_;
		RenderTechniquePtr gbuffer_rt0_tech_;
		RenderTechniquePtr gbuffer_alpha_test_rt0_tech_;
		RenderTechniquePtr gbuffer_alpha_blend_back_rt0_tech_;
		RenderTechniquePtr gbuffer_alpha_blend_front_rt0_tech_;
		RenderTechniquePtr gbuffer_rt1_tech_;
		RenderTechniquePtr gbuffer_alpha_test_rt1_tech_;
		RenderTechniquePtr gbuffer_alpha_blend_back_rt1_tech_;
		RenderTechniquePtr gbuffer_alpha_blend_front_rt1_tech_;
		RenderTechniquePtr gbuffer_mrt_tech_;
		RenderTechniquePtr gbuffer_alpha_test_mrt_tech_;
		RenderTechniquePtr gbuffer_alpha_blend_back_mrt_tech_;
		RenderTechniquePtr gbuffer_alpha_blend_front_mrt_tech_;
		RenderTechniquePtr gen_sm_tech_;
		RenderTechniquePtr gen_sm_alpha_test_tech_;
		RenderTechniquePtr gen_sm_wo_dt_tech_;
		RenderTechniquePtr gen_sm_wo_dt_alpha_test_tech_;
		RenderTechniquePtr gen_rsm_tech_;
		RenderTechniquePtr gen_rsm_alpha_test_tech_;
		RenderTechniquePtr special_shading_tech_;
		RenderTechniquePtr special_shading_alpha_blend_back_tech_;
		RenderTechniquePtr special_shading_alpha_blend_front_tech_;

		float4x4 model_mat_;

		PassType type_;
		bool opacity_map_enabled_;
		uint32_t effect_attrs_;

		RenderMaterialPtr mtl_;

		RenderEffectParameterPtr mvp_param_;
		RenderEffectParameterPtr model_view_param_;
		RenderEffectParameterPtr pos_center_param_;
		RenderEffectParameterPtr pos_extent_param_;
		RenderEffectParameterPtr tc_center_param_;
		RenderEffectParameterPtr tc_extent_param_;
		RenderEffectParameterPtr shininess_param_;
		RenderEffectParameterPtr specular_tex_param_;
		RenderEffectParameterPtr normal_map_enabled_param_;
		RenderEffectParameterPtr normal_tex_param_;
		RenderEffectParameterPtr height_map_enabled_param_;
		RenderEffectParameterPtr height_tex_param_;
		RenderEffectParameterPtr diffuse_tex_param_;
		RenderEffectParameterPtr diffuse_clr_param_;
		RenderEffectParameterPtr emit_tex_param_;
		RenderEffectParameterPtr emit_clr_param_;
		RenderEffectParameterPtr specular_level_param_;
		RenderEffectParameterPtr opacity_clr_param_;
		RenderEffectParameterPtr opacity_map_enabled_param_;
		RenderEffectParameterPtr opaque_depth_tex_param_;

		TexturePtr diffuse_tex_;
		TexturePtr specular_tex_;
		TexturePtr normal_tex_;
		TexturePtr height_tex_;
		TexturePtr emit_tex_;
	};
}

#endif		//_RENDERABLE_HPP
