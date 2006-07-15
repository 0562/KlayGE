// DShowFactory.cpp
// KlayGE DirectShow����������󹤳� ʵ���ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ���ν��� (2006.7.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/DShow/DShow.hpp>

#include <KlayGE/DShow/DShowFactory.hpp>

namespace KlayGE
{
	ShowFactory& DShowFactoryInstance()
	{
		static ConcreteShowFactory<DShowEngine> showFactory(L"DirectShow Show Factory");
		return showFactory;
	}
}
