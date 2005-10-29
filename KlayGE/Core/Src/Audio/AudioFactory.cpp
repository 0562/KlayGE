// AudioFactory.cpp
// KlayGE ��Ƶ������󹤳� ʵ���ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 3.1.0
// ���ν��� (2005.10.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
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

		AudioEngine& AudioEngineInstance()
		{
			return *AudioEngine::NullObject();
		}

		AudioBufferPtr MakeSoundBuffer(AudioDataSourcePtr const & dataSource, uint32_t numSource)
		{
			return AudioBuffer::NullObject();
		}

		AudioBufferPtr MakeMusicBuffer(AudioDataSourcePtr const & dataSource, uint32_t bufferSeconds)
		{
			return AudioBuffer::NullObject();
		}
	};

	AudioFactoryPtr AudioFactory::NullObject()
	{
		static AudioFactoryPtr obj(new NullAudioFactory);
		return obj;
	}
}
