// Util.hpp
// KlayGE ʵ�ú����� ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ������copy_if (2006.2.23)
//
// 3.0.0
// ������checked_cast (2005.9.26)
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

#include <boost/assert.hpp>
#include <boost/smart_ptr.hpp>
#define BOOST_MEM_FN_ENABLE_STDCALL
#include <boost/mem_fn.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// ���n bitΪ1
	inline uint32_t
	SetMask(uint32_t n)
		{ return 1UL << n; }
	template <uint32_t n>
	struct Mask
	{
		enum { value = 1UL << n };
	};

	// ȡ���еĵ� n bit
	inline uint32_t
	GetBit(uint32_t x, uint32_t n)
		{ return (x >> n) & 1; }
	// �����еĵ� n bitΪ1
	inline uint32_t
	SetBit(uint32_t x, uint32_t n)
		{ return x & SetMask(n); }

	// ȡ���ֽ�
	inline uint16_t
	LO_U8(uint16_t x)
		{ return x & 0xFF; }
	// ȡ���ֽ�
	inline uint16_t
	HI_U8(uint16_t x)
		{ return (x & 0xFF) >> 8; }

	// ȡ����
	inline uint32_t
	LO_U16(uint32_t x)
		{ return x & 0xFFFF; }
	// ȡ����
	inline uint32_t
	HI_U16(uint32_t x)
		{ return (x & 0xFFFF) >> 16; }

	// �ߵ��ֽڽ���
	inline uint16_t
	HI_LO_SwapU8(uint16_t x)
		{ return (LO_U8(x) << 8) | HI_U8(x); }
	// �ߵ��ֽ���
	inline uint32_t
	HI_LO_SwapU16(uint32_t x)
		{ return (LO_U16(x) << 16) | HI_U16(x); }

	// ���ĳһλ��1������
	inline uint32_t
	MakeMask(uint32_t dw)
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
	void Sleep(uint32_t ms);

	// �����ʽ�ͱ��ظ�ʽ֮��ת��
	uint32_t ToNet(uint32_t host);
	uint16_t ToNet(uint16_t host);
	uint32_t ToHost(uint32_t network);
	uint16_t ToHost(uint16_t network);

	// �õ�COM���������ָ��
	template <typename T>
	inline boost::shared_ptr<T>
	MakeCOMPtr(T* p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}

	template <typename To, typename From>
	inline To
	checked_cast(From* p)
	{
		BOOST_ASSERT(dynamic_cast<To>(p) == static_cast<To>(p));
		return static_cast<To>(p);
	}

	uint32_t LastError();

#ifdef _SELECT1ST2ND_SUPPORT
	template <typename pair_type>
	struct select1st : public std::unary_function<pair_type const &, typename pair_type::first_type const &>
	{
		const typename pair_type::first_type& operator()(pair_type const & x) const
		{
			return x.first;
		}
	};

	template <typename pair_type>
	struct select2nd : public std::unary_function<pair_type const &, typename pair_type::second_type const &>
	{
		const typename pair_type::second_type& operator()(pair_type const & x) const
		{
			return x.second;
		}
	};
#else
	using std::select1st;
	using std::select2nd;
#endif		// _SELECT1ST2ND_SUPPORT

#ifdef _COPYIF_SUPPORT
	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator copy_if(InputIterator first, InputIterator last,
							OutputIterator dest_first,
							Predicate p)
	{
		for (InputIterator iter = first; iter != last; ++ iter)
		{
			if (p(*iter))
			{
				*dest_first = *iter;
				++ dest_first;
			}
		}

		return dest_first;
	}
#else
	using std::copyif;
#endif
}

#endif		// _UTIL_HPP
