// OALAudio.hpp
// KlayGE OpenAL�������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OALAUDIO_HPP
#define _OALAUDIO_HPP

#include <KlayGE/PreDeclare.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include <pthread.h>

#include <vector>
#include <windows.h>

#include <boost/utility.hpp>

#include <KlayGE/Audio.hpp>

#pragma comment(lib, "KlayGE_AudioEngine_OpenAL.lib")

namespace KlayGE
{
	ALenum Convert(AudioFormat format);
	Vector3 VecToALVec(const Vector3& v);
	Vector3 ALVecToVec(const Vector3& v);

	// ����������
	/////////////////////////////////////////////////////////////////////////////////
	class OALSoundBuffer : boost::noncopyable, public SoundBuffer
	{
	private:
		typedef std::vector<ALuint>			Sources;
		typedef Sources::iterator			SourcesIter;
		typedef Sources::const_iterator		SourcesConstIter;

	public:
		OALSoundBuffer(const AudioDataSourcePtr& dataSource, U32 numSource, float volume);
		~OALSoundBuffer();

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
		void DoReset();
		SourcesIter FreeSource();

	private:
		Sources		sources_;
		ALuint		buffer_;

		Vector3		pos_;
		Vector3		vel_;
		Vector3		dir_;
	};

	// ���ֻ�����
	/////////////////////////////////////////////////////////////////////////////////
	class OALMusicBuffer : boost::noncopyable, public MusicBuffer
	{
	private:
		typedef std::vector<ALuint>		Buffers;
		typedef Buffers::iterator		BuffersIter;
		typedef Buffers::const_iterator	BuffersConstIter;

	public:
		OALMusicBuffer(const AudioDataSourcePtr& dataSource, U32 bufferSeconds, float volume);
		~OALMusicBuffer();

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
		static void* PlayProc(void* arg);
		pthread_t	playThread_;

	private:
		ALuint		source_;
		Buffers		bufferQueue_;

		bool		loop_;
	};

	// ������Ƶ����
	/////////////////////////////////////////////////////////////////////////////////
	class OALAudioEngine : boost::noncopyable, public AudioEngine
	{
	public:
		OALAudioEngine();
		~OALAudioEngine();

		const std::wstring& Name() const;

		Vector3 GetListenerPos() const;
		void SetListenerPos(const Vector3& v);
		Vector3 GetListenerVel() const;
		void SetListenerVel(const Vector3& v);
		void GetListenerOri(Vector3& face, Vector3& up) const;
		void SetListenerOri(const Vector3& face, const Vector3& up);
	};
}

#endif		// _OALAUDIO_HPP
