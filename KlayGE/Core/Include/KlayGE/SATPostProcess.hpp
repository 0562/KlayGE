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
	class SATSeparableScanPostProcess : public PostProcess
	{
	public:
		SATSeparableScanPostProcess(RenderTechniquePtr tech);
		virtual ~SATSeparableScanPostProcess();

		void Length(uint32_t length);
		void Pass(uint32_t pass);

	private:
		int length_;
	};

	class SATScanXPostProcess : public SATSeparableScanPostProcess
	{
	public:
		SATScanXPostProcess();
	};

	class SATScanYPostProcess : public SATSeparableScanPostProcess
	{
	public:
		SATScanYPostProcess();
	};

	class SummedAreaTablePostProcess : public PostProcess
	{
	public:
		SummedAreaTablePostProcess();

		void Source(TexturePtr const & tex, bool flipping);
		void Apply();

		TexturePtr SATTexture();

	private:
		uint32_t num_pass_x_, num_pass_y_;
		TexturePtr inter_tex_[2];
		FrameBufferPtr inter_fb_[2];
		bool index_;

		SATScanXPostProcess scan_x_;
		SATScanYPostProcess scan_y_;
	};
}

#endif		// _SATPOSTPROCESS_HPP