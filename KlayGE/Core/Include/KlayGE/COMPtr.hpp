// COMPtr.hpp
// KlayGE ����COM����ָ�� ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ���ν��� (2006.7.20)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _COMPTR_HPP
#define _COMPTR_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <boost/smart_ptr.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#ifdef KLAYGE_PLATFORM_WIN32
#define BOOST_MEM_FN_ENABLE_STDCALL
#endif
#endif
#include <boost/mem_fn.hpp>

namespace KlayGE
{
	// �õ�COM���������ָ��
	template <typename T>
	inline boost::shared_ptr<T>
	MakeCOMPtr(T* p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}
}

#endif		// _COMPTR_HPP
