// AudioFactory.cpp
// KlayGE ��Ƶ������󹤳� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.klayge.org
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KFL/Util.hpp>
#include <KlayGE/Audio.hpp>
#include <KlayGE/AudioFactory.hpp>

namespace KlayGE
{
	class NullAudioFactory : public AudioFactory
	{
	public:
		std::wstring const & Name() const
		{
			static std::wstring const name(L"Null Audio Factory");
			return name;
		}

		AudioBufferPtr MakeSoundBuffer(AudioDataSourcePtr const & /*dataSource*/, uint32_t /*numSource*/)
		{
			return AudioBuffer::NullObject();
		}

		AudioBufferPtr MakeMusicBuffer(AudioDataSourcePtr const & /*dataSource*/, uint32_t /*bufferSeconds*/)
		{
			return AudioBuffer::NullObject();
		}

	private:
		virtual AudioEnginePtr MakeAudioEngine() KLAYGE_OVERRIDE
		{
			return AudioEngine::NullObject();
		}

		virtual void DoSuspend() KLAYGE_OVERRIDE
		{
		}
		virtual void DoResume() KLAYGE_OVERRIDE
		{
		}
	};

	AudioFactoryPtr AudioFactory::NullObject()
	{
		static AudioFactoryPtr obj = MakeSharedPtr<NullAudioFactory>();
		return obj;
	}

	AudioEngine& AudioFactory::AudioEngineInstance()
	{
		if (!ae_)
		{
			ae_ = this->MakeAudioEngine();
		}

		return *ae_;
	}

	void AudioFactory::Suspend()
	{
		if (ae_)
		{
			ae_->Suspend();
		}
		this->DoSuspend();
	}

	void AudioFactory::Resume()
	{
		this->DoResume();
		if (ae_)
		{
			ae_->Resume();
		}
	}
}
