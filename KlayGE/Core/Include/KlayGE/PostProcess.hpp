// PostProcess.hpp
// KlayGE ���ڴ����� ͷ�ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2006-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ������BlurPostProcess (2007.3.24)
//
// 3.5.0
// ������GammaCorrectionProcess (2007.1.22)
//
// 3.3.0
// ���ν��� (2006.6.23)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _POSTPROCESS_HPP
#define _POSTPROCESS_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderableHelper.hpp>

namespace KlayGE
{
	class PostProcess : public RenderableHelper
	{
	public:
		explicit PostProcess(RenderTechniquePtr tech);
		virtual ~PostProcess()
		{
		}

		virtual void Source(TexturePtr const & tex, bool flipping);
		virtual void Destinate(RenderTargetPtr const & rt);

		virtual void Apply();

		virtual void OnRenderBegin();

	protected:
		TexturePtr src_texture_;
		bool flipping_;

		RenderTargetPtr render_target_;

		GraphicsBufferPtr pos_vb_;
	};

	class GammaCorrectionProcess : public PostProcess
	{
	public:
		explicit GammaCorrectionProcess();

		void Gamma(float gamma);
	};
	
	class SeparableBlurPostProcess : public PostProcess
	{
	public:
		SeparableBlurPostProcess(std::string const & tech, int kernel_radius, float multiplier);
		virtual ~SeparableBlurPostProcess();

	protected:
		float GaussianDistribution(float x, float y, float rho);
		void CalSampleOffsets(uint32_t tex_size, float deviation);

	protected:
		int kernel_radius_;
		float multiplier_;
	};

	class Downsampler2x2PostProcess : public PostProcess
	{
	public:
		Downsampler2x2PostProcess();
	};

	class BrightPassDownsampler2x2PostProcess : public PostProcess
	{
	public:
		BrightPassDownsampler2x2PostProcess();
	};

	class BlurXPostProcess : public SeparableBlurPostProcess
	{
	public:
		BlurXPostProcess(int kernel_radius, float multiplier);

		void Source(TexturePtr const & src_tex, bool flipping);
	};

	class BlurYPostProcess : public SeparableBlurPostProcess
	{
	public:
		BlurYPostProcess(int kernel_radius, float multiplier);

		void Source(TexturePtr const & src_tex, bool flipping);
	};

	class BlurPostProcess : public PostProcess
	{
	public:
		BlurPostProcess(int kernel_radius, float multiplier);

		void Destinate(RenderTargetPtr const & rt);

		void Apply();

	private:
		BlurXPostProcess blur_x_;
		BlurYPostProcess blur_y_;

		TexturePtr blurx_tex_;
	};
}

#endif		// _POSTPROCESS_HPP
