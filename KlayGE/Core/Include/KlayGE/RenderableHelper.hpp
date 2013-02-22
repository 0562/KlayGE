// RenderableHelper.hpp
// KlayGE һЩ���õĿ���Ⱦ���� ͷ�ļ�
// Ver 3.9.0
// ��Ȩ����(C) ������, 2005-2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// RenderableSkyBox��RenderableHDRSkyBox������Technique() (2010.1.4)
//
// 3.9.0
// ������RenderableHDRSkyBox (2009.5.4)
//
// 2.7.1
// ������RenderableHelper���� (2005.7.10)
//
// 2.6.0
// ������RenderableSkyBox (2005.5.26)
//
// 2.5.0
// ������RenderablePoint��RenderableLine��RenderableTriangle (2005.4.13)
//
// 2.4.0
// ���ν��� (2005.3.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERABLEHELPER_HPP
#define _RENDERABLEHELPER_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Renderable.hpp>
#include <KFL/AABBox.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API RenderableHelper : public Renderable
	{
	public:
		RenderableHelper(std::wstring const & name);
		virtual ~RenderableHelper()
		{
		}

		virtual RenderTechniquePtr const & GetRenderTechnique() const;
		virtual RenderLayoutPtr const & GetRenderLayout() const;

		virtual AABBox const & PosBound() const;
		virtual AABBox const & TexcoordBound() const;

		virtual std::wstring const & Name() const;

	protected:
		std::wstring name_;

		AABBox pos_aabb_;
		AABBox tc_aabb_;

		RenderLayoutPtr rl_;

		RenderEffectParameterPtr color_ep_;
		RenderEffectParameterPtr mvp_ep_;
	};

	class KLAYGE_CORE_API RenderablePoint : public RenderableHelper
	{
	public:
		RenderablePoint();
		RenderablePoint(float3 const & v, Color const & clr);
		virtual ~RenderablePoint()
		{
		}

		void SetPoint(float3 const & v);
		void SetColor(Color const & clr);

		void OnRenderBegin();

	private:
		void Init();

	private:
		RenderEffectParameterPtr v0_ep_;
	};

	class KLAYGE_CORE_API RenderableLine : public RenderableHelper
	{
	public:
		RenderableLine();
		RenderableLine(float3 const & v0, float3 const & v1, Color const & clr);
		virtual ~RenderableLine()
		{
		}

		void SetLine(float3 const & v0, float3 const & v1);
		void SetColor(Color const & clr);

		void OnRenderBegin();

	private:
		void Init();

	private:
		RenderEffectParameterPtr v0_ep_;
		RenderEffectParameterPtr v1_ep_;
	};

	class KLAYGE_CORE_API RenderableTriangle : public RenderableHelper
	{
	public:
		RenderableTriangle();
		RenderableTriangle(float3 const & v0, float3 const & v1, float3 const & v2, Color const & clr);
		virtual ~RenderableTriangle()
		{
		}

		void SetTriangle(float3 const & v0, float3 const & v1, float3 const & v2);
		void SetColor(Color const & clr);

		void OnRenderBegin();

	private:
		void Init();

	private:
		RenderEffectParameterPtr v0_ep_;
		RenderEffectParameterPtr v1_ep_;
		RenderEffectParameterPtr v2_ep_;
	};

	class KLAYGE_CORE_API RenderableTriBox : public RenderableHelper
	{
	public:
		RenderableTriBox();
		RenderableTriBox(OBBox const & obb, Color const & clr);
		virtual ~RenderableTriBox()
		{
		}

		void SetBox(OBBox const & obb);
		void SetColor(Color const & clr);

		void OnRenderBegin();

	private:
		void Init();

	private:
		RenderEffectParameterPtr v0_ep_;
		RenderEffectParameterPtr v1_ep_;
		RenderEffectParameterPtr v2_ep_;
		RenderEffectParameterPtr v3_ep_;
		RenderEffectParameterPtr v4_ep_;
		RenderEffectParameterPtr v5_ep_;
		RenderEffectParameterPtr v6_ep_;
		RenderEffectParameterPtr v7_ep_;
	};

	class KLAYGE_CORE_API RenderableLineBox : public RenderableHelper
	{
	public:
		RenderableLineBox();
		RenderableLineBox(OBBox const & obb, Color const & clr);
		virtual ~RenderableLineBox()
		{
		}

		void SetBox(OBBox const & obb);
		void SetColor(Color const & clr);

		void OnRenderBegin();

	private:
		void Init();

	private:
		RenderEffectParameterPtr v0_ep_;
		RenderEffectParameterPtr v1_ep_;
		RenderEffectParameterPtr v2_ep_;
		RenderEffectParameterPtr v3_ep_;
		RenderEffectParameterPtr v4_ep_;
		RenderEffectParameterPtr v5_ep_;
		RenderEffectParameterPtr v6_ep_;
		RenderEffectParameterPtr v7_ep_;
	};

	class KLAYGE_CORE_API RenderableSkyBox : public RenderableHelper
	{
	public:
		RenderableSkyBox();
		virtual ~RenderableSkyBox()
		{
		}

		virtual void Technique(RenderTechniquePtr const & tech);
		void CubeMap(TexturePtr const & cube);

		void OnRenderBegin();

		// For deferred only

		virtual void Pass(PassType type);

	protected:
		RenderEffectParameterPtr depth_far_ep_;
		RenderEffectParameterPtr inv_mvp_ep_;
		RenderEffectParameterPtr skybox_cube_tex_ep_;
	};

	class KLAYGE_CORE_API RenderableHDRSkyBox : public RenderableSkyBox
	{
	public:
		RenderableHDRSkyBox();
		virtual ~RenderableHDRSkyBox()
		{
		}

		virtual void Technique(RenderTechniquePtr const & tech);
		void CompressedCubeMap(TexturePtr const & y_cube, TexturePtr const & c_cube);

	protected:
		RenderEffectParameterPtr skybox_Ccube_tex_ep_;
	};

	class KLAYGE_CORE_API RenderablePlane : public RenderableHelper
	{
	public:
		RenderablePlane(float length, float width, int length_segs, int width_segs, bool has_tex_coord);
		virtual ~RenderablePlane()
		{
		}
	};

	class KLAYGE_CORE_API RenderDecal : public RenderableHelper
	{
	public:
		RenderDecal(TexturePtr const & normal_tex, TexturePtr const & diffuse_tex, float3 const & diffuse_clr,
				TexturePtr const & specular_tex, float3 const & specular_level, float shininess);

		void OnRenderBegin();

	private:
		RenderEffectParameterPtr inv_mv_ep_;
		float3 diffuse_clr_;
		float specular_level_;
		float shininess_;
	};
}

#endif		// _RENDERABLEHELPER_HPP
