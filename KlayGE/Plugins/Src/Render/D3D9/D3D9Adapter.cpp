#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/COMPtr.hpp>

#include <cassert>
#include <algorithm>

#include <KlayGE/D3D9/D3D9Adapter.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9Adapter::D3D9Adapter()
					: adapterNo_(0)
	{
		MemoryLib::Zero(&d3dAdapterIdentifier_, sizeof(d3dAdapterIdentifier_));
		MemoryLib::Zero(&d3ddmDesktop_, sizeof(d3ddmDesktop_));
	}

	D3D9Adapter::D3D9Adapter(U32 adapterNo,
							   const D3DADAPTER_IDENTIFIER9& d3dadapterIdentifer,
							   const D3DDISPLAYMODE& d3ddmDesktop)
					: adapterNo_(adapterNo),
						d3dAdapterIdentifier_(d3dadapterIdentifer),
						d3ddmDesktop_(d3ddmDesktop)
	{
	}

	// ��ȡ�豸�����ַ���
	/////////////////////////////////////////////////////////////////////////////////
	const String D3D9Adapter::Description() const
	{
		return String(this->AdapterIdentifier().Description);
	}

	// ��ȡ֧�ֵ���ʾģʽ��Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t D3D9Adapter::NumVideoMode() const
	{
		return modes_.size();
	}

	// ��ȡ��ʾģʽ
	/////////////////////////////////////////////////////////////////////////////////
	const D3D9VideoMode& D3D9Adapter::VideoMode(size_t index) const
	{
		assert(index < modes_.size());

		return modes_[index];
	}

	// ö����ʾģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Adapter::Enumerate(const COMPtr<IDirect3D9>& d3d)
	{
		using std::vector;

		typedef vector<D3DFORMAT, alloc<D3DFORMAT> > FormatType;
		FormatType formats;
		formats.push_back(D3DFMT_X8R8G8B8);
		formats.push_back(D3DFMT_A8R8G8B8);
		formats.push_back(D3DFMT_A2R10G10B10);
		formats.push_back(D3DFMT_X1R5G5B5);
		formats.push_back(D3DFMT_A1R5G5B5);
		formats.push_back(D3DFMT_R5G6B5);

		for (FormatType::iterator iter = formats.begin(); iter != formats.end(); ++ iter)
		{
			const U32 modeCount(d3d->GetAdapterModeCount(adapterNo_, *iter));
			for (U32 i = 0; i < modeCount; ++ i)
			{
				// ��ȡ��ʾģʽ����
				D3DDISPLAYMODE d3dDisplayMode;
				d3d->EnumAdapterModes(adapterNo_, *iter, i, &d3dDisplayMode);

				// ���˳��ͷֱ���ģʽ
				if ((d3dDisplayMode.Width < 640) || (d3dDisplayMode.Height < 400))
				{
					continue;
				}

				// ����ˢ���ʵĲ�ͬ
				const D3D9VideoMode videoMode(d3dDisplayMode.Width, d3dDisplayMode.Height,
						d3dDisplayMode.Format);

				// ����ҵ�һ����ģʽ, ����ģʽ�б�
				if (std::find(modes_.begin(), modes_.end(), videoMode) == modes_.end())
				{
					modes_.push_back(videoMode);
				}
			}
		}

		std::sort(modes_.begin(), modes_.end());
	}
}
