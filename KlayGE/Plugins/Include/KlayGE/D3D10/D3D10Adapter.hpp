// D3D10Adapter.hpp
// KlayGE D3D10������ ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://www.klayge.org
//
// 3.8.0
// ���ν��� (2008.9.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D10ADAPTER_HPP
#define _D3D10ADAPTER_HPP

#pragma once

#include <KlayGE/D3D10/D3D10MinGWDefs.hpp>
#include <d3d10.h>
#include <vector>
#include <string>
#include <KlayGE/D3D10/D3D10Typedefs.hpp>
#include <KlayGE/D3D10/D3D10VideoMode.hpp>

namespace KlayGE
{
	// ��������������Ϣ��������������֧�ֵ��豸�б�
	/////////////////////////////////////////////////////////////////////////////////
	class D3D10Adapter
	{
	public:
		D3D10Adapter();
		D3D10Adapter(uint32_t adapter_no, IDXGIAdapterPtr const & adapter);

		void Enumerate();

		// �����豸�����ַ���
		std::wstring const Description() const;
		void ResetAdapter(IDXGIAdapterPtr const & ada);

		uint32_t AdapterNo() const
		{
			return adapter_no_;
		}

		IDXGIAdapterPtr const & Adapter() const
		{
			return adapter_;
		}

		DXGI_FORMAT DesktopFormat() const
		{
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		size_t NumVideoMode() const;
		D3D10VideoMode const & VideoMode(size_t index) const;

	private:
		// ���ǵڼ���������
		uint32_t			adapter_no_;

		// ��������Ϣ
		IDXGIAdapterPtr		adapter_;
		DXGI_ADAPTER_DESC	adapter_desc_;

		// ��ʾģʽ�б�
		typedef std::vector<D3D10VideoMode> ModeType;
		ModeType modes_;
	};

	typedef boost::shared_ptr<D3D10Adapter> D3D10AdapterPtr;
}

#endif			// _D3D10ADAPTER_HPP
