// AudioFactory.hpp
// KlayGE ��Ƶ������󹤳� ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ������NullObject (2005.10.29)
//
// 2.0.0
// ���ν��� (2003.8.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIOFACTORY_HPP
#define _AUDIOFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>

#include <string>
#include <boost/utility.hpp>

namespace KlayGE
{
	class AudioFactory
	{
	public:
		virtual ~AudioFactory()
			{ }

		static AudioFactoryPtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual AudioEngine& AudioEngineInstance() = 0;
		virtual AudioBufferPtr MakeSoundBuffer(AudioDataSourcePtr const & dataSource, uint32_t numSource = 1) = 0;
		virtual AudioBufferPtr MakeMusicBuffer(AudioDataSourcePtr const & dataSource, uint32_t bufferSeconds = 2) = 0;
	};

	template <typename AudioEngineType, typename SoundBufferType, typename MusicBufferType>
	class ConcreteAudioFactory : boost::noncopyable, public AudioFactory
	{
	public:
		ConcreteAudioFactory(std::wstring const & name)
			: name_(name)
			{ }

		std::wstring const & Name() const
			{ return name_; }

		AudioEngine& AudioEngineInstance()
		{
			static AudioEngineType audioEngine;
			return audioEngine;
		}

		AudioBufferPtr MakeSoundBuffer(AudioDataSourcePtr const & dataSource, uint32_t numSource = 1)
		{
			return MakeSharedPtr<SoundBufferType>(dataSource, numSource,
				this->AudioEngineInstance().SoundVolume());
		}

		AudioBufferPtr MakeMusicBuffer(AudioDataSourcePtr const & dataSource, uint32_t bufferSeconds = 2)
		{
			return MakeSharedPtr<MusicBufferType>(dataSource, bufferSeconds,
				this->AudioEngineInstance().MusicVolume());
		}

	private:
		std::wstring const name_;
	};
}

#endif			// _AUDIOFACTORY_HPP
