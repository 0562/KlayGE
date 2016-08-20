// D3D11RenderLayout.hpp
// KlayGE D3D11��Ⱦ������ ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2009
// Homepage: http://www.klayge.org
//
// 3.8.0
// ���ν��� (2009.1.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11RENDERLAYOUT_HPP
#define _D3D11RENDERLAYOUT_HPP

#pragma once

#include <vector>

#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/D3D11/D3D11Typedefs.hpp>

namespace KlayGE
{
	class D3D11RenderLayout : public RenderLayout
	{
	public:
		D3D11RenderLayout();

		void Active(ShaderObject const * so) const;

		std::vector<ID3D11Buffer*> const & VBs() const
		{
			return vbs_;
		}
		std::vector<UINT> const & Strides() const
		{
			return strides_;
		}
		std::vector<UINT> const & Offsets() const
		{
			return offsets_;
		}
		ID3D11InputLayout* InputLayout() const
		{
			return layout_.get();
		}

	private:
		mutable std::vector<ID3D11Buffer*> vbs_;
		mutable std::vector<UINT> strides_;
		mutable std::vector<UINT> offsets_;
		mutable ID3D11InputLayoutPtr layout_;
	};
}

#endif			// _D3D11RENDERLAYOUT_HPP
