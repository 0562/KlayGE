// AudioFactory.hpp
// KlayGE ��Ƶ������󹤳� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIOFACTORY_HPP
#define _AUDIOFACTORY_HPP

namespace KlayGE
{
	class AudioFactory
	{
	public:
		virtual ~AudioFactory()
			{ }

		virtual const std::wstring& Name() const = 0;

		virtual AudioEngine& AudioEngineInstance() = 0;
		virtual AudioBufferPtr MakeSoundBuffer(const AudioDataSourcePtr& dataSource, U32 numSource = 1) = 0;
		virtual AudioBufferPtr MakeMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds = 2) = 0;
	};

	template <typename AudioEngineType, typename SoundBufferType, typename MusicBufferType>
	class ConcreteAudioFactory : public AudioFactory
	{
	public:
		ConcreteAudioFactory(const std::wstring& name)
			: name_(name)
			{ }

		const std::wstring& Name() const
			{ return name_; }

		AudioEngine& AudioEngineInstance()
		{
			static AudioEngineType audioEngine;
			return audioEngine;
		}

		AudioBufferPtr MakeSoundBuffer(const AudioDataSourcePtr& dataSource, U32 numSource = 1)
		{
			return AudioBufferPtr(new SoundBufferType(dataSource, numSource,
				this->AudioEngineInstance().SoundVolume()));
		}

		AudioBufferPtr MakeMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds = 2)
		{
			return AudioBufferPtr(new MusicBufferType(dataSource, bufferSeconds,
				this->AudioEngineInstance().MusicVolume()));
		}

	private:
		const std::wstring name_;
	};
}

#endif			// _AUDIOFACTORY_HPP
