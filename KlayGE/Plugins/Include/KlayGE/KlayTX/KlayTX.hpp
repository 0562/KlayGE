// KlayTX.hpp
// KlayGE .klaytx������ ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////

#ifndef _KLAYTX_HPP
#define _KLAYTX_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/alloc.hpp>

#include <vector>

namespace KlayGE
{
#pragma pack(push, 1)

	struct KlayTXHeader
	{
		FourCC			id;
		U32				width;
		U32				height;
		PixelFormat		format;

		U32				offset;
	};

#pragma pack(pop)

	class KlayTX
	{
	public:
		KlayTX();

		TexturePtr Load(VFile& file, U16 numMipmaps = 0);

		U32 Width() const;
		U32 Height() const;
		PixelFormat Format() const;

	private:
		KlayTXHeader header_;
		std::vector<U8, alloc<U8> > data_;
	};
}

#endif			// _KLAYTX_HPP