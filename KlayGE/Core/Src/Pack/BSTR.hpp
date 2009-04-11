// BSTR.hpp
// KlayGE ���ϵͳBSTR ͷ�ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _BSTR_HPP
#define _BSTR_HPP

#pragma KLAYGE_ONCE

#include <string>

typedef wchar_t* BSTR;

namespace KlayGE
{
	BSTR AllocBSTR(std::wstring const & sz);
	void FreeBSTR(BSTR bstr);
}

#endif      // _BSTR_HPP
