// Random.hpp
// KlayGE ����� ʵ���ļ�
// Ver 2.1.1
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.1
// ���ν��� (2004.4.22)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <ctime>

#include <KlayGE/Math.hpp>

namespace KlayGE
{
	Random& Random::Instance()
	{
		static Random random;
		return random;
	}

	Random::Random()
	{
		std::srand(static_cast<unsigned int>(time(NULL)));
	}

	int Random::Next() const
	{
		return std::rand();
	}
}
