// RenderableHelper.hpp
// KlayGE һЩ���õĿ���Ⱦ���� ͷ�ļ�
// Ver 2.5.1
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.1
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
	class RenderablePoint : public Renderable
	{
	public:
		explicit RenderablePoint(Vector3 const & v);

		RenderEffectPtr GetRenderEffect() const;
		VertexBufferPtr GetVertexBuffer() const;
		std::wstring const & Name() const;

		Box GetBound() const;

	private:
		Box box_;

		VertexBufferPtr vb_;
		RenderEffectPtr effect_;
	};

	class RenderableLine : public Renderable
	{
	public:
		explicit RenderableLine(Vector3 const & v0, Vector3 const & v1);

		RenderEffectPtr GetRenderEffect() const;
		VertexBufferPtr GetVertexBuffer() const;
		std::wstring const & Name() const;

		Box GetBound() const;

	private:
		Box box_;

		VertexBufferPtr vb_;
		RenderEffectPtr effect_;
	};

	class RenderableTriangle : public Renderable
	{
	public:
		RenderableTriangle(Vector3 const & v0, Vector3 const & v1, Vector3 const & v2);

		RenderEffectPtr GetRenderEffect() const;
		VertexBufferPtr GetVertexBuffer() const;
		std::wstring const & Name() const;

		Box GetBound() const;

	private:
		Box box_;

		VertexBufferPtr vb_;
		RenderEffectPtr effect_;
	};

	class RenderableBox : public Renderable
	{
	public:
		explicit RenderableBox(Box const & box);

		RenderEffectPtr GetRenderEffect() const;
		VertexBufferPtr GetVertexBuffer() const;
		std::wstring const & Name() const;

		Box GetBound() const;

	private:
		Box box_;

		VertexBufferPtr vb_;
		RenderEffectPtr effect_;
	};

	class RenderableSkyBox : public Renderable
	{
	public:
		RenderableSkyBox();

		void CubeMap(TexturePtr const & cube);
		void MVPMatrix(Matrix4 const & mvp);

		void OnRenderBegin();
		bool CanBeCulled() const;

		RenderEffectPtr GetRenderEffect() const;
		VertexBufferPtr GetVertexBuffer() const;

		Box GetBound() const;

		std::wstring const & Name() const;

	private:
		Box box_;

		VertexBufferPtr vb_;
		RenderEffectPtr effect_;

		Matrix4 inv_mvp_;
	};
}

#endif		//_RENDERABLEHELPER_HPP
