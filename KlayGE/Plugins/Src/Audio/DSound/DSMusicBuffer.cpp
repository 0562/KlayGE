// DSMusicBuffer.cpp
// KlayGE DirectSound���ֻ������� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ����timeSetEventʵ�� (2004.3.28)
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

#pragma comment(lib, "winmm.lib")

namespace KlayGE
{
	// ���캯��������һ������������ʽ���ŵĻ�����
	/////////////////////////////////////////////////////////////////////////////////
	DSMusicBuffer::DSMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds, float volume)
					: MusicBuffer(dataSource),
						writePos_(0)
	{
		WAVEFORMATEX wfx(WaveFormatEx(dataSource));
		fillSize_	= wfx.nAvgBytesPerSec / PreSecond;
		fillCount_	= bufferSeconds * PreSecond;

		bool mono(1 == wfx.nChannels);

		const COMPtr<IDirectSound>& dsound(static_cast<DSAudioEngine&>(Engine::AudioFactoryInstance().AudioEngineInstance()).DSound());

		// ���� DirectSound ��������Ҫ��������ʹ�ý�����־��
		// ��Ϊʹ��̫�಻��Ҫ�ı�־��Ӱ��Ӳ����������
		DSBUFFERDESC dsbd;
		MemoryLib::Zero(&dsbd, sizeof(dsbd));
		dsbd.dwSize				= sizeof(dsbd);
		if (mono)
		{
			dsbd.dwFlags |= DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;
			dsbd.guid3DAlgorithm	= GUID_NULL;
		}
		dsbd.dwBufferBytes		= fillSize_ * fillCount_;
		dsbd.lpwfxFormat		= &wfx;

		// DirectSoundֻ�ܲ���PCM���ݡ�������ʽ���ܲ��ܹ�����
		IDirectSoundBuffer* buffer;
		TIF(dsound->CreateSoundBuffer(&dsbd, &buffer, NULL));
		buffer_ = COMPtr<IDirectSoundBuffer>(buffer);

		if (mono)
		{
			buffer_.QueryInterface<IID_IDirectSound3DBuffer>(ds3DBuffer_);
		}

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
	}

	// ���»�����
	/////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::TimerProc(::UINT timerID, ::UINT /*uMsg*/,
										DWORD_PTR dwUser, DWORD_PTR /*dw1*/, DWORD_PTR /*dw2*/)
	{
		DSMusicBuffer* buffer(reinterpret_cast<DSMusicBuffer*>(dwUser));

		if (timerID != buffer->timerID_)
		{
			return;
		}

		buffer->FillBuffer();
	}

	void DSMusicBuffer::FillBuffer()
	{
		// ����������
		void* lockedBuffer;			// ָ�򻺳����������ڴ��ָ��
		U32   lockedBufferSize;		// �������ڴ��С
		TIF(buffer_->Lock(fillSize_ * writePos_, fillSize_,
			&lockedBuffer, &lockedBufferSize, NULL, NULL, 0));

		std::vector<U8> data(fillSize_);
		data.resize(dataSource_->Read(&data[0], fillSize_));

		if (data.size() > 0)
		{
			MemoryLib::Copy(lockedBuffer, &data[0], data.size());

			MemoryLib::Set(static_cast<U8*>(lockedBuffer) + data.size(), 
				0, lockedBufferSize - data.size());
		}
		else
		{
			MemoryLib::Set(lockedBuffer, 0, lockedBufferSize);
			this->Stop();
		}

		// ����������
		buffer_->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);

		// �γɻ��λ�����
		++ writePos_;
		writePos_ %= fillCount_;
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
		TIF(buffer_->Lock(0, fillSize_ * fillCount_,
			&lockedBuffer, &lockedBufferSize, NULL, NULL, 0));

		std::vector<U8> data(fillSize_ * fillCount_);
		data.resize(dataSource_->Read(&data[0], fillSize_ * fillCount_));

		if (data.size() > 0)
		{
			// �������Դ�Ȼ�����С��������Ƶ������仺����
			MemoryLib::Copy(lockedBuffer, &data[0], data.size());

			// ʣ�µ������ÿհ����
			MemoryLib::Set(static_cast<U8*>(lockedBuffer) + data.size(), 
				0, lockedBufferSize - data.size());
		}
		else
		{
			// �����Ƶ���ݿհף��þ������
			MemoryLib::Set(lockedBuffer, 0, lockedBufferSize);
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

		timerID_ = timeSetEvent(1000 / this->PreSecond, 0, TimerProc,
								reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC);
	}

	// ֹͣ������Ƶ��
	////////////////////////////////////////////////////////////////////////////////
	void DSMusicBuffer::DoStop()
	{
		timeKillEvent(timerID_);

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
