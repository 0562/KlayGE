#ifndef _D3D9ADAPTERINFO_HPP
#define _D3D9ADAPTERINFO_HPP

#include <d3d9.h>
#include <vector>
#include <KlayGE/D3D9/D3D9VideoMode.hpp>

#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")

namespace KlayGE
{
	// ��������������Ϣ��������������֧�ֵ��豸�б�
	/////////////////////////////////////////////////////////////////////////////////
	class D3D9Adapter
	{
	public:
		D3D9Adapter();
		D3D9Adapter(U32 adapterNo,
			const D3DADAPTER_IDENTIFIER9& d3dadapterIdentifer,
			const D3DDISPLAYMODE& d3ddmDesktop);

		void Enumerate(const COMPtr<IDirect3D9>& d3d);

		// �����豸�����ַ���
		const String Description() const;

		U32 AdapterNo() const
			{ return adapterNo_; }

		const D3DADAPTER_IDENTIFIER9& AdapterIdentifier() const
			{ return d3dAdapterIdentifier_; }

		const D3DDISPLAYMODE& DesktopMode() const
			{ return d3ddmDesktop_; }

		size_t VideoModeNum() const;
		const D3D9VideoMode& VideoMode(size_t index) const;

	private:
		// ���ǵڼ���������
		U32			adapterNo_;

		// ��������Ϣ
		D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier_;
		D3DDISPLAYMODE d3ddmDesktop_;		// �����������õ�����ģʽ

		// ��ʾģʽ�б�
		typedef std::vector<D3D9VideoMode, alloc<D3D9VideoMode> > ModeType;
		ModeType modes_;
	};
}

#endif			// _D3D9ADAPTERINFO_HPP