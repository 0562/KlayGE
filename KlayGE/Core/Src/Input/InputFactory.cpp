// InputFactory.cpp
// KlayGE ����������󹤳� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Input.hpp>

#include <string>

#include <KlayGE/InputFactory.hpp>

namespace KlayGE
{
	class NullInputFactory : public InputFactory
	{
	public:
		std::wstring const & Name() const
		{
			static std::wstring const name(L"Null Input Factory");
			return name;
		}

		InputEngine& InputEngineInstance()
		{
			return *InputEngine::NullObject();
		}
	};

	InputFactoryPtr InputFactory::NullObject()
	{
		static InputFactoryPtr obj(new NullInputFactory);
		return obj;
	}
}
