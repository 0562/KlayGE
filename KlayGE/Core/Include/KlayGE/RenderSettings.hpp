// RenderSettings.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2005-2008
// Homepage: http://klayge.sourceforge.net
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
				multi_sample(0)
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
		uint32_t multi_sample;
	};
}

#endif			// _RENDERSETTINGS_HPP
