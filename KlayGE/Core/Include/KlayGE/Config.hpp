#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#if defined(DEBUG) | defined(_DEBUG)
#define KLAYGE_DEBUG
#endif

#if _MSC_VER >= 1400
#define _VC_8_0
#elif _MSC_VER >= 1310
#define _VC_7_1
#endif

#if __GNUC__ == 2
#define _GCC_2_x
#elif __GNUC__ == 3
#define _GCC_3_x
#elif __GNUC__ == 4
#define _GCC_4_x
#endif

// ������ֱ�����ѡ��
#define _SELECT1ST2ND_SUPPORT
#define _COPYIF_SUPPORT

// ���屾�ص�endian��ʽ
#define _LITTLE_ENDIAN

// �ر�windows.h��min/max
#define NOMINMAX

#endif		// _CONFIG_HPP
