// Types.hpp
// KlayGE ���Ͷ���ͷ�ļ�
// Ver 1.3.8.3
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 1.3.8.3
// ���ν���
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TYPES_HPP
#define _TYPES_HPP

namespace KlayGE
{
	#ifndef _WCHAR_T_DEFINED
		typedef unsigned short		wchar_t;
		#define _WCHAR_T_DEFINED
	#endif		// _WCHAR_T_DEFINED

	typedef unsigned int		UINT;
	typedef unsigned __int64	U64;
	typedef unsigned long		U32;
	typedef unsigned short		U16;
	typedef unsigned char		U8;

	typedef __int64				S64;
	typedef int					S32;
	typedef short				S16;
	typedef char				S8;

	typedef U32 FourCC;
}

#endif		// _TYPES_HPP
