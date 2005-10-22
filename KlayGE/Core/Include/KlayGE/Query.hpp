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

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	class Query
	{
	public:
		virtual ~Query()
		{
		}

		virtual void Begin() = 0;
		virtual void End() = 0;
	};
}

#endif		// _QUERY_HPP
