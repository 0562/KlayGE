// RenderDeviceCaps.hpp
// KlayGE ��Ⱦ�豸������ ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.17)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERDEVICECAPS_HPP
#define _RENDERDEVICECAPS_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

namespace KlayGE
{
	struct RenderDeviceCaps
	{
		uint8_t max_shader_model;

		uint32_t max_texture_width;
		uint32_t max_texture_height;
		uint32_t max_texture_depth;
		uint32_t max_texture_cube_size;
		uint32_t max_texture_array_length;
		uint8_t max_vertex_texture_units;
		uint8_t max_pixel_texture_units;
		uint8_t max_geometry_texture_units;

		uint8_t max_simultaneous_rts;

		uint32_t max_vertices;
		uint32_t max_indices;

		uint8_t max_texture_anisotropy;

		bool hw_instancing_support;
		bool stream_output_support;
		bool alpha_to_coverage_support;
		bool depth_texture_support;
		bool primitive_restart_support;
		bool argb8_support;
		bool bc4_support;
		bool bc5_support;
		bool bc6_support;
		bool bc7_support;

		bool gs_support;
		bool cs_support;
		bool hs_support;
		bool ds_support;
	};
}

#endif			// _RENDERDEVICECAPS_HPP
