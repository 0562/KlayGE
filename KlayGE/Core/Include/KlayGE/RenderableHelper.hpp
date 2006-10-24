// RenderableHelper.hpp
// KlayGE һЩ���õĿ���Ⱦ���� ͷ�ļ�
// Ver 2.7.1
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
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

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/Box.hpp>

namespace KlayGE
{
	class RenderableHelper : public Renderable
	{
	public:
		RenderableHelper(std::wstring const & name);
		virtual ~RenderableHelper()
		{
		}

		virtual RenderTechniquePtr GetRenderTechnique() const;
		virtual RenderLayoutPtr GetRenderLayout() const;

		virtual Box GetBound() const;

		virtual std::wstring const & Name() const;

	protected:
		std::wstring name_;

		Box box_;

		RenderLayoutPtr rl_;
		RenderTechniquePtr technique_;
	};

	class RenderablePoint : public RenderableHelper
	{
	public:
		RenderablePoint(float3 const & v, Color const & clr);
		virtual ~RenderablePoint()
		{
		}

		void OnRenderBegin();

	protected:
		float4 clr_;
	};

	class RenderableLine : public RenderableHelper
	{
	public:
		explicit RenderableLine(float3 const & v0, float3 const & v1, Color const & clr);
		virtual ~RenderableLine()
		{
		}

		void OnRenderBegin();

	protected:
		float4 clr_;
	};

	class RenderableTriangle : public RenderableHelper
	{
	public:
		RenderableTriangle(float3 const & v0, float3 const & v1, float3 const & v2, Color const & clr);
		virtual ~RenderableTriangle()
		{
		}

		void OnRenderBegin();

	protected:
		float4 clr_;
	};

	class RenderableBox : public RenderableHelper
	{
	public:
		explicit RenderableBox(Box const & box, Color const & clr);
		virtual ~RenderableBox()
		{
		}

		void OnRenderBegin();

	protected:
		float4 clr_;
	};

	class RenderableSkyBox : public RenderableHelper
	{
	public:
		RenderableSkyBox();
		virtual ~RenderableSkyBox()
		{
		}

		void CubeMap(TexturePtr const & cube);

		void OnRenderBegin();

	protected:
		TexturePtr cube_tex_;
	};

	class RenderablePlane : public RenderableHelper
	{
	public:
		RenderablePlane(float length, float width, int length_segs, int width_segs, bool has_tex_coord);
		virtual ~RenderablePlane()
		{
		}
	};
}

#endif		// _RENDERABLEHELPER_HPP
