// Bound.hpp
// KlayGE �߿� ͷ�ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.1
// ���ν��� (2004.4.30)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#ifndef _BOUND_HPP
#define _BOUND_HPP

#include <boost/operators.hpp>

namespace KlayGE
{
	class Bound
	{
	public:
		virtual ~Bound()
		{
		}

		virtual bool IsEmpty() const = 0;

		virtual bool VecInBound(Vector3 const & v) const = 0;
		virtual float MaxRadiusSq() const = 0;
	};
}

#endif			// _BOUND_HPP
