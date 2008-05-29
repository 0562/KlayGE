// SATPostProcess.hpp
// KlayGE Summed-Area Table���ڴ����� ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ���ν��� (2006.10.10)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SATPOSTPROCESS_HPP
#define _SATPOSTPROCESS_HPP

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/PostProcess.hpp>

namespace KlayGE
{
	class SATSeparableScanSweepPostProcess : public PostProcess
	{
	public:
		SATSeparableScanSweepPostProcess(RenderTechniquePtr tech, bool dir);

		void ChildBuffer(TexturePtr const & tex);
		void Length(int length);
		void AddrOffset(float3 offset);
		void Scale(float scale);

	private:
		int length_;
		bool dir_;
	};

	class SummedAreaTablePostProcess : public PostProcess
	{
	public:
		SummedAreaTablePostProcess();

		void Source(TexturePtr const & tex, bool flipping);
		void Apply();

		TexturePtr SATTexture();

	private:
		std::vector<TexturePtr> inter_tex_x_up_;
		std::vector<TexturePtr> inter_tex_x_down_;
		std::vector<TexturePtr> inter_tex_y_up_;
		std::vector<TexturePtr> inter_tex_y_down_;
		std::vector<FrameBufferPtr> inter_fb_x_up_;
		std::vector<FrameBufferPtr> inter_fb_x_down_;
		std::vector<FrameBufferPtr> inter_fb_y_up_;
		std::vector<FrameBufferPtr> inter_fb_y_down_;

		SATSeparableScanSweepPostProcess scan_x_up_;
		SATSeparableScanSweepPostProcess scan_y_up_;
		SATSeparableScanSweepPostProcess scan_x_down_;
		SATSeparableScanSweepPostProcess scan_y_down_;
	};
}

#endif		// _SATPOSTPROCESS_HPP
