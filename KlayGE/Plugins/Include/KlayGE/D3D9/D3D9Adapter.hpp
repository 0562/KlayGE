#ifndef _D3D9ADAPTERINFO_HPP
#define _D3D9ADAPTERINFO_HPP

#include <d3d9.h>
#include <vector>
#include <string>
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
		D3D9Adapter(uint32_t adapterNo,
			D3DADAPTER_IDENTIFIER9 const & d3dadapterIdentifer,
			D3DDISPLAYMODE const & d3ddmDesktop);

		void Enumerate(boost::shared_ptr<IDirect3D9> const & d3d);

		// �����豸�����ַ���
		std::string const Description() const;

		uint32_t AdapterNo() const
			{ return adapterNo_; }

		D3DADAPTER_IDENTIFIER9 const & AdapterIdentifier() const
			{ return d3dAdapterIdentifier_; }

		D3DDISPLAYMODE const & DesktopMode() const
			{ return d3ddmDesktop_; }

		size_t NumVideoMode() const;
		D3D9VideoMode const & VideoMode(size_t index) const;

	private:
		// ���ǵڼ���������
		uint32_t			adapterNo_;

		// ��������Ϣ
		D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier_;
		D3DDISPLAYMODE d3ddmDesktop_;		// �����������õ�����ģʽ

		// ��ʾģʽ�б�
		typedef std::vector<D3D9VideoMode> ModeType;
		ModeType modes_;
	};
}

#endif			// _D3D9ADAPTERINFO_HPP