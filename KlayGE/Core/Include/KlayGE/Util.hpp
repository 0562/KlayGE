// Util.hpp
// KlayGE ʵ�ú����� ͷ�ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// �����˱��غ������ʽ��ת������ (2004.6.2)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <string>

#include <boost/smart_ptr.hpp>
#define BOOST_MEM_FN_ENABLE_STDCALL
#include <boost/mem_fn.hpp>

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

	// ����FourCC����
	template <char ch0, char ch1, char ch2, char ch3>
	struct MakeFourCC
	{
		enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) };
	};

	// Unicode����, ����string, wstring֮���ת��
	std::string& Convert(std::string& strDest, std::string const & strSrc);
	std::string& Convert(std::string& strDest, std::wstring const & wstrSrc);
	std::wstring& Convert(std::wstring& wstrDest, std::string const & strSrc);
	std::wstring& Convert(std::wstring& wstrDest, std::wstring const & wstrSrc);

	// ��ͣ������
	void Sleep(U32 ms);

	// �����ʽ�ͱ��ظ�ʽ֮��ת��
	U32 ToNet(U32 host);
	U16 ToNet(U16 host);
	U32 ToHost(U32 network);
	U16 ToHost(U16 network);

	// �õ�COM���������ָ��
	template <typename T>
	inline boost::shared_ptr<T>
	MakeCOMPtr(T* p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}
}

#endif		// _UTIL_HPP
