// Audio.hpp
// KlayGE �������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_HPP
#define _AUDIO_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/MathTypes.hpp>

#include <pthread.h>
#include <map>

#include <KlayGE/AudioDataSource.hpp>
#include <KlayGE/MgrBase.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ��������������ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	class AudioBuffer
	{
	public:
		virtual void Play(bool loop = false) = 0;
		virtual void Reset() = 0;
		virtual void Stop() = 0;

		virtual void Volume(float vol) = 0;

		virtual bool IsPlaying() const = 0;
		virtual bool IsSound() const = 0;

		virtual Vector3 Position() const = 0;
		virtual void Position(const Vector3& v) = 0;
		virtual Vector3 Velocity() const = 0;
		virtual void Velocity(const Vector3& v) = 0;
		virtual Vector3 Direction() const = 0;
		virtual void Direction(const Vector3& v) = 0;

	public:
		virtual ~AudioBuffer()
			{ }

	protected:
		AudioBuffer(const AudioDataSourcePtr& dataSource)
			: dataSource_(dataSource),
				format_(dataSource->Format()),
				freq_(dataSource->Freq())
			{ }

	protected:
		AudioDataSourcePtr dataSource_;

		AudioFormat	format_;
		U32			freq_;
	};

	// ����������
	/////////////////////////////////////////////////////////////////////////////////
	class SoundBuffer : public AudioBuffer
	{
	public:
		virtual void Reset();

		bool IsSound() const
			{ return true; }

	public:
		SoundBuffer(const AudioDataSourcePtr& dataSource);
		virtual ~SoundBuffer()
			{ }

	protected:
		virtual void DoReset() = 0;
	};

	// ���ֻ���������ʽ�ṹ
	/////////////////////////////////////////////////////////////////////////////////
	class MusicBuffer : public AudioBuffer
	{
	public:
		void Play(bool loop = false);
		void Stop();
		void Reset();

		bool IsSound() const
			{ return false; }

	public:
		MusicBuffer(const AudioDataSourcePtr& dataSource);
		virtual ~MusicBuffer();

	protected:
		virtual void LoopUpdateBuffer() = 0;

		virtual void DoReset() = 0;
		virtual void DoPlay(bool loop) = 0;
		virtual void DoStop() = 0;

		static void* PlayProc(void* arg);

	protected:
		pthread_t	playThread_;

		// ÿ���ȡ�Ĵ���
		static U32	PreSecond;
	};

	// ������Ƶ����
	/////////////////////////////////////////////////////////////////////////////////
	class AudioEngine
	{
		typedef ManagerBase<AudioBufferPtr>		AudioBufs;
		typedef AudioBufs::iterator				AudioBufsIter;
		typedef AudioBufs::const_iterator		AudioBufsConstIter;

	public:
		AudioEngine();
		virtual ~AudioEngine()
			{ }

		virtual const WString& Name() const = 0;
		virtual void CustomAttribute(const String& name, void* pData)
			{ }

		void AddBuffer(size_t id, const AudioBufferPtr& buffer);

		size_t BufferNum() const;
		AudioBufferPtr& Buffer(size_t bufID);
		const AudioBufferPtr& Buffer(size_t bufID) const;

		void Play(size_t bufID, bool loop = false)
			{ this->Buffer(bufID)->Play(loop); }
		void Stop(size_t bufID)
			{ this->Buffer(bufID)->Stop(); }
		void PlayAll(bool loop = false);
		void StopAll();

		// ���úͻ�ȡ����
		void  SoundVolume(float vol);
		float SoundVolume() const;
		void  MusicVolume(float vol);
		float MusicVolume() const;

		virtual Vector3 ListenerPos() const = 0;
		virtual void ListenerPos(const Vector3& v) = 0;
		virtual Vector3 ListenerVel() const = 0;
		virtual void ListenerVel(const Vector3& v) = 0;
		virtual void ListenerOri(Vector3& face, Vector3& up) const = 0;
		virtual void ListenerOri(const Vector3& face, const Vector3& up) = 0;

	protected:
		AudioBufs	audioBufs_;

		float		soundVol_;
		float		musicVol_;
	};
}

#endif		// _AUDIO_HPP