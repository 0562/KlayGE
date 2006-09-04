// ThrowErr.cpp
// KlayGE �׳����� ʵ���ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// Ver 3.4.0
// ���ν��� (2006.9.5)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <sstream>

#include <KlayGE/ThrowErr.hpp>

namespace KlayGE
{
	Exception::Exception(std::string const & errFile, uint32_t errLine, int32_t errCode, std::string const & msg) throw()
			: errFile_(errFile), errLine_(errLine),
				errCode_(errCode)
	{
		std::ostringstream ss;
		ss << "File: " << errFile << " Line: " << errLine << " ErrCode: " << errCode << " Msg: " << msg;
		msg_ = ss.str();
	}
}
