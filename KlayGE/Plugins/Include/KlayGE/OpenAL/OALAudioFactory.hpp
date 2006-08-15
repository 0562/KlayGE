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

#define KLAYGE_LIB_NAME KlayGE_AudioEngine_OpenAL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/OpenAL/OALAudio.hpp>

namespace KlayGE
{
	AudioFactory& OALAudioFactoryInstance();
}

#endif			// _OALAUDIOFACTORY_HPP
