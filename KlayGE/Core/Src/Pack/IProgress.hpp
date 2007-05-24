// IProgress.hpp
// KlayGE ���ϵͳ���Ƚӿ� ͷ�ļ� ����7zip
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _IPROGRESS_HPP
#define _IPROGRESS_HPP

#include "BaseDefines.hpp"

// MIDL_INTERFACE("23170F69-40C1-278A-0000-000000050000")
DEFINE_GUID(IID_IProgress, 
	0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00);
struct IProgress : public IUnknown
{
	STDMETHOD(SetTotal)(KlayGE::uint64_t total) PURE;
	STDMETHOD(SetCompleted)(const KlayGE::uint64_t* completeValue) PURE;
};

#endif		// _IPROGRESS_HPP
