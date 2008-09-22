// OcclusionQuery.hpp
// KlayGE �ڵ���ѯ�� ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ���ν��� (2005.9.27)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OCCLUSIONQUERY_HPP
#define _OCCLUSIONQUERY_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Query.hpp>

namespace KlayGE
{
	class OcclusionQuery : public Query
	{
	public:
		virtual ~OcclusionQuery()
		{
		}

		virtual uint64_t SamplesPassed() = 0;
	};
}

#endif		// _OCCLUSIONQUERY_HPP
