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
#include <functional>

#include <boost/smart_ptr.hpp>
#define BOOST_MEM_FN_ENABLE_STDCALL
#include <boost/mem_fn.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ���n bitΪ1
	inline uint32
	SetMask(uint32 n)
		{ return 1UL << n; }
	template <uint32 n>
	struct Mask
	{
		enum { value = 1UL << n };
	};

	// ȡ���еĵ� n bit
	inline uint32
	GetBit(uint32 x, uint32 n)
		{ return (x >> n) & 1; }
	// �����еĵ� n bitΪ1
	inline uint32
	SetBit(uint32 x, uint32 n)
		{ return x & SetMask(n); }

	// ȡ���ֽ�
	inline uint16
	LO_U8(uint16 x)
		{ return x & 0xFF; }
	// ȡ���ֽ�
	inline uint16
	HI_U8(uint16 x)
		{ return (x & 0xFF) >> 8; }

	// ȡ����
	inline uint32
	LO_U16(uint32 x)
		{ return x & 0xFFFF; }
	// ȡ����
	inline uint32
	HI_U16(uint32 x)
		{ return (x & 0xFFFF) >> 16; }

	// �ߵ��ֽڽ���
	inline uint16
	HI_LO_SwapU8(uint16 x)
		{ return (LO_U8(x) << 8) | HI_U8(x); }
	// �ߵ��ֽ���
	inline uint32
	HI_LO_SwapU16(uint32 x)
		{ return (LO_U16(x) << 16) | HI_U16(x); }

	// ���ĳһλ��1������
	inline uint32
	MakeMask(uint32 dw)
		{ return (1UL << (dw + 1)) - 1; }

	// ����FourCC����
	template <unsigned char ch0, unsigned char ch1, unsigned char ch2, unsigned char ch3>
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
	void Sleep(uint32 ms);

	// �����ʽ�ͱ��ظ�ʽ֮��ת��
	uint32 ToNet(uint32 host);
	uint16 ToNet(uint16 host);
	uint32 ToHost(uint32 network);
	uint16 ToHost(uint16 network);

	// �õ�COM���������ָ��
	template <typename T>
	inline boost::shared_ptr<T>
	MakeCOMPtr(T* p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}

#ifndef _SELECT1ST2ND_SUPPORT
	template <typename Pair>
	struct select1st : public std::unary_function<Pair, typename Pair::first_type>
	{
		const typename Pair::first_type& operator()(const Pair& x) const
		{
			return x.first;
		}
	};

	template <typename Pair>
	struct select2nd : public std::unary_function<Pair, typename Pair::second_type>
	{
		const typename Pair::second_type& operator()(const Pair& x) const
		{
			return x.second;
		}
	};
#endif		// _SELECT1ST2ND_SUPPORT
}

#endif		// _UTIL_HPP
