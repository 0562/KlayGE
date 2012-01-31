// OGLESRenderLayout.hpp
// KlayGE OpenGL ES��Ⱦ�ֲ��� ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ���ν��� (2010.1.22)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLESRENDERLAYOUT_HPP
#define _OGLESRENDERLAYOUT_HPP

#pragma once

#include <KlayGE/RenderLayout.hpp>

namespace KlayGE
{
	class OGLESRenderLayout : public RenderLayout
	{
	public:
		OGLESRenderLayout();
		~OGLESRenderLayout();

		void Active(ShaderObjectPtr const & so) const;
		void Deactive(ShaderObjectPtr const & so) const;
	};
}

#endif			// _OGLESRENDERLAYOUT_HPP
