// KlayTX.hpp
// KlayGE .klaytx������ ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.29)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/VFile.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>

#include <KlayGE/KlayTX/KlayTX.hpp>

namespace KlayGE
{
	// ���캯��
	//////////////////////////////////////////////////////////////////////////////////
	KlayTX::KlayTX()
	{
	}

	// ��������
	//////////////////////////////////////////////////////////////////////////////////
	TexturePtr KlayTX::Load(VFile& file, U16 numMipMaps)
	{
		file.Rewind();
		file.Read(&header_, sizeof(header_));
		Verify(header_.id == MakeFourCC<'K', 'l', 'T', 'X'>::value);

		file.Seek(header_.offset, VFile::SM_Current);

		std::vector<U8> data;
		data.resize(this->Width() * this->Height() * PixelFormatBits(this->Format()) / 8);
		file.Read(&data[0], data.size());

		TexturePtr tex(Context::Instance().RenderFactoryInstance().MakeTexture(this->Width(), this->Height(), numMipMaps,
			this->Format(), Texture::TU_Default));

		tex->CopyMemoryToTexture(&data[0], this->Format());

		return tex;
	}

	// ͼƬ���
	//////////////////////////////////////////////////////////////////////////////////
	U32 KlayTX::Width() const
	{
		return header_.width;
	}

	// ͼƬ�߶�
	//////////////////////////////////////////////////////////////////////////////////
	U32 KlayTX::Height() const
	{
		return header_.height;
	}

	// ͼƬ��ʽ
	//////////////////////////////////////////////////////////////////////////////////
	PixelFormat KlayTX::Format() const
	{
		return header_.format;
	}
}
