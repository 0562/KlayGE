// RenderStateObject.cpp
// KlayGE ��Ⱦ״̬������ ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2006-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ��RenderStateObject��������� (2008.6.29)
//
// 3.5.0
// ���ν��� (2006.11.1)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <boost/assert.hpp>

#include <KlayGE/RenderStateObject.hpp>

namespace KlayGE
{
	RasterizerStateDesc::RasterizerStateDesc()
		: polygon_mode(PM_Fill),
			shade_mode(SM_Gouraud),
			cull_mode(CM_AntiClockwise),
			polygon_offset_factor(0),
			polygon_offset_units(0),
			scissor_enable(false),
			multisample_enable(false)
	{
	}

	BlendStateDesc::BlendStateDesc()
		: alpha_to_coverage_enable(false),
			independent_blend_enable(false)
	{
		blend_enable.assign(false);
		blend_op.assign(BOP_Add);
		src_blend.assign(ABF_One);
		dest_blend.assign(ABF_Zero);
		blend_op_alpha.assign(BOP_Add);
		src_blend_alpha.assign(ABF_One);
		dest_blend_alpha.assign(ABF_Zero);
		color_write_mask.assign(CMASK_All);
	}

	DepthStencilStateDesc::DepthStencilStateDesc()
		: depth_enable(true),
			depth_write_mask(true),
			depth_func(CF_Less),
			front_stencil_enable(false),
			front_stencil_func(CF_AlwaysPass),
			front_stencil_ref(0),
			front_stencil_read_mask(0xFFFF),
			front_stencil_write_mask(0xFFFF),
			front_stencil_fail(SOP_Keep),
			front_stencil_depth_fail(SOP_Keep),
			front_stencil_pass(SOP_Keep),
			back_stencil_enable(false),
			back_stencil_func(CF_AlwaysPass),
			back_stencil_ref(0),
			back_stencil_read_mask(0xFFFF),
			back_stencil_write_mask(0xFFFF),
			back_stencil_fail(SOP_Keep),
			back_stencil_depth_fail(SOP_Keep),
			back_stencil_pass(SOP_Keep)
	{
	}

	bool operator==(RasterizerStateObject const & lhs, RasterizerStateObject const & rhs)
	{
		return 0 == memcmp(&lhs, &rhs, sizeof(lhs));
	}

	bool operator!=(RasterizerStateObject const & lhs, RasterizerStateObject const & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator==(DepthStencilStateObject const & lhs, DepthStencilStateObject const & rhs)
	{
		return 0 == memcmp(&lhs, &rhs, sizeof(lhs));
	}

	bool operator!=(DepthStencilStateObject const & lhs, DepthStencilStateObject const & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator==(BlendStateObject const & lhs, BlendStateObject const & rhs)
	{
		return 0 == memcmp(&lhs, &rhs, sizeof(lhs));
	}

	bool operator!=(BlendStateObject const & lhs, BlendStateObject const & rhs)
	{
		return !(lhs == rhs);
	}
}
