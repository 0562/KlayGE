// Query.hpp
// KlayGE ��ѯ������ ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Query.hpp>

namespace KlayGE
{
	class NullQuery : public Query
	{
	public:
		void Begin()
		{
		}
		void End()
		{
		}
	};

	QueryPtr Query::NullObject()
	{
		static QueryPtr obj(new NullQuery);
		return obj;
	}
}
