// DInputFactory.hpp
// KlayGE DirectInput����������󹤳� ͷ�ļ�
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

#ifndef _DINPUTFACTORY_HPP
#define _DINPUTFACTORY_HPP

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_DINPUT_IE_SOURCE			// Build dll
		#define KLAYGE_DINPUT_IE_API __declspec(dllexport)
	#else									// Use dll
		#define KLAYGE_DINPUT_IE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_DINPUT_IE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_DINPUT_IE_API void InputFactoryInstance(KlayGE::InputFactoryPtr& ptr);
	KLAYGE_DINPUT_IE_API std::string const & Name();
}

#endif			// _DINPUTFACTORY_HPP
