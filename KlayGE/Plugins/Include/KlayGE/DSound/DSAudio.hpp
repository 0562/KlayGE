// DSAudio.hpp
// KlayGE DirectSound8�������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.10.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DSAUDIO_HPP
#define _DSAUDIO_HPP

#include <KlayGE/PreDeclare.hpp>

#include <vector>
#include <windows.h>
#include <dsound.h>

#include <boost/utility.hpp>

#include <KlayGE/COMPtr.hpp>
#include <KlayGE/Audio.hpp>

#pragma comment(lib, "KlayGE_AudioEngine_DSound.lib")

namespace KlayGE
{
	typedef COMPtr<IDirectSoundBuffer> DSBufferType;

	WAVEFORMATEX WaveFormatEx(const AudioDataSourcePtr& dataSource);
	long LinearGainToDB(float vol);

	// ����������
	/////////////////////////////////////////////////////////////////////////////////
	class DSSoundBuffer : boost::noncopyable, public SoundBuffer
	{
	private:
		typedef std::vector<DSBufferType>	Sources;
		typedef Sources::iterator			SourcesIter;
		typedef Sources::const_iterator		SourcesConstIter;

	public:
		DSSoundBuffer(const AudioDataSourcePtr& dataSource, U32 numSource, float volume);
		~DSSoundBuffer();

		void Play(bool loop = false);
		void Stop();

		void Volume(float vol);

		bool IsPlaying() const;

		Vector3 Position() const;
		void Position(const Vector3& v);
		Vector3 Velocity() const;
		void Velocity(const Vector3& v);
		Vector3 Direction() const;
		void Direction(const Vector3& v);

	private:
		COMPtr<IDirectSound3DBuffer> Get3DBufferInterface(SourcesIter iter);

		void DoReset();
		SourcesIter FreeSource();

	private:
		Sources		sources_;

		Vector3		pos_;
		Vector3		vel_;
		Vector3		dir_;
	};

	// ���ֻ�����
	/////////////////////////////////////////////////////////////////////////////////
	class DSMusicBuffer : boost::noncopyable, public MusicBuffer
	{
	public:
		DSMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds, float volume);
		~DSMusicBuffer();

		void Volume(float vol);

		bool IsPlaying() const;

		Vector3 Position() const;
		void Position(const Vector3& v);
		Vector3 Velocity() const;
		void Velocity(const Vector3& v);
		Vector3 Direction() const;
		void Direction(const Vector3& v);

	private:
		void LoopUpdateBuffer();

		void DoReset();
		void DoPlay(bool loop);
		void DoStop();

	private:
		DSBufferType	buffer_;
		U32				fillSize_;
		U32				fillCount_;
		U32				writePos_;

		COMPtr<IDirectSound3DBuffer> ds3DBuffer_;

		static void WINAPI TimerProc(UINT uTimerID, UINT uMsg,
			DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
		UINT timerID_;

		void FillBuffer();
	};

	// ������Ƶ����
	/////////////////////////////////////////////////////////////////////////////////
	class DSAudioEngine : boost::noncopyable, public AudioEngine
	{
	public:
		DSAudioEngine();
		~DSAudioEngine();

		const COMPtr<IDirectSound>& DSound() const
			{ return dsound_; }

		const std::wstring& Name() const;

		Vector3 GetListenerPos() const;
		void SetListenerPos(const Vector3& v);
		Vector3 GetListenerVel() const;
		void SetListenerVel(const Vector3& v);
		void GetListenerOri(Vector3& face, Vector3& up) const;
		void SetListenerOri(const Vector3& face, const Vector3& up);

	private:
		COMPtr<IDirectSound>			dsound_;
		COMPtr<IDirectSound3DListener>	ds3dListener_;
	};
}

#endif		// _DS8AUDIO_HPP
