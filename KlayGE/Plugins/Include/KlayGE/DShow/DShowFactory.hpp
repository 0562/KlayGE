// DShowFactory.hpp
// KlayGE DirectShow����������󹤳� ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ���ν��� (2006.7.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DSHOWFACTORY_HPP
#define _DSHOWFACTORY_HPP

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_DSHOW_SE_SOURCE				// Build dll
		#define KLAYGE_DSHOW_SE_API __declspec(dllexport)
	#else										// Use dll
		#define KLAYGE_DSHOW_SE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_DSHOW_SE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_DSHOW_SE_API void ShowFactoryInstance(KlayGE::ShowFactoryPtr& ptr);
	KLAYGE_DSHOW_SE_API std::string const & Name();
}

#endif			// _DSHOWFACTORY_HPP
