// WaveSource.hpp
// KlayGE wav����Դ ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _WAVESOURCE_HPP
#define _WAVESOURCE_HPP

#include <KlayGE/PreDeclare.hpp>

#include <istream>

#include <KlayGE/AudioDataSource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_AudioDataSource_OggWav_d.lib")
#else
	#pragma comment(lib, "KlayGE_AudioDataSource_OggWav.lib")
#endif

namespace KlayGE
{
	class WaveSource : public AudioDataSource
	{
	public:
		explicit WaveSource(ResIdentifierPtr const & file);

		size_t Size();

		size_t Read(void* data, size_t size);
		void Reset();

	private:
		ResIdentifierPtr		wavFile_;
		std::istream::off_type	dataOffset_;
		size_t					size_;

		void ReadMMIO();
	};
}

#endif			// _WAVESOURCE_HPP
