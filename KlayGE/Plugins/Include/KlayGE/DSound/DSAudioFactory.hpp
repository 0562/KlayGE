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

#pragma comment(lib, "KlayGE_AudioEngine_DSound.lib")

namespace KlayGE
{
	AudioFactory& DSAudioFactoryInstance();
}

#endif			// _DSAUDIOFACTORY_HPP
