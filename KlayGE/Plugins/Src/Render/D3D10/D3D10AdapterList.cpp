// D3D10AdapterList.cpp
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

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/COMPtr.hpp>

#include <vector>
#include <boost/assert.hpp>

#include <KlayGE/D3D10/D3D10AdapterList.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D10AdapterList::D3D10AdapterList()
						: current_adapter_(0)
	{
	}

	// ��ȡϵͳ�Կ���Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t D3D10AdapterList::NumAdapter() const
	{
		return adapters_.size();
	}

	// ��ȡ�Կ�
	/////////////////////////////////////////////////////////////////////////////////
	D3D10AdapterPtr const & D3D10AdapterList::Adapter(size_t index) const
	{
		BOOST_ASSERT(index < adapters_.size());

		return adapters_[index];
	}

	// ��ȡ��ǰ�Կ�����
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t D3D10AdapterList::CurrentAdapterIndex() const
	{
		return current_adapter_;
	}

	// ���õ�ǰ�Կ�����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D10AdapterList::CurrentAdapterIndex(uint32_t index)
	{
		current_adapter_ = index;
	}

	// ö��ϵͳ�Կ�
	/////////////////////////////////////////////////////////////////////////////////
	void D3D10AdapterList::Enumerate(IDXGIFactoryPtr const & gi_factory)
	{
		// ö��ϵͳ�е�������
		UINT adapter_no = 0;
		IDXGIAdapter* dxgi_adapter = NULL;
		while (gi_factory->EnumAdapters(adapter_no, &dxgi_adapter) != DXGI_ERROR_NOT_FOUND)
		{
			if (dxgi_adapter != NULL)
			{
				DXGI_ADAPTER_DESC ad;
				dxgi_adapter->GetDesc(&ad);

				if (0 == wcscmp(ad.Description, L"NVIDIA PerfHUD"))
				{
					current_adapter_ = adapter_no;
				}

				D3D10AdapterPtr adapter(new D3D10Adapter(adapter_no, MakeCOMPtr(dxgi_adapter)));
				adapters_.push_back(adapter);
			}

			++ adapter_no;
		}

		// ���û���ҵ����ݵ��豸���׳�����
		if (adapters_.empty())
		{
			THR(boost::system::posix_error::not_supported);
		}
	}
}
