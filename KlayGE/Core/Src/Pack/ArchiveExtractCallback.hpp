// ArchiveExtractCallback.hpp
// KlayGE ���ϵͳ��ȡ�ص����� ͷ�ļ� ����7zip
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://www.klayge.org
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _ARCHIVEEXTRACTCALLBACK_HPP
#define _ARCHIVEEXTRACTCALLBACK_HPP

#pragma once

#include <KlayGE/atomic.hpp>

#include <string>

#include "IArchive.hpp"
#include "IPassword.hpp"

namespace KlayGE
{
	class CArchiveExtractCallback : public IArchiveExtractCallback, public ICryptoGetTextPassword
	{
	public:
		STDMETHOD_(ULONG, AddRef)()
		{
			++ ref_count_;
			return ref_count_.value();
		}
		STDMETHOD_(ULONG, Release)()
		{
			-- ref_count_;
			if (0 == ref_count_.value())
			{
				delete this;
				return 0;
			}
			return ref_count_.value();
		}

		STDMETHOD(QueryInterface)(REFGUID iid, void** outObject)
		{
			if (IID_ICryptoGetTextPassword == iid)
			{
				*outObject = static_cast<ICryptoGetTextPassword*>(this);
				this->AddRef();
				return S_OK;
			}
			else
			{
				if (IID_IArchiveExtractCallback == iid)
				{
					*outObject = static_cast<IArchiveExtractCallback*>(this);
					this->AddRef();
					return S_OK;
				}
				else
				{
					return E_NOINTERFACE;
				}
			}
		}

		// IProgress
		STDMETHOD(SetTotal)(uint64_t size);
		STDMETHOD(SetCompleted)(const uint64_t* completeValue);

		// IExtractCallBack
		STDMETHOD(GetStream)(uint32_t index, ISequentialOutStream **outStream, int32_t askExtractMode);
		STDMETHOD(PrepareOperation)(int32_t askExtractMode);
		STDMETHOD(SetOperationResult)(int32_t resultEOperationResult);

		// ICryptoGetTextPassword
		STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword);


	public:
		CArchiveExtractCallback()
			: ref_count_(1)
		{
		}

		void Init(std::string const & pw, boost::shared_ptr<ISequentialOutStream> const & outFileStream);

	private:
		atomic<int32_t> ref_count_;

		bool password_is_defined_;
		std::wstring password_;

		boost::shared_ptr<ISequentialOutStream> _outFileStream;
	};
}

#endif		// _ARCHIVEEXTRACTCALLBACK_HPP
