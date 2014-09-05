// InputFactory.cpp
// KlayGE ����������󹤳� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://www.klayge.org
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KFL/Util.hpp>
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

	private:
		InputEnginePtr DoMakeInputEngine()
		{
			return InputEngine::NullObject();
		}

		virtual void DoSuspend() KLAYGE_OVERRIDE
		{
		}
		virtual void DoResume() KLAYGE_OVERRIDE
		{
		}
	};

	InputFactoryPtr InputFactory::NullObject()
	{
		static InputFactoryPtr obj = MakeSharedPtr<NullInputFactory>();
		return obj;
	}

	InputEngine& InputFactory::InputEngineInstance()
	{
		if (!ie_)
		{
			ie_ = this->DoMakeInputEngine();
		}

		return *ie_;
	}

	void InputFactory::Suspend()
	{
		if (ie_)
		{
			ie_->Suspend();
		}
		this->DoSuspend();
	}

	void InputFactory::Resume()
	{
		this->DoResume();
		if (ie_)
		{
			ie_->Resume();
		}
	}
}
