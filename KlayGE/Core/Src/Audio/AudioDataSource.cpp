// AudioBuffer.cpp
// KlayGE �������� ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ���ν��� (2004.4.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <KlayGE/AudioDataSource.hpp>

namespace KlayGE
{
	class NullAudioDataSource : public AudioDataSource
	{
	public:
		AudioFormat Format() const
			{ return AF_Unknown; }
		U32 Freq() const
			{ return 0; }

		size_t Size()
			{ return 0; }

		size_t Read(U8* data, size_t size)
			{ return 0; }
		void Reset()
			{ }
	};

	AudioDataSource::~AudioDataSource()
	{
	}

	AudioDataSourcePtr AudioDataSource::NullObject()
	{
		static AudioDataSourcePtr obj(new NullAudioDataSource);
		return obj;
	}

	AudioFormat AudioDataSource::Format() const
	{
		return this->format_;
	}

	U32 AudioDataSource::Freq() const
	{
		return this->freq_;
	}
}
