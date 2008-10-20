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
	void MakeInputFactory(KlayGE::InputFactoryPtr& ptr, boost::program_options::variables_map const & /*vm*/)
	{
		ptr = KlayGE::MakeSharedPtr<KlayGE::ConcreteInputFactory<KlayGE::DInputEngine> >(L"DirectInput Input Factory");
	}

	bool Match(std::string const & name, std::string const & compiler)
	{
		if (("DInput" == name) && (KLAYGE_COMPILER_TOOLSET == compiler))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
