// DSAudioFactory.cpp
// KlayGE DSound��Ƶ������󹤳��� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.10.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/SharePtr.hpp>
#include <KlayGE/DSound/DSAudio.hpp>

#include <KlayGE/DSound/DSAudioFactory.hpp>

namespace KlayGE
{
	// ��Ƶ����������
	/////////////////////////////////////////////////////////////////////////////////
	const WString& DSAudioFactory::Name() const
	{
		static WString name(L"DirectSound Audio Factory");
		return name;
	}

	// ��ȡ��Ƶ����ʵ��
	/////////////////////////////////////////////////////////////////////////////////
	AudioEngine& DSAudioFactory::AudioEngineInstance()
	{
		static DSAudioEngine audioEngine;
		return audioEngine;
	}

	// ��������������
	/////////////////////////////////////////////////////////////////////////////////
	AudioBufferPtr DSAudioFactory::MakeSoundBuffer(const AudioDataSourcePtr& dataSource, U32 sourceNum)
	{
		return AudioBufferPtr(new DSSoundBuffer(dataSource, sourceNum,
			this->AudioEngineInstance().SoundVolume()));
	}

	// �������ֻ�����
	/////////////////////////////////////////////////////////////////////////////////
	AudioBufferPtr DSAudioFactory::MakeMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds)
	{
		return AudioBufferPtr(new DSMusicBuffer(dataSource, bufferSeconds,
			this->AudioEngineInstance().MusicVolume()));
	}
}
