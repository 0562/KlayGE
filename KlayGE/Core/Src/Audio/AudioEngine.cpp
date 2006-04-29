// AudioEngine.cpp
// KlayGE ��Ƶ������ ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <boost/assert.hpp>
#pragma warning(disable: 4239)
#include <boost/bind.hpp>

#include <KlayGE/Audio.hpp>

namespace KlayGE
{
	class NullAudioEngine : public AudioEngine
	{
	public:
		std::wstring const & Name() const
		{
			static std::wstring const name(L"Null Audio Engine");
			return name;
		}

		void AddBuffer(size_t /*id*/, AudioBufferPtr const & /*buffer*/)
			{ }

		AudioBufferPtr Buffer(size_t /*bufID*/) const
			{ return AudioBuffer::NullObject(); }

		Vector3 GetListenerPos() const
			{ return Vector3::Zero(); }
		void SetListenerPos(Vector3 const & /*v*/)
			{ }
		Vector3 GetListenerVel() const
			{ return Vector3::Zero(); }
		void SetListenerVel(Vector3 const & /*v*/)
			{ }
		void GetListenerOri(Vector3& face, Vector3& up) const
		{
			face = Vector3::Zero();
			up = Vector3::Zero();
		}
		void SetListenerOri(Vector3 const & /*face*/, Vector3 const & /*up*/)
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
	void AudioEngine::AddBuffer(size_t id, AudioBufferPtr const & buffer)
	{
		audioBufs_.insert(std::make_pair(id, buffer));
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
		std::for_each(audioBufs_.begin(), audioBufs_.end(),
			boost::bind(&AudioBuffer::Play,
				boost::bind(select2nd<AudioBufs::value_type>(), _1), loop));
	}

	// ֹͣ���е�����
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::StopAll()
	{
		std::for_each(audioBufs_.begin(), audioBufs_.end(),
			boost::bind(&AudioBuffer::Stop,
				boost::bind(select2nd<AudioBufs::value_type>(), _1)));
	}

	// �б��ﻺ��������Ŀ
	/////////////////////////////////////////////////////////////////////////////////
	size_t AudioEngine::NumBuffer() const
	{
		return audioBufs_.size();
	}

	// ��ȡ����������
	/////////////////////////////////////////////////////////////////////////////////
	AudioBufferPtr AudioEngine::Buffer(size_t bufID) const
	{
		AudioBufsConstIter iter(audioBufs_.find(bufID));
		if (iter != audioBufs_.end())
		{
			return iter->second;
		}

		BOOST_ASSERT(false);
		return AudioBufferPtr();
	}

	// ������Ч������vol��ȡֵ��ΧΪ0--1.0f
	/////////////////////////////////////////////////////////////////////////////////
	void AudioEngine::SoundVolume(float vol)
	{
		soundVol_ = vol;

		for (AudioBufsIter iter = audioBufs_.begin(); iter != audioBufs_.end(); ++ iter)
		{
			if (iter->second->IsSound())
			{
				iter->second->Volume(vol);
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
			if (!(iter->second->IsSound()))
			{
				iter->second->Volume(vol);
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
