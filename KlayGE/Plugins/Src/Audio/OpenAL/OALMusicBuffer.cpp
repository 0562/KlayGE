// OALMusicBuffer.cpp
// KlayGE OpenAL���ֻ������� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ������ѭ�����Ź��� (2004.3.22)
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/AudioDataSource.hpp>

#include <cassert>

#include <KlayGE/OpenAL/OALAudio.hpp>

const size_t READSIZE(88200);

namespace KlayGE
{
	// ���캯��������һ������������ʽ���ŵĻ�����
	/////////////////////////////////////////////////////////////////////////////////
	OALMusicBuffer::OALMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds, float volume)
							: MusicBuffer(dataSource),
								bufferQueue_(bufferSeconds * PreSecond),
								playThread_(0)
	{
		alGenBuffers(static_cast<ALsizei>(bufferQueue_.size()), &bufferQueue_[0]);

		alGenSources(1, &source_);
		alSourcef(source_, AL_PITCH, 1);

		this->Position(Vector3(0, 0, 0.1f));
		this->Velocity(Vector3(0, 0, 0));
		this->Direction(Vector3(0, 0, 0));

		this->Volume(volume);

		this->Reset();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	OALMusicBuffer::~OALMusicBuffer()
	{
		this->Stop();

		alDeleteBuffers(static_cast<ALsizei>(bufferQueue_.size()), &bufferQueue_[0]);
		alDeleteSources(1, &source_);
	}
	
	// ����ʱ���¼�
	/////////////////////////////////////////////////////////////////////////////////
	void* OALMusicBuffer::PlayProc(void* arg)
	{
		OALMusicBuffer* streaming(reinterpret_cast<OALMusicBuffer*>(arg));

		// ���»�����
		streaming->LoopUpdateBuffer();

		return NULL;
	}

	// ���»�����
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::LoopUpdateBuffer()
	{
		ALint processed;
		bool finishedPlaying(false);
		size_t buffersInQueue(bufferQueue_.size());
		ALuint buf;

		while (!finishedPlaying)
		{
			alGetSourcei(source_, AL_BUFFERS_PROCESSED, &processed);
			if (processed > 0)
			{
				while (processed != 0)
				{
					-- processed;

					alSourceUnqueueBuffers(source_, 1, &buf);

					std::vector<U8> data(READSIZE);
					data.resize(dataSource_->Read(&data[0], data.size()));
					if (data.size() > 0)
					{
						alBufferData(buf, Convert(format_), &data[0],
							static_cast<ALsizei>(data.size()), freq_);
						alSourceQueueBuffers(source_, 1, &buf);
					}
					else
					{
						-- buffersInQueue;

						if (0 == buffersInQueue)
						{
							if (loop_)
							{
								this->Reset();
								this->Play(true);
							}
							else
							{
								finishedPlaying = true;
								break;
							}
						}
					}
				}
			}
			else
			{
				Sleep(500 / this->PreSecond);
			}
		}
	}

	// ��������λ�Ա��ڴ�ͷ����
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::DoReset()
	{
		alSourceUnqueueBuffers(source_, static_cast<ALsizei>(bufferQueue_.size()), &bufferQueue_[0]);

		const ALenum format(Convert(format_));
		std::vector<U8> data(READSIZE);

		// ÿ����������װ1 / PreSecond�������
		for (BuffersIter iter = bufferQueue_.begin(); iter != bufferQueue_.end(); ++ iter)
		{
			dataSource_->Read(&data[0], data.size());
			alBufferData(*iter, format, &data[0], static_cast<ALuint>(data.size()), freq_);
		}

		alSourceQueueBuffers(source_, static_cast<ALsizei>(bufferQueue_.size()), &bufferQueue_[0]);

		alSourceRewindv(1, &source_);
	}

	// ������Ƶ��
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::DoPlay(bool loop)
	{
		pthread_create(&playThread_, NULL, PlayProc, this);

		loop_ = loop;

		alSourcei(source_, AL_LOOPING, false);
		alSourcePlay(source_);
	}

	// ֹͣ������Ƶ��
	////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::DoStop()
	{
		if (playThread_ != 0)
		{
			pthread_join(playThread_, NULL);
			playThread_ = 0;
		}

		alSourceStopv(1, &source_);
	}

	// ��黺�����Ƿ��ڲ���
	/////////////////////////////////////////////////////////////////////////////////
	bool OALMusicBuffer::IsPlaying() const
	{
		ALint value;
		alGetSourcei(source_, AL_SOURCE_STATE, &value);

		return (AL_PLAYING == (value & AL_PLAYING));
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::Volume(float vol)
	{
		alSourcef(source_, AL_GAIN, vol);
	}

	// ��ȡ��Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::Position() const
	{
		float pos[3];
		alGetSourcefv(source_, AL_POSITION, pos);
		return ALVecToVec(Vector3(pos[0], pos[1], pos[2]));
	}

	// ������Դλ��
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::Position(const Vector3& v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(source_, AL_POSITION, &alv.x());
	}

	// ��ȡ��Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::Velocity() const
	{
		float vel[3];
		alGetSourcefv(source_, AL_VELOCITY, vel);
		return ALVecToVec(Vector3(vel[0], vel[1], vel[2]));
	}

	// ������Դ�ٶ�
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::Velocity(const Vector3& v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(source_, AL_VELOCITY, &alv.x());
	}

	// ��ȡ��Դ����
	/////////////////////////////////////////////////////////////////////////////////
	Vector3 OALMusicBuffer::Direction() const
	{
		float dir[3];
		alGetSourcefv(source_, AL_DIRECTION, dir);
		return ALVecToVec(Vector3(dir[0], dir[1], dir[2]));
	}

	// ������Դ����
	/////////////////////////////////////////////////////////////////////////////////
	void OALMusicBuffer::Direction(const Vector3& v)
	{
		Vector3 alv(VecToALVec(v));
		alSourcefv(source_, AL_DIRECTION, &alv.x());
	}
}
