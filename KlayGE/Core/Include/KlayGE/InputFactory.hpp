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

#include <boost/utility.hpp>

namespace KlayGE
{
	class InputFactory
	{
	public:
		virtual ~InputFactory()
			{ }

		virtual std::wstring const & Name() const = 0;
		virtual InputEngine& InputEngineInstance() = 0;
	};

	template <typename InputEngineType>
	class ConcreteInputFactory : boost::noncopyable, public InputFactory
	{
	public:
		ConcreteInputFactory(std::wstring const & name)
				: name_(name)
			{ }

		std::wstring const & Name() const
			{ return name_; }

		InputEngine& InputEngineInstance()
		{
			static InputEngineType inputEngine;
			return inputEngine;
		}

	private:
		std::wstring const name_;
	};
}

#endif			// _INPUTFACTORY_HPP
