// SharePtr.hpp
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

#ifndef _SHAREPTR_HPP
#define _SHAREPTR_HPP

#include <KlayGE/ResPtr.hpp>

namespace KlayGE
{
	// ���ü���ָ��
	/////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	class SharePtr : public ResPtr<T>
	{
	public:
		SharePtr()
			: ResPtr<T>()
			{ }
		explicit SharePtr(const stored_type& p)
			: ResPtr<T>(p)
			{ }
		SharePtr(const SharePtr& rhs)
			: ResPtr<T>(rhs)
			{ }
		template <typename U>
		SharePtr(const SharePtr<U>& rhs)
			: ResPtr<T>(rhs)
			{ }
	};
}

#endif			// _SHAREPTR_HPP