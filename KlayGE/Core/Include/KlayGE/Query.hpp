// Query.hpp
// KlayGE ��ѯ������ ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ���ν��� (2005.10.18)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _QUERY_HPP
#define _QUERY_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>

namespace KlayGE
{
	class Query
	{
	public:
		virtual ~Query()
		{
		}

		static QueryPtr NullObject();

		virtual void Begin() = 0;
		virtual void End() = 0;
	};
}

#endif		// _QUERY_HPP
