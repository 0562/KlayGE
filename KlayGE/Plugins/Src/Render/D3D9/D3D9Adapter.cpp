#include <KlayGE/KlayGE.hpp>

#include <boost/smart_ptr.hpp>

#include <cassert>
#include <algorithm>
#include <cstring>

#include <KlayGE/D3D9/D3D9Adapter.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9Adapter::D3D9Adapter()
					: adapterNo_(0)
	{
		std::memset(&d3dAdapterIdentifier_, 0, sizeof(d3dAdapterIdentifier_));
		std::memset(&d3ddmDesktop_, 0, sizeof(d3ddmDesktop_));
	}

	D3D9Adapter::D3D9Adapter(uint32 adapterNo,
							   D3DADAPTER_IDENTIFIER9 const & d3dadapterIdentifer,
							   D3DDISPLAYMODE const & d3ddmDesktop)
					: adapterNo_(adapterNo),
						d3dAdapterIdentifier_(d3dadapterIdentifer),
						d3ddmDesktop_(d3ddmDesktop)
	{
	}

	// ��ȡ�豸�����ַ���
	/////////////////////////////////////////////////////////////////////////////////
	std::string const D3D9Adapter::Description() const
	{
		return std::string(this->AdapterIdentifier().Description);
	}

	// ��ȡ֧�ֵ���ʾģʽ��Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t D3D9Adapter::NumVideoMode() const
	{
		return modes_.size();
	}

	// ��ȡ��ʾģʽ
	/////////////////////////////////////////////////////////////////////////////////
	D3D9VideoMode const & D3D9Adapter::VideoMode(size_t index) const
	{
		assert(index < modes_.size());

		return modes_[index];
	}

	// ö����ʾģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Adapter::Enumerate(boost::shared_ptr<IDirect3D9> const & d3d)
	{
		using std::vector;

		typedef vector<D3DFORMAT> FormatType;
		FormatType formats;
		formats.push_back(D3DFMT_X8R8G8B8);
		formats.push_back(D3DFMT_A8R8G8B8);
		formats.push_back(D3DFMT_A2R10G10B10);
		formats.push_back(D3DFMT_X1R5G5B5);
		formats.push_back(D3DFMT_A1R5G5B5);
		formats.push_back(D3DFMT_R5G6B5);

		for (FormatType::iterator iter = formats.begin(); iter != formats.end(); ++ iter)
		{
			uint32 const modeCount(d3d->GetAdapterModeCount(adapterNo_, *iter));
			for (uint32 i = 0; i < modeCount; ++ i)
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
				D3D9VideoMode const videoMode(d3dDisplayMode.Width, d3dDisplayMode.Height,
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
