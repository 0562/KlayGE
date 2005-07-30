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
	void Sampler::Filtering(TexFilterType type, TexFilterOp op)
	{
		switch (type)
		{
		case TFT_Min:
			min_filter_ = op;
			break;

		case TFT_Mag:
			mag_filter_ = op;
			break;

		case TFT_Mip:
			mip_filter_ = op;
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}
	}

	// ��ȡ�������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	Sampler::TexFilterOp Sampler::Filtering(TexFilterType type) const
	{
		switch (type)
		{
		case TFT_Min:
			return min_filter_;

		case TFT_Mag:
			return mag_filter_;

		case TFT_Mip:
			return mip_filter_;

		default:
			BOOST_ASSERT(false);
			return min_filter_;
		}
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
