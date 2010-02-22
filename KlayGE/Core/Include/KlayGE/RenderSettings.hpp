// RenderSettings.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2005-2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// ������motion_frames (2010.2.22)
//
// 3.7.0
// ȥ����ConfirmDevice�Ĳ��� (2008.3.17)
//
// 2.8.0
// ������ConfirmDevice (2005.7.17)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERSETTINGS_HPP
#define _RENDERSETTINGS_HPP

#pragma once

#include <boost/function.hpp>

#include <KlayGE/ElementFormat.hpp>

namespace KlayGE
{
	// ������Ⱦ���ڵ�����
	/////////////////////////////////////////////////////////////////////////////////
	struct RenderSettings
	{
		RenderSettings()
			: left(0), top(0),
				depth_stencil_fmt(EF_D16),
				sample_count(1), sample_quality(0),
				motion_frames(0)
		{
		}

		boost::function<bool()> ConfirmDevice;

		bool	full_screen;
		int		left;
		int		top;
		int		width;
		int		height;
		ElementFormat color_fmt;
		ElementFormat depth_stencil_fmt;
		uint32_t sample_count;
		uint32_t sample_quality;
		uint32_t motion_frames;
	};
}

#endif			// _RENDERSETTINGS_HPP
