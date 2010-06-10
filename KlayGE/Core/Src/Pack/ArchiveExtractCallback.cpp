// ArchiveExtractCallback.cpp
// KlayGE ���ϵͳ��ȡ�ص����� ʵ���ļ� ����7zip
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

#include "BaseDefines.hpp"

#include "BSTR.hpp"
#include "ArchiveExtractCallback.hpp"

namespace KlayGE
{
	STDMETHODIMP CArchiveExtractCallback::SetTotal(uint64_t /*size*/)
	{
		return S_OK;
	}

	STDMETHODIMP CArchiveExtractCallback::SetCompleted(const uint64_t* /*completeValue*/)
	{
		return S_OK;
	}

	STDMETHODIMP CArchiveExtractCallback::GetStream(uint32_t /*index*/, ISequentialOutStream** outStream, int32_t askExtractMode)
	{
		enum 
		{
			kExtract = 0,
			kTest,
			kSkip,
		};

		if (kExtract == askExtractMode)
		{
			_outFileStream->AddRef();
			*outStream = _outFileStream.get();
		}
		else
		{
			*outStream = NULL;
		}
		return S_OK;
	}

	STDMETHODIMP CArchiveExtractCallback::PrepareOperation(int32_t /*askExtractMode*/)
	{
		return S_OK;
	}

	STDMETHODIMP CArchiveExtractCallback::SetOperationResult(int32_t /*operationResult*/)
	{
		return S_OK;
	}

	STDMETHODIMP CArchiveExtractCallback::CryptoGetTextPassword(BSTR* password)
	{
		if (!password_is_defined_)
		{
			return E_ABORT;
		}
		else
		{
			*password = AllocBSTR(password_.c_str());
			return S_OK;
		}
	}

	void CArchiveExtractCallback::Init(std::string const & pw, boost::shared_ptr<ISequentialOutStream> const & outFileStream)
	{
		_outFileStream = outFileStream;

		password_is_defined_ = !pw.empty();
		Convert(password_, pw);
	}
}
