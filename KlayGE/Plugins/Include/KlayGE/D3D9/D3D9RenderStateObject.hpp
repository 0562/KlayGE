// D3D9RenderStateObject.hpp
// KlayGE ��Ⱦ״̬������ ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ���ν��� (2008.7.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERSTATEOBJECT_HPP
#define _D3D9RENDERSTATEOBJECT_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderStateObject.hpp>

namespace KlayGE
{
	class D3D9RasterizerStateObject : public RasterizerStateObject
	{
	public:
		explicit D3D9RasterizerStateObject(RasterizerStateDesc const & desc);

		void Active();
	};

	class D3D9DepthStencilStateObject : public DepthStencilStateObject
	{
	public:
		explicit D3D9DepthStencilStateObject(DepthStencilStateDesc const & desc);

		void Active();
	};

	class D3D9BlendStateObject : public BlendStateObject
	{
	public:
		explicit D3D9BlendStateObject(BlendStateDesc const & desc);

		void Active();
	};
}

#endif			// _D3D9RENDERSTATEOBJECT_HPP
