// DSSoundBuffer.cpp
// KlayGE DirectSound������������ ʵ���ļ�
// Ver 2.1.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.10.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>
#include <algorithm>
#include <cstring>

#include <boost/bind.hpp>

#include <KlayGE/DSound/DSAudio.hpp>

namespace
{
	// ���һ����Ƶ�������Ƿ����
	/////////////////////////////////////////////////////////////////////////////////
	bool IsSourceFree(KlayGE::DSBufferType pDSB)
	{
		if (pDSB)
		{
			DWORD status;
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
	DSSoundBuffer::DSSoundBuffer(AudioDataSourcePtr const & dataSource,
									uint32 numSource, float volume)
					: SoundBuffer(dataSource),
						sources_(numSource)
	{
		WAVEFORMATEX wfx(WaveFormatEx(dataSource));

		// ���� DirectSound ��������Ҫ��������ʹ�ý�����־��
		// ��Ϊʹ��̫�಻��Ҫ�ı�־��Ӱ��Ӳ����������
		DSBUFFERDESC dsbd;
		std::memset(&dsbd, 0, sizeof(dsbd));
		dsbd.dwSize				= sizeof(dsbd);
		dsbd.dwFlags			= DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;
		dsbd.guid3DAlgorithm	= GUID_NULL;
		dsbd.dwBufferBytes		= static_cast<uint32>(dataSource->Size());
		dsbd.lpwfxFormat		= &wfx;

		boost::shared_ptr<IDirectSound> const & dsound(static_cast<DSAudioEngine&>(Context::Instance().AudioFactoryInstance().AudioEngineInstance()).DSound());

		// DirectSoundֻ�ܲ��� PCM ���ݡ�������ʽ���ܲ��ܹ�����
		IDirectSoundBuffer* temp;
		TIF(dsound->CreateSoundBuffer(&dsbd, &temp, NULL));
		sources_[0] = DSBufferType(temp);

		// ���ƻ�������ʹ���л�����ʹ��ͬһ������
		for (SourcesIter iter = sources_.begin() + 1; iter != sources_.end(); ++ iter)
		{
			TIF(dsound->DuplicateSoundBuffer(sources_[0].get(), &temp));
			*iter = DSBufferType(temp);
		}

		// ����������
		uint8* lockedBuffer;			// ָ�򻺳����������ڴ��ָ��
		uint32 lockedBufferSize;		// �������ڴ��С
		TIF(sources_[0]->Lock(0, static_cast<DWORD>(dataSource_->Size()),
			reinterpret_cast<void**>(&lockedBuffer), &lockedBufferSize, 
			NULL, NULL, DSBLOCK_FROMWRITECURSOR));

		dataSource_->Reset();

		// ������waveFile��仺����
		std::vector<uint8> data(dataSource_->Size());
		dataSource_->Read(&data[0], data.size());

		if (data.empty())
		{
			// ���wav�հף��þ������
			std::fill_n(lockedBuffer, lockedBufferSize, 128);
		}
		else
		{
			if (data.size() <= lockedBufferSize)
			{
				std::copy(data.begin(), data.end(), lockedBuffer);

				// �����Ƶ����Դ�Ȼ�����С��������Ƶ������仺����
				std::fill_n(lockedBuffer + data.size(), lockedBufferSize - data.size(), 128);
			}
		}

		// ����������
		sources_[0]->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);

		this->Position(Vector3(0, 0, 0));
		this->Velocity(Vector3(0, 0, 0));
		this->Direction(Vector3(0, 0, 0));

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
			std::advance(iter, Random::Instance().Next(sources_.size()));
		}

		return iter;
	}

	// ����3D�������Ľӿ�
	/////////////////////////////////////////////////////////////////////////////////
	boost::shared_ptr<IDirectSound3DBuffer> DSSoundBuffer::Get3DBufferInterface(SourcesIter iter)
	{
		IDirectSound3DBuffer* ds3DBuffer;
		(*iter)->QueryInterface(IID_IDirectSound3DBuffer, reinterpret_cast<void**>(&ds3DBuffer));

		return MakeCOMPtr(ds3DBuffer);
	}

	// ������Դ
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Play(bool loop)
	{
		SourcesIter iter(this->FreeSource());

		boost::shared_ptr<IDirectSound3DBuffer> ds3DBuf(this->Get3DBufferInterface(iter));
		if (ds3DBuf)
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
		std::for_each(sources_.begin(), sources_.end(),
			boost::bind(&IDirectSoundBuffer::Stop, _1));
	}

	// ������������λ
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::DoReset()
	{
		std::for_each(sources_.begin(), sources_.end(),
			boost::bind(&IDirectSoundBuffer::SetCurrentPosition, _1, 0));
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
		long const dB(LinearGainToDB(vol));
		std::for_each(sources_.begin(), sources_.end(),
			boost::bind(&IDirectSoundBuffer::SetVolume, _1, dB));
	}

	// ��ȡ��Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 DSSoundBuffer::Position() const
	{
		return pos_;
	}

	// ������Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	void DSSoundBuffer::Position(Vector3 const & v)
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
	void DSSoundBuffer::Velocity(Vector3 const & v)
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
	void DSSoundBuffer::Direction(Vector3 const & v)
	{
		dir_ = v;
	}
}
