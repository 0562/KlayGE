// Types.hpp
// KlayGE ���Ͷ���ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ��boost�����Ͷ������ֱ�Ӷ��� (2004.10.30)
//
// 1.3.8.3
// ���ν���
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TYPES_HPP
#define _TYPES_HPP

#include <boost/cstdint.hpp>

namespace KlayGE
{
#ifdef _MSC_VER
	#ifndef _WCHAR_T_DEFINED
		typedef unsigned short		wchar_t;
		#define _WCHAR_T_DEFINED
	#endif		// _WCHAR_T_DEFINED
#endif

	typedef boost::uint64_t		uint64;
	typedef boost::uint32_t		uint32;
	typedef boost::uint16_t		uint16;
	typedef boost::uint8_t		uint8;

	typedef boost::int64_t		int64;
	typedef boost::int32_t		int32;
	typedef boost::int16_t		int16;
	typedef boost::int8_t		int8;

	typedef uint32 FourCC;
}

#endif		// _TYPES_HPP
