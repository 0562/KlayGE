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
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/Audio.hpp>

namespace KlayGE
{
	class NullAudioEngine : public AudioEngine
	{
	public:
		const WString& Name() const
		{
			static WString name(L"Null Audio Engine");
			return name;
		}

		void AddBuffer(size_t id, const AudioBufferPtr& buffer)
			{ }

		size_t BufferNum() const
			{ return 0; }
		AudioBufferPtr Buffer(size_t bufID)
			{ return AudioBuffer::NullObject(); }
		const AudioBufferPtr Buffer(size_t bufID) const
			{ return AudioBuffer::NullObject(); }

		void Play(size_t bufID, bool loop = false)
			{ }
		void Stop(size_t bufID)
			{ }
		void PlayAll(bool loop = false)
			{ }
		void StopAll()
			{ }

		// ���úͻ�ȡ����
		void  SoundVolume(float vol)
			{ }
		float SoundVolume() const
			{ }
		void  MusicVolume(float vol)
			{ }
		float MusicVolume() const
			{ }

		Vector3 GetListenerPos() const
			{ return Vector3::Zero(); }
		void SetListenerPos(const Vector3& v)
			{ }
		Vector3 GetListenerVel() const
			{ return Vector3::Zero(); }
		void SetListenerVel(const Vector3& v)
			{ }
		void GetListenerOri(Vector3& face, Vector3& up) const
		{
			face = Vector3::Zero();
			up = Vector3::Zero();
		}
		void SetListenerOri(const Vector3& face, const Vector3& up)
			{ }
	};

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	AudioEngine::AudioEngine()
					: soundVol_(1),
						musicVol_(1)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	AudioEngine::~AudioEngine()
	{
	}

	// ��ȡ�ն���
	/////////////////////////////////////////////////////////////////////////////////
	AudioEnginePtr AudioEngine::NullObject()
	{
		static AudioEnginePtr obj(new NullAudioEngine);
		return obj;
	}

	// ���б������һ����Ƶ������
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::AddBuffer(size_t id, const AudioBufferPtr& buffer)
	{
		audioBufs_.insert(id, buffer);
	}

	// ����id��ָ��������
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::Play(size_t bufID, bool loop)
	{
		this->Buffer(bufID)->Play(loop);
	}

	// ֹͣid��ָ��������
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::Stop(size_t bufID)
	{
		this->Buffer(bufID)->Stop();
	}	

	// �������е�����
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::PlayAll(bool loop)
	{
		for (AudioBufsIter iter = audioBufs_.begin(); iter != audioBufs_.end(); ++ iter)
		{
			audioBufs_.data(iter)->Play(loop);
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
	AudioBufferPtr AudioEngine::Buffer(size_t bufID)
	{
		AudioBufsIter iter(audioBufs_.find(bufID));
		if (iter != audioBufs_.end())
		{
			return audioBufs_.data(iter);
		}
		THR(E_FAIL);
	}

	const AudioBufferPtr AudioEngine::Buffer(size_t bufID) const
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
