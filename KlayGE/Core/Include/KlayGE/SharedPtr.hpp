// SharedPtr.hpp
// KlayGE ��������ָ�� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _SHAREDPTR_HPP
#define _SHAREDPTR_HPP

#include <KlayGE/ResPtr.hpp>

namespace KlayGE
{
	// ���ü���ָ��
	/////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	class SharedPtr : public ResPtr<T>
	{
	public:
		SharedPtr()
			: ResPtr<T>()
			{ }
		explicit SharedPtr(const stored_type& p)
			: ResPtr<T>(p)
			{ }
		SharedPtr(const SharedPtr& rhs)
			: ResPtr<T>(rhs)
			{ }
		template <typename U>
		SharedPtr(const SharedPtr<U>& rhs)
			: ResPtr<T>(rhs)
			{ }
	};
}

#endif			// _SHAREDPTR_HPP
