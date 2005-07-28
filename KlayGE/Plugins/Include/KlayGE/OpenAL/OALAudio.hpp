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

#include <al.h>
#include <alc.h>

#include <vector>
#define NOMINMAX
#include <windows.h>

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <KlayGE/Audio.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_AudioEngine_OpenAL_d.lib")
#else
	#pragma comment(lib, "KlayGE_AudioEngine_OpenAL.lib")
#endif

namespace KlayGE
{
	ALenum Convert(AudioFormat format);
	Vector3 VecToALVec(Vector3 const & v);
	Vector3 ALVecToVec(Vector3 const & v);

	// ����������
	/////////////////////////////////////////////////////////////////////////////////
	class OALSoundBuffer : boost::noncopyable, public SoundBuffer
	{
	private:
		typedef std::vector<ALuint>				Sources_type;
		typedef Sources_type::iterator			SourcesIter;
		typedef Sources_type::const_iterator	SourcesConstIter;

	public:
		OALSoundBuffer(AudioDataSourcePtr const & dataSource, uint32_t numSource, float volume);
		~OALSoundBuffer();

		void Play(bool loop = false);
		void Stop();

		void Volume(float vol);

		bool IsPlaying() const;

		Vector3 Position() const;
		void Position(Vector3 const & v);
		Vector3 Velocity() const;
		void Velocity(Vector3 const & v);
		Vector3 Direction() const;
		void Direction(Vector3 const & v);

	private:
		void DoReset();
		SourcesIter FreeSource();

	private:
		Sources_type	sources_;
		ALuint			buffer_;

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
		OALMusicBuffer(AudioDataSourcePtr const & dataSource, uint32_t bufferSeconds, float volume);
		~OALMusicBuffer();

		void Volume(float vol);

		bool IsPlaying() const;

		Vector3 Position() const;
		void Position(Vector3 const & v);
		Vector3 Velocity() const;
		void Velocity(Vector3 const & v);
		Vector3 Direction() const;
		void Direction(Vector3 const & v);

		void LoopUpdateBuffer();

	private:
		void DoReset();
		void DoPlay(bool loop);
		void DoStop();

	private:
		boost::shared_ptr<boost::thread> playThread_;

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

		std::wstring const & Name() const;

		Vector3 GetListenerPos() const;
		void SetListenerPos(Vector3 const & v);
		Vector3 GetListenerVel() const;
		void SetListenerVel(Vector3 const & v);
		void GetListenerOri(Vector3& face, Vector3& up) const;
		void SetListenerOri(Vector3 const & face, Vector3 const & up);
	};
}

#endif		// _OALAUDIO_HPP
