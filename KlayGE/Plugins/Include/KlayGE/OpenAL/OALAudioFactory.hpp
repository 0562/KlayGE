// OALAudioFactory.hpp
// KlayGE OpenAL����������󹤳��� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OALAUDIOFACTORY_HPP
#define _OALAUDIOFACTORY_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/OpenAL/OALAudio.hpp>

#pragma comment(lib, "KlayGE_AudioEngine_OpenAL.lib")

namespace KlayGE
{
	class OALAudioFactory : public AudioFactory
	{
	public:
		const WString& Name() const;

		AudioEngine& AudioEngineInstance();

		AudioBufferPtr MakeSoundBuffer(const AudioDataSourcePtr& dataSource, U32 sourceNum = 1);
		AudioBufferPtr MakeMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds = 2);
	};
}

#endif			// _OALAUDIOFACTORY_HPP