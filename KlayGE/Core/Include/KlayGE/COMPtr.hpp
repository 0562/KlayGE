// COMPtr.hpp
// KlayGE COM����ָ�� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _COMPTR_HPP
#define _COMPTR_HPP

#define NOMINMAX
#include <windows.h>
#include <KlayGE/ResPtr.hpp>

namespace KlayGE
{
	// COM���ü�������
	/////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	class COMRefCountedOP
	{
	public:
		COMRefCountedOP()
			{ }

		template <class U>
		COMRefCountedOP(const COMRefCountedOP<U>&)
			{ }

		static T Clone(const T& p)
		{
			if (p != 0)
			{
				p->AddRef();
			}
			return p;
		}
		static bool Release(const T& p)
		{
			if (p != 0)
			{
				p->Release();
			}
			return false;
		}

		void Swap(COMRefCountedOP& /*rhs*/)
			{ }
	};

	// COMָ��
	/////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	class COMPtr : public ResPtr<T, COMRefCountedOP>
	{
	public:
		COMPtr()
			: ResPtr<T, COMRefCountedOP>()
			{ }
		explicit COMPtr(const stored_type& p)
			: ResPtr<T, COMRefCountedOP>(p)
			{ }
		COMPtr(const COMPtr& rhs)
			: ResPtr<T, COMRefCountedOP>(rhs)
			{ }
		template <typename U>
		COMPtr(const COMPtr<U>& rhs)
			: ResPtr<T, COMRefCountedOP>(rhs)
			{ }

		template <REFIID iid>
		HRESULT CoCreateInstance(REFCLSID rclsid, U32 clsContext = CLSCTX_ALL, IUnknown* pUnkOuter = NULL)
		{
			this->Release();
			return ::CoCreateInstance(rclsid, pUnkOuter, clsContext, iid, reinterpret_cast<void**>(&(this->GetRef())));
		}

		template <REFIID iid, typename U>
		HRESULT QueryInterface(COMPtr<U>& u) const
		{
			U* p(0);

			HRESULT hr(this->Get()->QueryInterface(iid, reinterpret_cast<void**>(&p)));
			if (SUCCEEDED(hr))
			{
				u = COMPtr<U>(p);
			}

			return hr;
		}
	};
}

#endif			// _COMPTR_HPP