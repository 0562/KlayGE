// WaveSource.hpp
// KlayGE wav����Դ ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.7.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _WAVESOURCE_HPP
#define _WAVESOURCE_HPP

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/AudioDataSource.hpp>

#pragma comment(lib, "KlayGE_AudioDataSource_OggWav.lib")

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
		ResIdentifierPtr	wavFile_;
		size_t		dataOffset_;
		size_t		size_;

		void ReadMMIO();
	};
}

#endif			// _WAVESOURCE_HPP
