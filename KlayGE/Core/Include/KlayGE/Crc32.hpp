// Crc32.hpp
// KlayGE CRC32 ͷ�ļ�
// Ver 1.2.8.10
// ��Ȩ����(C) ������, 2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.10
// ��string�����ַ���ָ�� (2002.10.27)
//
// 1.2.8.11
// ����UNICODE�ں� (2002.11.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _CRC32_HPP
#define _CRC32_HPP

#include <KlayGE/PreDeclare.hpp>
#include <string>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	namespace Crc32
	{
		U32 CrcMem(const U8* data, size_t len);
		U32 CrcString(const std::string& str);
		U32 CrcFile(VFile& file);
	};
}

#endif		// _CRC32_HPP
