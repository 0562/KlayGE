// OcclusionQuery.hpp
// KlayGE �ڵ������ ʵ���ļ�
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

#include <KlayGE/PreDeclare.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class OcclusionQuery
	{
	public:
		virtual ~OcclusionQuery()
		{
		}

		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual uint32_t SamplesPassed() = 0;
	};
}

#endif		// _OCCLUSIONQUERY_HPP
