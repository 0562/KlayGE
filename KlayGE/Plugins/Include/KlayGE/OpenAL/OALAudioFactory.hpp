// OALAudioFactory.hpp
// KlayGE OpenAL����������󹤳��� ͷ�ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// ��Ϊtemplateʵ�� (2004.3.4)
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OALAUDIOFACTORY_HPP
#define _OALAUDIOFACTORY_HPP

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4251 4275 4512 4702)
#endif
#include <boost/program_options.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#ifdef KLAYGE_HAS_DECLSPEC
	#ifdef KLAYGE_OAL_AE_SOURCE				// Build dll
		#define KLAYGE_OAL_AE_API __declspec(dllexport)
	#else									// Use dll
		#define KLAYGE_OAL_AE_API __declspec(dllimport)
	#endif
#else
	#define KLAYGE_OAL_AE_API
#endif // KLAYGE_HAS_DECLSPEC

extern "C"
{
	KLAYGE_OAL_AE_API void MakeAudioFactory(KlayGE::AudioFactoryPtr& ptr, boost::program_options::variables_map const & vm);
}

#endif			// _OALAUDIOFACTORY_HPP
