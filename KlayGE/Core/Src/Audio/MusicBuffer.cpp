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
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/Audio.hpp>

namespace KlayGE
{
	uint32_t MusicBuffer::PreSecond = 2;

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	MusicBuffer::MusicBuffer(AudioDataSourcePtr const & dataSource)
					: AudioBuffer(dataSource)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	MusicBuffer::~MusicBuffer()
	{
	}

	// �Ƿ�������
	/////////////////////////////////////////////////////////////////////////////////
	bool MusicBuffer::IsSound() const
	{
		return false;
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

		this->DoPlay(loop);
	}

	// ֹͣ������Ƶ��
	////////////////////////////////////////////////////////////////////////////////
	void MusicBuffer::Stop()
	{
		this->DoStop();

		dataSource_->Reset();
	}
}
