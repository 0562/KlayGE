// RenderableHelper.hpp
// KlayGE һЩ���õĿ���Ⱦ���� ͷ�ļ�
// Ver 3.9.0
// ��Ȩ����(C) ������, 2005-2009
// Homepage: http://klayge.sourceforge.net
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
#include <KlayGE/Box.hpp>

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

		virtual Box const & GetBound() const;

		virtual std::wstring const & Name() const;

	protected:
		std::wstring name_;

		Box box_;

		RenderLayoutPtr rl_;
		RenderTechniquePtr technique_;

		RenderEffectParameterPtr color_ep_;
		RenderEffectParameterPtr matViewProj_ep_;
	};

	class KLAYGE_CORE_API RenderablePoint : public RenderableHelper
	{
	public:
		RenderablePoint(float3 const & v, Color const & clr);
		virtual ~RenderablePoint()
		{
		}

		void OnRenderBegin();
	};

	class KLAYGE_CORE_API RenderableLine : public RenderableHelper
	{
	public:
		explicit RenderableLine(float3 const & v0, float3 const & v1, Color const & clr);
		virtual ~RenderableLine()
		{
		}

		void OnRenderBegin();
	};

	class KLAYGE_CORE_API RenderableTriangle : public RenderableHelper
	{
	public:
		RenderableTriangle(float3 const & v0, float3 const & v1, float3 const & v2, Color const & clr);
		virtual ~RenderableTriangle()
		{
		}

		void OnRenderBegin();
	};

	class KLAYGE_CORE_API RenderableTriBox : public RenderableHelper
	{
	public:
		explicit RenderableTriBox(Box const & box, Color const & clr);
		virtual ~RenderableTriBox()
		{
		}

		void OnRenderBegin();
	};

	class KLAYGE_CORE_API RenderableLineBox : public RenderableHelper
	{
	public:
		explicit RenderableLineBox(Box const & box, Color const & clr);
		virtual ~RenderableLineBox()
		{
		}

		void OnRenderBegin();
	};

	class KLAYGE_CORE_API RenderableSkyBox : public RenderableHelper
	{
	public:
		RenderableSkyBox();
		virtual ~RenderableSkyBox()
		{
		}

		void CubeMap(TexturePtr const & cube);

		void OnRenderBegin();

	protected:
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
}

#endif		// _RENDERABLEHELPER_HPP
