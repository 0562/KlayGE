// ShowFactory.hpp
// KlayGE ����������󹤳� ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ���ν��� (2006.7.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _SHOWFACTORY_HPP
#define _SHOWFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>

#include <boost/utility.hpp>

namespace KlayGE
{
	class ShowFactory
	{
	public:
		virtual ~ShowFactory()
			{ }

		static ShowFactoryPtr NullObject();

		virtual std::wstring const & Name() const = 0;
		virtual ShowEngine& ShowEngineInstance() = 0;
	};

	template <typename ShowEngineType>
	class ConcreteShowFactory : boost::noncopyable, public ShowFactory
	{
	public:
		ConcreteShowFactory(std::wstring const & name)
				: name_(name)
			{ }

		std::wstring const & Name() const
			{ return name_; }

		ShowEngine& ShowEngineInstance()
		{
			static ShowEngineType showEngine;
			return showEngine;
		}

	private:
		std::wstring const name_;
	};
}

#endif			// _SHOWFACTORY_HPP
