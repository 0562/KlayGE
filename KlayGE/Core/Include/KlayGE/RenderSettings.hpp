// RenderEngine.hpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������ConfirmDevice (2005.7.17)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERSETTINGS_HPP
#define _RENDERSETTINGS_HPP

#define BOOST_MEM_FN_ENABLE_STDCALL
#include <boost/mem_fn.hpp>
#include <boost/function.hpp>
#include <KlayGE/RenderDeviceCaps.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// ������Ⱦ���ڵ�����
	/////////////////////////////////////////////////////////////////////////////////
	struct RenderSettings
	{
		RenderSettings()
			: left(0), top(0),
				depthBuffer(true), depthBits(16), stencilBits(0),
				multiSample(0)
		{
		}

		boost::function<bool (RenderDeviceCaps const &)> ConfirmDevice;

		int		width;
		int		height;
		int		colorDepth;
		bool	fullScreen;
		int		left;
		int		top;
		bool	depthBuffer;
		int		depthBits;
		int		stencilBits;
		uint32_t multiSample;
	};
}

#endif			// _RENDERSETTINGS_HPP
