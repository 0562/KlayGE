// ArchiveOpenCallback.hpp
// KlayGE ���ϵͳ��ѹ�����ص����� ͷ�ļ� ����7zip
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _ARCHIVE_OPEN_CALLBACK_HPP
#define _ARCHIVE_OPEN_CALLBACK_HPP

#include <string>

#include "IPassword.hpp"
#include "IArchive.hpp"

namespace KlayGE
{
	class CArchiveOpenCallback : public IArchiveOpenCallback, public ICryptoGetTextPassword
	{
	public:
		STDMETHOD_(ULONG, AddRef)()
		{
			return ++ ref_count_;
		}
		STDMETHOD_(ULONG, Release)()
		{
			-- ref_count_;
			if (0 == ref_count_)
			{
				delete this;
				return 0;
			}
			return ref_count_;
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
				if (IID_IArchiveOpenCallback == iid)
				{
					*outObject = static_cast<IArchiveOpenCallback*>(this);
					this->AddRef();
					return S_OK;
				}
				else
				{
					return E_NOINTERFACE;
				}
			}
		}

		STDMETHOD(SetTotal)(const uint64_t* files, const uint64_t* bytes);
		STDMETHOD(SetCompleted)(const uint64_t* files, const uint64_t* bytes);

		// ICryptoGetTextPassword
		STDMETHOD(CryptoGetTextPassword)(BSTR *password);

		CArchiveOpenCallback()
			: ref_count_(1)
		{
		}

		void Init(std::string const & pw);

	private:
		uint32_t ref_count_;

		bool password_is_defined_;
		std::wstring password_;
	};
}

#endif		// _ARCHIVE_OPEN_CALLBACK_HPP
