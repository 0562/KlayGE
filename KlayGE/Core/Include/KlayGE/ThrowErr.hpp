// ThrowErr.hpp
// KlayGE �׳����� ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2001-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ����Boost.System���쳣 (2008.5.21)
//
// 1.3.8.1
// ���ν���
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _THROWERR_HPP
#define _THROWERR_HPP

#include <string>
#include <stdexcept>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif

namespace KlayGE
{
	std::string CombineFileLine(std::string const & file, int line);
}

#define THR(x)			{ throw boost::system::system_error(boost::system::posix_error::make_error_code(x), KlayGE::CombineFileLine(__FILE__, __LINE__)); }

// ������󣬾��׳��������
#define TIF(x)			{ HRESULT _hr = x; if (static_cast<HRESULT>(_hr) < 0) { throw std::runtime_error(KlayGE::CombineFileLine(__FILE__, __LINE__)); } }

// ����
inline void
Verify(bool x)
{
	if (!x)
	{
		THR(boost::system::posix_error::not_supported);
	}
}

#endif		// _THROWERR_HPP
