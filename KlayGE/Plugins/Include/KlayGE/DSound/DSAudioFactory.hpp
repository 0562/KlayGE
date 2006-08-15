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

#define KLAYGE_LIB_NAME KlayGE_AudioEngine_DSound
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/AudioFactory.hpp>

namespace KlayGE
{
	AudioFactory& DSAudioFactoryInstance();
}

#endif			// _DSAUDIOFACTORY_HPP
