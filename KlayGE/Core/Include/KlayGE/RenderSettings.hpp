#ifndef _RENDERSETTINGS_HPP
#define _RENDERSETTINGS_HPP

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ������Ⱦ���ڵ�����
	/////////////////////////////////////////////////////////////////////////////////
	class RenderSettings
	{
	public:
		RenderSettings()
			: left(0), top(0),
				depthBuffer(true)
			{ }
		virtual ~RenderSettings()
			{ }

		int		width;
		int		height;
		int		colorDepth;
		bool	fullScreen;
		int		left;
		int		top;
		bool	depthBuffer;
	};
}

#endif			// _RENDERSETTINGS_HPP
