#ifndef _CONFIG_HPP
#define _CONFIG_HPP

// ������ֱ�����ѡ��
#define _SELECT1ST2ND_SUPPORT
#define _COPYIF_SUPPORT

#if defined(DEBUG) | defined(_DEBUG)
#define KLAYGE_DEBUG
#endif

#if _MSC_VER >= 1400
#define _VC_8_0
#elif _MSC_VER >= 1310
#define _VC_7_1
#endif

// ���屾�ص�endian��ʽ
#define _LITTLE_ENDIAN

#endif		// _CONFIG_HPP
