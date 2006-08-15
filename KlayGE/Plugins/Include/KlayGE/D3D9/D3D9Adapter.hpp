#ifndef _D3D9ADAPTERINFO_HPP
#define _D3D9ADAPTERINFO_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_D3D9
#include <KlayGE/config/auto_link.hpp>

#include <d3d9.h>
#include <vector>
#include <string>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9VideoMode.hpp>

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

		void Enumerate(ID3D9Ptr const & d3d);

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