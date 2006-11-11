// Sampler.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.5.0
// ��Ȩ����(C) ������, 2005-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.5.0
// ��Ϊʹ�ýṹ�� (2006.11.5)
//
// 3.2.0
// ȥ����TextureMatrix (2006.5.9)
//
// 3.0.0
// ������TAM_Border (2005.8.30)
//
// 2.8.0
// ���ν��� (2005.7.30)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SAMPLER_HPP
#define _SAMPLER_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Math.hpp>

#include <string>
#include <boost/assert.hpp>

namespace KlayGE
{
#ifdef KLAYGE_PLATFORM_WINDOWS
#pragma pack(push, 1)
#endif
	struct Sampler
	{
		// Sampler addressing modes - default is TAM_Wrap.
		enum TexAddressingMode
		{
			// Texture wraps at values over 1.0
			TAM_Wrap,
			// Texture mirrors (flips) at joins over 1.0
			TAM_Mirror,
			// Texture clamps at 1.0
			TAM_Clamp,
			// Texture coordinates outside the range [0.0, 1.0] are set to the border color.
			TAM_Border
		};

		enum TexFilterOp
		{
			TFO_Point,
			TFO_Bilinear,
			TFO_Trilinear,
			TFO_Anisotropic
		};

		TexturePtr texture;

		Color border_clr;

		TexAddressingMode addr_mode_u : 4;
		TexAddressingMode addr_mode_v : 4;
		TexAddressingMode addr_mode_w : 4;

		TexFilterOp filter : 4;

		uint8_t anisotropy;
		uint8_t max_mip_level;
		float mip_map_lod_bias;

		Sampler();
	};
#ifdef KLAYGE_PLATFORM_WINDOWS
#pragma pack(pop)
#endif
}

#endif			// _SAMPLER_HPP
