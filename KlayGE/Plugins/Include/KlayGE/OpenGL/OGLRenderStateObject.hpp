// OGLRenderStateObject.hpp
// KlayGE OpenGL��Ⱦ״̬������ ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ���ν��� (2008.7.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERSTATEOBJECT_HPP
#define _OGLRENDERSTATEOBJECT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderStateObject.hpp>

namespace KlayGE
{
	class OGLRasterizerStateObject : public RasterizerStateObject
	{
	public:
		explicit OGLRasterizerStateObject(RasterizerStateDesc const & desc);

		void SetStates(RasterizerStateObject const & current);
	};

	class OGLDepthStencilStateObject : public DepthStencilStateObject
	{
	public:
		explicit OGLDepthStencilStateObject(DepthStencilStateDesc const & desc);

		void SetStates(DepthStencilStateObject const & current);
	};

	class OGLBlendStateObject : public BlendStateObject
	{
	public:
		explicit OGLBlendStateObject(BlendStateDesc const & desc);

		void SetStates(BlendStateObject const & current);
	};
}

#endif			// _OGLRENDERSTATEOBJECT_HPP
