// DInputFactory.cpp
// KlayGE DirectInput����������󹤳� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/DInput/DInput.hpp>

#include <KlayGE/DInput/DInputFactory.hpp>

namespace KlayGE
{
	// ���빤��������
	/////////////////////////////////////////////////////////////////////////////////
	const WString& DInputFactory::Name() const
	{
		static WString name(L"DirectInput Input Factory");
		return name;
	}

	// ��ȡ��������ʵ��
	//////////////////////////////////////////////////////////////////////////////////
	InputEngine& DInputFactory::InputEngineInstance()
	{
		static DInputEngine inputEngine;
		return inputEngine;
	}
}