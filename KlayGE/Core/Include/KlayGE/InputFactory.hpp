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
}

#endif			// _INPUTFACTORY_HPP
