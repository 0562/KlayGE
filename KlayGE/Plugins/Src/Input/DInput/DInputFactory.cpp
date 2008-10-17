// DInputFactory.cpp
// KlayGE DirectInput����������󹤳� ʵ���ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// ��Ϊtemplateʵ�� (2004.3.4)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/InputFactory.hpp>

#include <KlayGE/DInput/DInput.hpp>
#include <KlayGE/DInput/DInputFactory.hpp>

extern "C"
{
	KlayGE::InputFactoryPtr const & InputFactoryInstance()
	{
		static KlayGE::InputFactoryPtr ifactory = KlayGE::MakeSharedPtr<KlayGE::ConcreteInputFactory<KlayGE::DInputEngine> >(L"DirectInput Input Factory");
		return ifactory;
	}

	std::string const & Name()
	{
		static std::string const name("DInput");
		return name;
	}
}
