// Util.hpp
// KlayGE ʵ�ú����� ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <string>
#include <KlayGE/KlayGE.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ���n bitΪ1
	inline U32
	SetMask(U32 n)
		{ return 1UL << n; }
	template <U32 n>
	struct Mask
	{
		enum { value = 1UL << n };
	};

	// ȡ���еĵ� n bit
	inline U32
	GetBit(U32 x, U32 n)
		{ return (x >> n) & 1; }
	// �����еĵ� n bitΪ1
	inline U32
	SetBit(U32 x, U32 n)
		{ return x & SetMask(n); }

	// ȡ���ֽ�
	inline U16
	LO_U8(U16 x)
		{ return x & 0xFF; }
	// ȡ���ֽ�
	inline U16
	HI_U8(U16 x)
		{ return (x & 0xFF) >> 8; }

	// ȡ����
	inline U32
	LO_U16(U32 x)
		{ return x & 0xFFFF; }
	// ȡ����
	inline U32
	HI_U16(U32 x)
		{ return (x & 0xFFFF) >> 16; }

	// �ߵ��ֽڽ���
	inline U16
	HI_LO_SwapU8(U16 x)
		{ return (LO_U8(x) << 8) | HI_U8(x); }
	// �ߵ��ֽ���
	inline U32
	HI_LO_SwapU16(U32 x)
		{ return (LO_U16(x) << 16) | HI_U16(x); }

	// ���ĳһλ��1������
	inline U32
	MakeMask(U32 dw)
		{ return (1UL << (dw + 1)) - 1; }

	// UNICODE����, ����String, WString֮���ת��
	/////////////////////////////////////////////////////////////////////////////////
	String& Convert(String& strDest, const String& strSrc);
	String& Convert(String& strDest, const WString& wstrSrc);
	WString& Convert(WString& wstrDest, const String& strSrc);
	WString& Convert(WString& wstrDest, const WString& wstrSrc);

	// ��ͣ������
	/////////////////////////////////////////////////////////////////////////////////
	void Sleep(U32 ms);
}

#endif		// _UTIL_HPP