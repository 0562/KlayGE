// InputFactory.hpp
// KlayGE ����������󹤳� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _INPUTFACTORY_HPP
#define _INPUTFACTORY_HPP

namespace KlayGE
{
	class InputFactory
	{
	public:
		virtual ~InputFactory()
			{ }

		virtual const WString& Name() const = 0;
		virtual InputEngine& InputEngineInstance() = 0;
	};

	template <typename InputEngineType>
	class ConcreteInputFactory : public InputFactory
	{
	public:
		ConcreteInputFactory(const WString& name)
				: name_(name)
			{ }

		const WString& Name() const
			{ return name_; }

		InputEngine& InputEngineInstance()
		{
			static InputEngineType inputEngine;
			return inputEngine;
		}

	private:
		const WString name_;
	};
}

#endif			// _INPUTFACTORY_HPP
