// ShowFactory.cpp
// KlayGE ����������󹤳� ʵ���ļ�
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
#include <KlayGE/Show.hpp>

#include <string>

#include <KlayGE/ShowFactory.hpp>

namespace KlayGE
{
	class NullShowFactory : public ShowFactory
	{
	public:
		std::wstring const & Name() const
		{
			static std::wstring const name(L"Null Show Factory");
			return name;
		}

		ShowEngine& ShowEngineInstance()
		{
			return *ShowEngine::NullObject();
		}
	};

	ShowFactoryPtr ShowFactory::NullObject()
	{
		static ShowFactoryPtr obj(new NullShowFactory);
		return obj;
	}
}
