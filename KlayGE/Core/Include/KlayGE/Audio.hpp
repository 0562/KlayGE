// Audio.hpp
// KlayGE �������� ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ������NullObject (2004.4.7)
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_HPP
#define _AUDIO_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Vector.hpp>

#include <map>

#include <KlayGE/AudioDataSource.hpp>

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// ��������������ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	class AudioBuffer
	{
	public:
		AudioBuffer(AudioDataSourcePtr const & dataSource);
		virtual ~AudioBuffer();

		static AudioBufferPtr NullObject();

		virtual void Play(bool loop = false) = 0;
		virtual void Reset() = 0;
		virtual void Stop() = 0;

		virtual void Volume(float vol) = 0;

		virtual bool IsPlaying() const = 0;
		virtual bool IsSound() const = 0;

		virtual Vector3 Position() const = 0;
		virtual void Position(Vector3 const & v) = 0;
		virtual Vector3 Velocity() const = 0;
		virtual void Velocity(Vector3 const & v) = 0;
		virtual Vector3 Direction() const = 0;
		virtual void Direction(Vector3 const & v) = 0;

	protected:
		AudioDataSourcePtr dataSource_;

		AudioFormat	format_;
		uint32_t			freq_;
	};

	// ����������
	/////////////////////////////////////////////////////////////////////////////////
	class SoundBuffer : public AudioBuffer
	{
	public:
		SoundBuffer(AudioDataSourcePtr const & dataSource);
		virtual ~SoundBuffer();

		virtual void Reset();

		bool IsSound() const;

	protected:
		virtual void DoReset() = 0;
	};

	// ���ֻ���������ʽ�ṹ
	/////////////////////////////////////////////////////////////////////////////////
	class MusicBuffer : public AudioBuffer
	{
	public:
		MusicBuffer(AudioDataSourcePtr const & dataSource);
		virtual ~MusicBuffer();

		void Play(bool loop = false);
		void Stop();
		void Reset();

		bool IsSound() const;

	protected:
		virtual void DoReset() = 0;
		virtual void DoPlay(bool loop) = 0;
		virtual void DoStop() = 0;

		// ÿ���ȡ�Ĵ���
		static uint32_t	PreSecond;
	};

	// ������Ƶ����
	/////////////////////////////////////////////////////////////////////////////////
	class AudioEngine
	{
		typedef std::map<size_t, AudioBufferPtr>	AudioBufs;
		typedef AudioBufs::iterator					AudioBufsIter;
		typedef AudioBufs::const_iterator			AudioBufsConstIter;

	public:
		AudioEngine();
		virtual ~AudioEngine();

		static AudioEnginePtr NullObject();

		virtual std::wstring const & Name() const = 0;

		virtual void AddBuffer(size_t id, AudioBufferPtr const & buffer);

		size_t NumBuffer() const;
		virtual AudioBufferPtr Buffer(size_t bufID) const;

		void Play(size_t bufID, bool loop = false);
		void Stop(size_t bufID);
		void PlayAll(bool loop = false);
		void StopAll();

		// ���úͻ�ȡ����
		void  SoundVolume(float vol);
		float SoundVolume() const;
		void  MusicVolume(float vol);
		float MusicVolume() const;

		virtual Vector3 GetListenerPos() const = 0;
		virtual void SetListenerPos(Vector3 const & v) = 0;
		virtual Vector3 GetListenerVel() const = 0;
		virtual void SetListenerVel(Vector3 const & v) = 0;
		virtual void GetListenerOri(Vector3& face, Vector3& up) const = 0;
		virtual void SetListenerOri(Vector3 const & face, Vector3 const & up) = 0;

	protected:
		AudioBufs	audioBufs_;

		float		soundVol_;
		float		musicVol_;
	};
}

#endif		// _AUDIO_HPP
