// D3D9RenderFactory.hpp
// KlayGE D3D9��Ⱦ������󹤳� ͷ�ļ�
// Ver 2.4.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.4.0
// ������resource_pool_��Ա (2005.3.3)
//
// 2.3.0
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// 2.0.3
// ��Ϊtemplateʵ�� (2004.3.4)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERFACTORY_HPP
#define _D3D9RENDERFACTORY_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_D3D9
#include <KlayGE/config/auto_link.hpp>

namespace KlayGE
{
	RenderFactoryPtr const & D3D9RenderFactoryInstance();
}

#endif			// _D3D9RENDERFACTORY_HPP
