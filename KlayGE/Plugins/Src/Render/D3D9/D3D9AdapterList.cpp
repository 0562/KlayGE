#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <vector>
#include <boost/assert.hpp>

#include <KlayGE/D3D9/D3D9AdapterList.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9AdapterList::D3D9AdapterList()
						: currentAdapter_(0)
	{
	}

	// ��ȡϵͳ�Կ���Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t D3D9AdapterList::NumAdapter() const
	{
		return adapters_.size();
	}

	// ��ȡ�Կ�
	/////////////////////////////////////////////////////////////////////////////////
	D3D9Adapter const & D3D9AdapterList::Adapter(size_t index) const
	{
		BOOST_ASSERT(index < adapters_.size());

		return adapters_[index];
	}

	// ��ȡ��ǰ�Կ�����
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t D3D9AdapterList::CurrentAdapterIndex() const
	{
		return currentAdapter_;
	}

	// ���õ�ǰ�Կ�����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9AdapterList::CurrentAdapterIndex(uint32_t index)
	{
		currentAdapter_ = index;
	}

	// ö��ϵͳ�Կ�
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9AdapterList::Enumerate(boost::shared_ptr<IDirect3D9> const & d3d)
	{
		// ö��ϵͳ�е������� (ͨ��ֻ��һ���������м����Կ�)
		for (uint32_t i = 0; i < d3d->GetAdapterCount(); ++ i)
		{
			D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
			D3DDISPLAYMODE d3ddmDesktop;

			// �����������Ϣ
			d3d->GetAdapterIdentifier(i, 0, &d3dAdapterIdentifier);
			d3d->GetAdapterDisplayMode(i, &d3ddmDesktop);

			D3D9Adapter adapter(i, d3dAdapterIdentifier, d3ddmDesktop);
			adapter.Enumerate(d3d);

			// ���������Ч���豸��ʹ�ø�������
			if (adapter.NumVideoMode() != 0)
			{
				adapters_.push_back(adapter);
			}
		}

		// ���û���ҵ����ݵ��豸���׳�����
		if (adapters_.empty())
		{
			THR(E_FAIL);
		}
	}
}
