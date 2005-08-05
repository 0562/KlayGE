// Sampler.cpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.30)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <KlayGE/Sampler.hpp>

namespace KlayGE
{
	Sampler::Sampler()
		: border_clr_(0, 0, 0, 0),
			addr_mode_u_(TAM_Wrap), addr_mode_v_(TAM_Wrap), addr_mode_w_(TAM_Wrap),
			filter_(TFO_Point),
			anisotropy_(0),
			max_mip_level_(1),
			mip_map_lod_bias_(0),
			mat_(Matrix4::Identity())
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::SetTexture(TexturePtr tex)
	{
		tex_ = tex;
	}

	// ��ȡ����
	/////////////////////////////////////////////////////////////////////////////////
	TexturePtr Sampler::GetTexture() const
	{
		return tex_;
	}

	// ���ñ߽���ɫ
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::BorderColor(Color const & clr)
	{
		border_clr_ = clr;
	}
	
	// ��ȡ�߽���ɫ
	/////////////////////////////////////////////////////////////////////////////////
	Color const & Sampler::BorderColor() const
	{
		return border_clr_;
	}

	// ��������Ѱַģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::AddressingMode(TexAddressingType type, TexAddressingMode tam)
	{
		switch (type)
		{
		case TAT_Addr_U:
			addr_mode_u_ = tam;
			break;

		case TAT_Addr_V:
			addr_mode_v_ = tam;
			break;

		case TAT_Addr_W:
			addr_mode_w_ = tam;
			break;
		}
	}

	// ��ȡ����Ѱַģʽ
	/////////////////////////////////////////////////////////////////////////////////
	Sampler::TexAddressingMode Sampler::AddressingMode(TexAddressingType type) const
	{
		switch (type)
		{
		case TAT_Addr_U:
			return addr_mode_u_;

		case TAT_Addr_V:
			return addr_mode_v_;

		case TAT_Addr_W:
			return addr_mode_w_;

		default:
			BOOST_ASSERT(false);
			return addr_mode_u_;
		}
	}
	
	// �����������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::Filtering(TexFilterOp op)
	{
		filter_ = op;
	}

	// ��ȡ�������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	Sampler::TexFilterOp Sampler::Filtering() const
	{
		return filter_;
	}

	// �����������Թ���
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::Anisotropy(uint32_t maxAnisotropy)
	{
		anisotropy_ = maxAnisotropy;
	}

	// ��ȡ�������Թ���
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t Sampler::Anisotropy() const
	{
		return anisotropy_;
	}

	// ��������mip�ȼ�
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::MaxMipLevel(uint32_t level)
	{
		max_mip_level_ = level;
	}

	// ��ȡ����mip�ȼ�
	/////////////////////////////////////////////////////////////////////////////////
	uint32_t Sampler::MaxMipLevel() const
	{
		return max_mip_level_;
	}

	// ����mip mapƫ����
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::MipMapLodBias(float bias)
	{
		mip_map_lod_bias_ = bias;
	}

	// ��ȡmip mapƫ����
	/////////////////////////////////////////////////////////////////////////////////
	float Sampler::MipMapLodBias() const
	{
		return mip_map_lod_bias_;
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	void Sampler::TextureMatrix(Matrix4 const & mat)
	{
		mat_ = mat;
	}
	
	// ��ȡ�������
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 const & Sampler::TextureMatrix() const
	{
		return mat_;
	}
}
