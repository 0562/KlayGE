// DSAudioFactory.hpp
// KlayGE DirectSound����������󹤳��� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
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
	class DSAudioFactory : public AudioFactory
	{
	public:
		const WString& Name() const;

		AudioEngine& AudioEngineInstance();

		AudioBufferPtr MakeSoundBuffer(const AudioDataSourcePtr& dataSource, U32 sourceNum = 1);
		AudioBufferPtr MakeMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds = 2);
	};
}

#endif			// _DSAUDIOFACTORY_HPP
