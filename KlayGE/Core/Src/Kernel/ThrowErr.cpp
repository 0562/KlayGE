// ThrowErr.cpp
// KlayGE �׳����� ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2006-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ����Boost.System���쳣 (2008.5.21)
//
// 3.4.0
// ���ν��� (2006.9.5)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <sstream>

#include <KlayGE/ThrowErr.hpp>

namespace KlayGE
{
	std::string CombineFileLine(std::string const & file, int line)
	{
		std::stringstream ss;
		ss << file << ": " << line;
		return ss.str();
	}
}
