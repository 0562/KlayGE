// SoundBuffer.cpp
// KlayGE ������������ ʵ���ļ�
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
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/Audio.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	SoundBuffer::SoundBuffer(const AudioDataSourcePtr& dataSource)
					: AudioBuffer(dataSource)
	{
	}

	// ��������λ
	/////////////////////////////////////////////////////////////////////////////////
	void SoundBuffer::Reset()
	{
		this->Stop();

		dataSource_->Reset();

		this->DoReset();
	}
}
