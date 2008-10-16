// DSAudioFactory.cpp
// KlayGE DSound��Ƶ������󹤳��� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.10.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/AudioFactory.hpp>

#include <KlayGE/DSound/DSAudio.hpp>
#include <KlayGE/DSound/DSAudioFactory.hpp>

namespace KlayGE
{
	AudioFactoryPtr const & DSAudioFactoryInstance()
	{
		static AudioFactoryPtr af = MakeSharedPtr<ConcreteAudioFactory<DSAudioEngine,
			DSSoundBuffer, DSMusicBuffer> >(L"DirectSound Audio Factory");
		return af;
	}	
}
