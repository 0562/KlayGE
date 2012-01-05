// ArchiveOpenCallback.cpp
// KlayGE ���ϵͳ��ѹ�����ص����� ʵ���ļ� ����7zip
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://www.klayge.org
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>

#include <CPP/Common/MyWindows.h>

#include "ArchiveOpenCallback.hpp"

namespace KlayGE
{
	STDMETHODIMP CArchiveOpenCallback::SetTotal(const uint64_t* /*files*/, const uint64_t* /*bytes*/)
	{
		return S_OK;
	}

	STDMETHODIMP CArchiveOpenCallback::SetCompleted(const uint64_t* /*files*/, const uint64_t* /*bytes*/)
	{
		return S_OK;
	}

	STDMETHODIMP CArchiveOpenCallback::CryptoGetTextPassword(BSTR* password)
	{
		if (!password_is_defined_)
		{
			return E_ABORT;
		}
		else
		{
#ifdef KLAYGE_PLATFORM_WINDOWS
			*password = SysAllocString(password_.c_str());
#else
			*password = NULL;
#endif
			return S_OK;
		}
	}

	void CArchiveOpenCallback::Init(std::string const & pw)
	{
		password_is_defined_ = !pw.empty();
		Convert(password_, pw);
	}
}
