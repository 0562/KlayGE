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

#pragma once

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/PostProcess.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API SATSeparableScanSweepPostProcess : public PostProcess
	{
	public:
		SATSeparableScanSweepPostProcess(RenderTechniquePtr const & tech, bool dir);

		void ChildBuffer(TexturePtr const & tex);
		void Length(int32_t length);
		void AddrOffset(float3 offset);
		void Scale(float scale);

	private:
		int32_t length_;
		bool dir_;

		RenderEffectParameterPtr child_tex_ep_;
		RenderEffectParameterPtr addr_offset_ep_;
		RenderEffectParameterPtr length_ep_;
		RenderEffectParameterPtr scale_ep_;
	};

	class KLAYGE_CORE_API SummedAreaTablePostProcess : public PostProcessChain
	{
	public:
		SummedAreaTablePostProcess();

		void InputPin(uint32_t index, TexturePtr const & tex);
		TexturePtr const & InputPin(uint32_t index) const;
	};
}

#endif		// _SATPOSTPROCESS_HPP
