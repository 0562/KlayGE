// DSSoundBuffer.cpp
// KlayGE DirectSound������������ ʵ���ļ�
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
#include <KlayGE/Math.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/Engine.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/DSound/DSAudio.hpp>

namespace
{
	using namespace KlayGE;

	// ���һ����Ƶ�������Ƿ����
	/////////////////////////////////////////////////////////////////////////////////
	bool IsSourceFree(DSBufferType pDSB)
	{
		if (pDSB.Get() != NULL)
		{
			U32 status;
			pDSB->GetStatus(&status);
			return (0 == (status & DSBSTATUS_PLAYING));
		}

		return false;
	}
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DSSoundBuffer::DSSoundBuffer(const AudioDataSourcePtr& dataSource,
									U32 sourceNum, float volume)
					: SoundBuffer(dataSource),
						sources_(sourceNum)
	{
		WAVEFORMATEX wfx(WaveFormatEx(dataSource));

		// ���� DirectSound ��������Ҫ��������ʹ�ý�����־��
		// ��Ϊʹ��̫�಻��Ҫ�ı�־��Ӱ��Ӳ����������
		DSBUFFERDESC dsbd;
		Engine::MemoryInstance().Zero(&dsbd, sizeof(dsbd));
		dsbd.dwSize				= sizeof(dsbd);
		dsbd.dwFlags			= DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;
		dsbd.guid3DAlgorithm	= GUID_NULL;
		dsbd.dwBufferBytes		= static_cast<U32>(dataSource->Size());
		dsbd.lpwfxFormat		= &wfx;

		const COMPtr<IDirectSound>& dsound(static_cast<DSAudioEngine&>(Engine::AudioFactoryInstance().AudioEngineInstance()).DSound());

		// DirectSoundֻ�ܲ��� PCM ���ݡ�������ʽ���ܲ��ܹ�����
		IDirectSoundBuffer* temp;
		TIF(dsound->CreateSoundBuffer(&dsbd, &temp, NULL));
		sources_[0] = DSBufferType(temp);

		// ���ƻ�������ʹ���л�����ʹ��ͬһ������
		for (SourcesIter iter = sources_.begin() + 1; iter != sources_.end(); ++ iter)
		{
			TIF(dsound->DuplicateSoundBuffer(sources_[0].Get(), &temp));
			*iter = DSBufferType(temp);
		}

		// ����������
		PVOID lockedBuffer;			// ָ�򻺳����������ڴ��ָ��
		U32   lockedBufferSize;		// �������ڴ��С
		TIF(sources_[0]->Lock(0, dataSource_->Size(), &lockedBuffer, &lockedBufferSize, 
			NULL, NULL, DSBLOCK_FROMWRITECURSOR));

		dataSource_->Reset();

		// ������waveFile��仺����
		std::vector<U8, alloc<U8> > data(dataSource_->Size());
		dataSource_->Read(&data[0], data.size());

		if (0 == data.size())
		{
			// ���wav�հף��þ������
			Engine::MemoryInstance().Set(lockedBuffer, 128, lockedBufferSize);
		}
		else
		{
			if (data.size() <= lockedBufferSize)
			{
				Engine::MemoryInstance().Cpy(lockedBuffer, &data[0], data.size());

				// �����Ƶ����Դ�Ȼ�����С��������Ƶ������仺����
				Engine::MemoryInstance().Set(static_cast<U8*>(lockedBuffer) + data.size(), 
					128, lockedBufferSize - data.size());
			}
		}

		// ����������
		sources_[0]->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);

		this->Position(MakeVector(0.0f, 0.0f, 0.0f));
		this->Velocity(MakeVector(0.0f, 0.0f, 0.0f));
		this->Direction(MakeVector(0.0f, 0.0f, 0.0f));

		this->Reset();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DSSoundBuffer::~DSSoundBuffer()
	{
		//this->Stop();
		sources_.clear();
	}

	// ���ؿ��еĻ�����
	/////////////////////////////////////////////////////////////////////////////////
	DSSoundBuffer::SourcesIter DSSoundBuffer::FreeSource()
	{
		assert(!sources_.empty());

		SourcesIter iter(std::find_if(sources_.begin(), sources_.end(), IsSourceFree));

		if (iter == sources_.end())
		{
			iter = sources_.begin();
			std::advance(iter, Engine::MathInstance().Random(sources_.size()));
		}

		return iter;
	}

	// ����3D�������Ľӿ�
	/////////////////////////////////////////////////////////////////////////////////
	COMPtr<IDirectSound3DBuffer> DSSoundBuffer::Get3DBufferInterface(SourcesIter iter)
	{
		COMPtr<IDirectSound3DBuffer> ds3DBuffer;
		iter->QueryInterface<IID_IDirectSound3DBuffer>(ds3DBuffer);

		return ds3DBuffer;
	}

	// ������Դ
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Play(bool loop)
	{
		SourcesIter iter(this->FreeSource());

		COMPtr<IDirectSound3DBuffer> ds3DBuf(this->Get3DBufferInterface(iter));
		if (ds3DBuf.Get() != NULL)
		{
			ds3DBuf->SetPosition(pos_[0], pos_[1], pos_[2], DS3D_IMMEDIATE);
			ds3DBuf->SetVelocity(vel_[0], vel_[1], vel_[2], DS3D_IMMEDIATE);
			ds3DBuf->SetConeOrientation(dir_[0], dir_[1], dir_[2], DS3D_IMMEDIATE);
		}

		(*iter)->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
	}

	// ֹͣ����
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Stop()
	{
		for (SourcesIter iter = sources_.begin(); iter != sources_.end(); ++ iter)
		{
			(*iter)->Stop();
		}
	}

	// ������������λ
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::DoReset()
	{
		for (SourcesIter iter = sources_.begin(); iter != sources_.end(); ++ iter)
		{
			(*iter)->SetCurrentPosition(0);
		}
	}

	// ��黺�����Ƿ��ڲ���
	/////////////////////////////////////////////////////////////////////////////////
	bool DSSoundBuffer::IsPlaying() const
	{
		return (std::find_if(sources_.begin(), sources_.end(),
			std::not1(std::ptr_fun(IsSourceFree))) != sources_.end());
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Volume(float vol)
	{
		const long dB(LinearGainToDB(vol));
		for (SourcesIter iter = sources_.begin(); iter != sources_.end(); ++ iter)
		{
			(*iter)->SetVolume(dB);
		}
	}

	// ��ȡ��Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSSoundBuffer::Position() const
	{
		return pos_;
	}

	// ������Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Position(const Vector3& v)
	{
		pos_ = v;
	}

	// ��ȡ��Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSSoundBuffer::Velocity() const
	{
		return vel_;
	}

	// ������Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Velocity(const Vector3& v)
	{
		vel_ = v;
	}

	// ��ȡ��Դ����
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSSoundBuffer::Direction() const
	{
		return dir_;
	}

	// ������Դ����
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Direction(const Vector3& v)
	{
		dir_ = v;
	}
}
