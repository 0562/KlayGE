// DInputFactory.hpp
// KlayGE DirectInput����������󹤳� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DINPUTFACTORY_HPP
#define _DINPUTFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/InputFactory.hpp>

#pragma comment(lib, "KlayGE_InputEngine_DInput.lib")

namespace KlayGE
{
	class DInputFactory : public InputFactory
	{
	public:
		const WString& Name() const;

		InputEngine& InputEngineInstance();
	};
}

#endif			// _DINPUTFACTORY_HPP