// HDRPostProcess.hpp
// KlayGE HDR���ڴ����� ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ���ν��� (2006.8.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _HDRPOSTPROCESS_HPP
#define _HDRPOSTPROCESS_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

#include <vector>

#include <KlayGE/Timer.hpp>
#include <KlayGE/PostProcess.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API SumLumPostProcess : public PostProcess
	{
	public:
		explicit SumLumPostProcess(RenderTechniquePtr const & tech);
		virtual ~SumLumPostProcess();

		void InputPin(uint32_t index, TexturePtr const & tex, bool flipping);

	private:
		void GetSampleOffsets4x4(uint32_t width, uint32_t height);

	protected:
		std::vector<float4> tex_coord_offset_;

		RenderEffectParameterPtr tex_coord_offset_ep_;
	};

	class KLAYGE_CORE_API SumLumLogPostProcess : public SumLumPostProcess
	{
	public:
		SumLumLogPostProcess();
	};

	class KLAYGE_CORE_API SumLumLogPostProcessCS : public SumLumPostProcess
	{
	public:
		SumLumLogPostProcessCS();
		void Apply();

		void InputPin(uint32_t index, TexturePtr const & tex, bool flipping);
		void DestinateSize(uint32_t width, uint32_t height);

		GraphicsBufferPtr const & SumLumBuff() const
		{
			return buff_;
		}

	private:
		GraphicsBufferPtr buff_;
	};

	class KLAYGE_CORE_API SumLumIterativePostProcess : public SumLumPostProcess
	{
	public:
		SumLumIterativePostProcess();
	};

	class KLAYGE_CORE_API AdaptedLumPostProcess : public PostProcess
	{
	public:
		AdaptedLumPostProcess();

		void Apply();
		void OnRenderBegin();

		TexturePtr AdaptedLum() const;

	private:
		FrameBufferPtr fb_[2];
		TexturePtr adapted_textures_[2];
		bool last_index_;

		Timer timer_;

		RenderEffectParameterPtr last_lum_tex_ep_;
		RenderEffectParameterPtr frame_delta_ep_;
	};

	class KLAYGE_CORE_API AdaptedLumPostProcessCS : public PostProcess
	{
	public:
		AdaptedLumPostProcessCS();

		void Apply();
		void OnRenderBegin();

		void SetLumBuff(GraphicsBufferPtr const & lum_buff);

		GraphicsBufferPtr const & AdaptedLum() const;

	private:
		GraphicsBufferPtr adapted_buff_;

		Timer timer_;

		RenderEffectParameterPtr frame_delta_ep_;
	};

	class KLAYGE_CORE_API ToneMappingPostProcess : public PostProcess
	{
	public:
		explicit ToneMappingPostProcess(bool blue_shift);

		void SetLumBuff(GraphicsBufferPtr const & lum_buff);

	private:
		RenderEffectParameterPtr lum_buff_ep_;
	};


	class KLAYGE_CORE_API HDRPostProcess : public PostProcess
	{
		enum
		{
			NUM_TONEMAP_TEXTURES = 3
		};

	public:
		HDRPostProcess(bool bright_pass, bool blur_shift);

		void InputPin(uint32_t index, TexturePtr const & tex, bool flipping);
		void Destinate(FrameBufferPtr const & fb);
		void Apply();

	private:
		TexturePtr downsample_tex_;
		TexturePtr blur_tex_;
		std::vector<TexturePtr> lum_texs_;

		PostProcessPtr downsampler_;
		PostProcessPtr blur_;
		PostProcessPtr sum_lums_1st_;
		std::vector<PostProcessPtr> sum_lums_;
		PostProcessPtr adapted_lum_;
		PostProcessPtr tone_mapping_;

		bool cs_support_;
	};
}

#endif		// _HDRPOSTPROCESS_HPP
