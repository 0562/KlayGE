// AudioEngine.cpp
// KlayGE ��Ƶ������ ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/SharePtr.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/Audio.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	AudioEngine::AudioEngine()
					: soundVol_(1),
						musicVol_(1)
	{
	}

	// ���б������һ����Ƶ������
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::AddBuffer(size_t id, const AudioBufferPtr& buffer)
	{
		audioBufs_.insert(id, buffer);
	}

	// �������е�����
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::PlayAll(bool bLoop)
	{
		for (AudioBufsIter iter = audioBufs_.begin(); iter != audioBufs_.end(); ++ iter)
		{
			audioBufs_.data(iter)->Play(bLoop);
		}
	}

	// ֹͣ���е�����
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::StopAll()
	{
		for (AudioBufsIter iter = audioBufs_.begin(); iter != audioBufs_.end(); ++ iter)
		{
			audioBufs_.data(iter)->Stop();
		}
	}

	// �б��ﻺ��������Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t AudioEngine::BufferNum() const
	{
		return audioBufs_.size();
	}

	// ��ȡ����������
	/////////////////////////////////////////////////////////////////////////////////
	AudioBufferPtr& AudioEngine::Buffer(size_t bufID)
	{
		AudioBufsIter iter(audioBufs_.find(bufID));
		if (iter != audioBufs_.end())
		{
			return audioBufs_.data(iter);
		}
		THR(E_FAIL);
	}

	const AudioBufferPtr& AudioEngine::Buffer(size_t bufID) const
	{
		AudioBufsConstIter iter(audioBufs_.find(bufID));
		if (iter != audioBufs_.end())
		{
			return audioBufs_.data(iter);
		}
		THR(E_FAIL);
	}

	// ������Ч������vol��ȡֵ��ΧΪ0--1.0f
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::SoundVolume(float vol)
	{
		soundVol_ = vol;

		for (AudioBufsIter iter = audioBufs_.begin(); iter != audioBufs_.end(); ++ iter)
		{
			if (audioBufs_.data(iter)->IsSound())
			{
				audioBufs_.data(iter)->Volume(vol);
			}
		}
	}

	// ��ȡ��Ч����
	/////////////////////////////////////////////////////////////////////////////////
	float AudioEngine::SoundVolume() const
	{
		return soundVol_;
	}

	// ��������������vol��ȡֵ��ΧΪ0--1.0f
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::MusicVolume(float vol)
	{
		musicVol_ = vol;

		for (AudioBufsIter iter = audioBufs_.begin(); iter != audioBufs_.end(); ++ iter)
		{
			if (!(audioBufs_.data(iter)->IsSound()))
			{
				audioBufs_.data(iter)->Volume(vol);
			}
		}
	}

	// ��ȡ��������
	/////////////////////////////////////////////////////////////////////////////////
	float AudioEngine::MusicVolume() const
	{
		return musicVol_;
	}
}
