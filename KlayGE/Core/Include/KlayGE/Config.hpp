#ifndef _CONFIG_HPP
#define _CONFIG_HPP

// ������ֱ�����ѡ��
#define _SELECT1ST2ND_SUPPORT
#define _COPYIF_SUPPORT

#if defined(DEBUG) | defined(_DEBUG)
#define KLAYGE_DEBUG
#endif

#ifdef KLAYGE_DEBUG
#define D3D_DEBUG_INFO
#endif

// ���屾�ص�endian��ʽ
#define _LITTLE_ENDIAN

#endif		// _CONFIG_HPP
