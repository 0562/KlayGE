// MusicBuffer.cpp
// KlayGE ���ֻ������� ʵ���ļ�
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

#pragma comment(lib, "libpthread.lib") 

namespace KlayGE
{
	U32 MusicBuffer::PreSecond = 2;

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	MusicBuffer::MusicBuffer(const AudioDataSourcePtr& dataSource)
					: AudioBuffer(dataSource),
						playThread_(0)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	MusicBuffer::~MusicBuffer()
	{
	}

	// ����ʱ���¼�
	/////////////////////////////////////////////////////////////////////////////////
	void* MusicBuffer::PlayProc(void* arg)
	{
		MusicBuffer* streaming(reinterpret_cast<MusicBuffer*>(arg));

		// ���»�����
		streaming->LoopUpdateBuffer();

		return NULL;
	}

	// ��������λ�Ա��ڴ�ͷ����
	/////////////////////////////////////////////////////////////////////////////////
	void MusicBuffer::Reset()
	{
		this->Stop();

		this->DoReset();
	}

	// ������Ƶ��
	/////////////////////////////////////////////////////////////////////////////////
	void MusicBuffer::Play(bool loop)
	{
		if (this->IsPlaying())
		{
			this->Stop();
		}

		pthread_create(&playThread_, NULL, PlayProc, this);

		this->DoPlay(loop);
	}

	// ֹͣ������Ƶ��
	////////////////////////////////////////////////////////////////////////////////
	void MusicBuffer::Stop()
	{
		if (playThread_ != 0)
		{
			pthread_join(playThread_, NULL);
			playThread_ = 0;
		}

		dataSource_->Reset();

		this->DoStop();
	}
}
