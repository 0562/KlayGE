// D3D10AdapterList.hpp
// KlayGE D3D10�������б� ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2008.9.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D10ADAPTERLIST_HPP
#define _D3D10ADAPTERLIST_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <boost/smart_ptr.hpp>
#include <KlayGE/D3D10/D3D10Adapter.hpp>

#include <KlayGE/D3D10/D3D10MinGWDefs.hpp>
#include <d3d10.h>

namespace KlayGE
{
	class D3D10AdapterList
	{
	public:
		D3D10AdapterList();

		void Enumerate(IDXGIFactoryPtr const & gi_factory);

		size_t NumAdapter() const;
		D3D10AdapterPtr const & Adapter(size_t index) const;

		uint32_t CurrentAdapterIndex() const;
		void CurrentAdapterIndex(uint32_t index);

	private:
		std::vector<D3D10AdapterPtr> adapters_;
		uint32_t			current_adapter_;
	};

	typedef boost::shared_ptr<D3D10AdapterList> D3D10AdapterListPtr;
}

#endif			// _D3D10ADAPTERLIST_HPP
