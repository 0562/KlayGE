// DSMusicBuffer.cpp
// KlayGE DirectSound���ֻ������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.10.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/Engine.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/DSound/DSAudio.hpp>

namespace KlayGE
{
	// ���캯��������һ������������ʽ���ŵĻ�����
	/////////////////////////////////////////////////////////////////////////////////
	DSMusicBuffer::DSMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds, float volume)
					: MusicBuffer(dataSource),
						writePos_(0)
	{
		WAVEFORMATEX wfx(WaveFormatEx(dataSource));
		notifySize_		= wfx.nAvgBytesPerSec / PreSecond;
		notifyCount_	= bufferSeconds * PreSecond;

		bool mono(1 == wfx.nChannels);

		const COMPtr<IDirectSound>& dsound(static_cast<DSAudioEngine&>(Engine::AudioFactoryInstance().AudioEngineInstance()).DSound());

		// ���� DirectSound ��������Ҫ��������ʹ�ý�����־��
		// ��Ϊʹ��̫�಻��Ҫ�ı�־��Ӱ��Ӳ����������
		DSBUFFERDESC dsbd;
		Engine::MemoryInstance().Zero(&dsbd, sizeof(dsbd));
		dsbd.dwSize				= sizeof(dsbd);
		dsbd.dwFlags			= DSBCAPS_CTRLPOSITIONNOTIFY;
		if (mono)
		{
			dsbd.dwFlags |= DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;
			dsbd.guid3DAlgorithm	= GUID_NULL;
		}
		dsbd.dwBufferBytes		= notifySize_ * notifyCount_;
		dsbd.lpwfxFormat		= &wfx;

		// DirectSoundֻ�ܲ���PCM���ݡ�������ʽ���ܲ��ܹ�����
		IDirectSoundBuffer* buffer;
		TIF(dsound->CreateSoundBuffer(&dsbd, &buffer, NULL));
		buffer_ = COMPtr<IDirectSoundBuffer>(buffer);

		if (mono)
		{
			buffer_.QueryInterface<IID_IDirectSound3DBuffer>(ds3DBuffer_);
		}

		notifyEvent_ = ::CreateEventW(NULL, FALSE, FALSE, NULL);

		COMPtr<IDirectSoundNotify> dsNotify;
		TIF(buffer_.QueryInterface<IID_IDirectSoundNotify>(dsNotify));

		std::vector<DSBPOSITIONNOTIFY> posNotify(notifyCount_);
		for (std::vector<DSBPOSITIONNOTIFY>::iterator iter = posNotify.begin(); iter != posNotify.end(); ++ iter)
		{
			iter->dwOffset     = (notifySize_ * std::distance(posNotify.begin(), iter)) + notifySize_ - 1;
			iter->hEventNotify = notifyEvent_;
		}

		// Tell DirectSound when to notify us. The notification will come in the from 
		// of signaled events that are handled in WinMain()
		TIF(dsNotify->SetNotificationPositions(notifyCount_, &posNotify[0]));


		this->Position(Vector3::Zero());
		this->Velocity(Vector3::Zero());
		this->Direction(Vector3::Zero());

		this->Volume(volume);

		this->Reset();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DSMusicBuffer::~DSMusicBuffer()
	{
		this->Stop();
		::CloseHandle(notifyEvent_);
	}

	// ���»�����
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::LoopUpdateBuffer()
	{
		bool finished(false);

		for (; !finished;)
		{
			if (WAIT_OBJECT_0 == ::WaitForMultipleObjects(1, &notifyEvent_, FALSE, INFINITE))
			{
				// ����������
				void* lockedBuffer;			// ָ�򻺳����������ڴ��ָ��
				U32   lockedBufferSize;		// �������ڴ��С
				TIF(buffer_->Lock(notifySize_ * writePos_, notifySize_,
					&lockedBuffer, &lockedBufferSize, NULL, NULL, 0));

				std::vector<U8, alloc<U8> > data(notifySize_);
				data.resize(dataSource_->Read(&data[0], notifySize_));

				if (data.size() > 0)
				{
					Engine::MemoryInstance().Cpy(lockedBuffer, &data[0], data.size());

					Engine::MemoryInstance().Set(static_cast<U8*>(lockedBuffer) + data.size(), 
						0, lockedBufferSize - data.size());
				}
				else
				{
					Engine::MemoryInstance().Set(lockedBuffer, 0, lockedBufferSize);
					finished = true;
					this->Stop();
				}

				// ����������
				buffer_->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);

				// �γɻ��λ�����
				++ writePos_;
				writePos_ %= notifyCount_;
			}
		}
	}

	// ��������λ�Ա��ڴ�ͷ����
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::DoReset()
	{
		this->writePos_	= 0;

		dataSource_->Reset();

		// ����������
		void* lockedBuffer;			// ָ�򻺳����������ڴ��ָ��
		U32   lockedBufferSize;		// �������ڴ��С
		TIF(buffer_->Lock(0, notifySize_ * notifyCount_,
			&lockedBuffer, &lockedBufferSize, NULL, NULL, 0));

		std::vector<U8, alloc<U8> > data(notifySize_ * notifyCount_);
		data.resize(dataSource_->Read(&data[0], notifySize_ * notifyCount_));

		if (data.size() > 0)
		{
			// �������Դ�Ȼ�����С��������Ƶ������仺����
			Engine::MemoryInstance().Cpy(lockedBuffer, &data[0], data.size());

			// ʣ�µ������ÿհ����
			Engine::MemoryInstance().Set(static_cast<U8*>(lockedBuffer) + data.size(), 
				0, lockedBufferSize - data.size());
		}
		else
		{
			// �����Ƶ���ݿհף��þ������
			Engine::MemoryInstance().Set(lockedBuffer, 0, lockedBufferSize);
		}

		// ����������
		buffer_->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);

		buffer_->SetCurrentPosition(0);
	}

	// ������Ƶ��
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::DoPlay(bool /*loop*/)
	{
		buffer_->Play(0, 0, DSBPLAY_LOOPING);
	}

	// ֹͣ������Ƶ��
	////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::DoStop()
	{
		buffer_->Stop();
	}

	// ��黺�����Ƿ��ڲ���
	/////////////////////////////////////////////////////////////////////////////////
	bool DSMusicBuffer::IsPlaying() const
	{
		if (buffer_.Get() != NULL)
		{
			U32 status;
			buffer_->GetStatus(&status);
			return ((status & DSBSTATUS_PLAYING) != 0);
		}

		return false;
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::Volume(float vol)
	{
		buffer_->SetVolume(LinearGainToDB(vol));
	}


	// ��ȡ��Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSMusicBuffer::Position() const
	{
		Vector3 ret(Vector3::Zero());

		if (ds3DBuffer_.Get() != NULL)
		{
			D3DVECTOR v;
			ds3DBuffer_->GetPosition(&v);
			ret = MakeVector(v.x, v.y, v.z);
		}

		return ret;
	}

	// ������Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::Position(const Vector3& v)
	{
		if (ds3DBuffer_.Get() != NULL)
		{
			ds3DBuffer_->SetPosition(v.x(), v.y(), v.z(), DS3D_IMMEDIATE);
		}
	}

	// ��ȡ��Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSMusicBuffer::Velocity() const
	{
		Vector3 ret(Vector3::Zero());

		if (ds3DBuffer_.Get() != NULL)
		{
			D3DVECTOR v;
			ds3DBuffer_->GetVelocity(&v);
			ret = MakeVector(v.x, v.y, v.z);
		}

		return ret;
	}

	// ������Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::Velocity(const Vector3& v)
	{
		if (ds3DBuffer_.Get() != NULL)
		{
			ds3DBuffer_->SetVelocity(v.x(), v.y(), v.z(), DS3D_IMMEDIATE);
		}
	}

	// ��ȡ��Դ����
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSMusicBuffer::Direction() const
	{
		Vector3 ret(Vector3::Zero());

		if (ds3DBuffer_.Get() != NULL)
		{
			D3DVECTOR v;
			ds3DBuffer_->GetConeOrientation(&v);
			ret = MakeVector(v.x, v.y, v.z);
		}

		return ret;
	}

	// ������Դ����
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::Direction(const Vector3& v)
	{
		if (ds3DBuffer_.Get() != NULL)
		{
			ds3DBuffer_->SetConeOrientation(v.x(), v.y(), v.z(), DS3D_IMMEDIATE);
		}
	}
}
