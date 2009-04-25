// D3D9RenderLayout.hpp
// KlayGE D3D9��Ⱦ������ ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.1.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERLAYOUT_HPP
#define _D3D9RENDERLAYOUT_HPP

#pragma once

#include <boost/smart_ptr.hpp>

#include <vector>
#include <d3d9.h>

#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

namespace KlayGE
{
	class D3D9RenderLayout : public RenderLayout, public D3D9Resource
	{
	public:
		D3D9RenderLayout();
		~D3D9RenderLayout();
		
		ID3D9VertexDeclarationPtr VertexDeclaration() const;

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		typedef std::vector<D3DVERTEXELEMENT9> vertex_elems_type;
		vertex_elems_type vertex_elems_;

		mutable bool dirty_decl_;
		mutable ID3D9VertexDeclarationPtr decl_;
	};
}

#endif			// _D3D9RENDERLAYOUT_HPP
