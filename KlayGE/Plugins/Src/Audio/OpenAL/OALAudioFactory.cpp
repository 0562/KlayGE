// OALAudioFactory.cpp
// KlayGE OpenAL��Ƶ������󹤳��� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/SharePtr.hpp>
#include <KlayGE/OpenAL/OALAudio.hpp>

#include <KlayGE/OpenAL/OALAudioFactory.hpp>

namespace KlayGE
{
	// ��Ƶ����������
	/////////////////////////////////////////////////////////////////////////////////
	const WString& OALAudioFactory::Name() const
	{
		static WString name(L"OpenAL Audio Factory");
		return name;
	}

	// ��ȡ��Ƶ����ʵ��
	/////////////////////////////////////////////////////////////////////////////////
	AudioEngine& OALAudioFactory::AudioEngineInstance()
	{
		static OALAudioEngine audioEngine;
		return audioEngine;
	}

	// ��������������
	/////////////////////////////////////////////////////////////////////////////////
	AudioBufferPtr OALAudioFactory::MakeSoundBuffer(const AudioDataSourcePtr& dataSource, U32 sourceNum)
	{
		return AudioBufferPtr(new OALSoundBuffer(dataSource, sourceNum,
			this->AudioEngineInstance().SoundVolume()));
	}

	// �������ֻ�����
	/////////////////////////////////////////////////////////////////////////////////
	AudioBufferPtr OALAudioFactory::MakeMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds)
	{
		return AudioBufferPtr(new OALMusicBuffer(dataSource, bufferSeconds,
			this->AudioEngineInstance().MusicVolume()));
	}
}
