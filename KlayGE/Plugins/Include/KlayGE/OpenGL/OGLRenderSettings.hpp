#ifndef _OGLRENDERSETTINGS_HPP
#define _OGLRENDERSETTINGS_HPP

#include <KlayGE/RenderSettings.hpp>

#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")

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
