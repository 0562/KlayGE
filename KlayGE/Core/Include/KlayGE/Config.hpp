#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#if defined(DEBUG) | defined(_DEBUG)
#define KLAYGE_DEBUG
#endif

#ifdef _MSC_VER
#define KLAYGE_COMPILER_MSVC
#if _MSC_VER >= 1400
#define KLAYGE_COMPILER_VERSION 8.0
#pragma warning(disable: 4819)
#elif _MSC_VER >= 1310
#define KLAYGE_COMPILER_VERSION 7.1
#endif
#endif

#ifdef __GNUC__
#define KLAYGE_COMPILER_GCC
#if __GNUC__ == 2
#define KLAYGE_COMPILER_VERSION 2.0
#elif __GNUC__ == 3
#define KLAYGE_COMPILER_VERSION 3.0
#elif __GNUC__ == 4
#define KLAYGE_COMPILER_VERSION 4.0
#endif
#endif


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define KLAYGE_PLATFORM_WINDOWS
// �ر�windows.h��min/max
#define NOMINMAX
#if defined(__MINGW32__)
#include <_mingw.h>
#endif
#elif defined(__CYGWIN__)
#define KLAYGE_PLATFORM_CYGWIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define KLAYGE_PLATFORM_LINUX
#endif

// ������ֱ�����ѡ��
#define _SELECT1ST2ND_SUPPORT
#define _COPYIF_SUPPORT


// ���屾�ص�endian��ʽ
#define KLAYGE_LITTLE_ENDIAN

#endif		// _CONFIG_HPP
