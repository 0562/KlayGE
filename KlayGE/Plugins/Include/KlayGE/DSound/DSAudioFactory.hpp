// DSAudioFactory.hpp
// KlayGE DirectSound����������󹤳��� ͷ�ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// ��Ϊtemplateʵ�� (2004.3.4)
//
// 2.0.0
// ���ν��� (2003.10.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DSAUDIOFACTORY_HPP
#define _DSAUDIOFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/AudioFactory.hpp>

#ifdef _DEBUG
	#pragma comment(lib, "KlayGE_AudioEngine_DSound_d.lib")
#else
	#pragma comment(lib, "KlayGE_AudioEngine_DSound.lib")
#endif

namespace KlayGE
{
	AudioFactory& DSAudioFactoryInstance();
}

#endif			// _DSAUDIOFACTORY_HPP
