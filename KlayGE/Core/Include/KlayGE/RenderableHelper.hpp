// RenderableHelper.hpp
// KlayGE һЩ���õĿ���Ⱦ���� ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
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
	class RenderableBox : public Renderable
	{
	public:
		RenderableBox(Box const & box);

		RenderEffectPtr GetRenderEffect() const;
		VertexBufferPtr GetVertexBuffer() const;
		std::wstring const & Name() const;

		Box GetBound() const;

	private:
		Box box_;

		VertexBufferPtr vb_;
		RenderEffectPtr effect_;
	};
}

#endif		//_RENDERABLEHELPER_HPP
