// OALAudioFactory.cpp
// KlayGE OpenAL��Ƶ������󹤳��� ʵ���ļ�
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

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/OpenAL/OALAudio.hpp>

#include <KlayGE/OpenAL/OALAudioFactory.hpp>

namespace KlayGE
{
	AudioFactory& OALAudioFactoryInstance()
	{
		static ConcreteAudioFactory<OALAudioEngine,
			OALSoundBuffer, OALMusicBuffer> audioFactory(L"OpenAL Audio Factory");
		return audioFactory;
	}
}
