#ifndef _OGLRENDERSETTINGS_HPP
#define _OGLRENDERSETTINGS_HPP

#include <KlayGE/RenderSettings.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	// ������Ⱦ���ڵ�����
	/////////////////////////////////////////////////////////////////////////////////
	struct OGLRenderSettings : public RenderSettings
	{
		OGLRenderSettings()
			{ }
		virtual ~OGLRenderSettings()
			{ }
	};
}

#endif		// _OGLRENDERSETTINGS_HPP
