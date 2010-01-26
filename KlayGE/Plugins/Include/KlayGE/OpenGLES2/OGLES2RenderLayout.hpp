// OGLES2RenderLayout.hpp
// KlayGE OpenGL ES 2��Ⱦ�ֲ��� ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// ���ν��� (2010.1.22)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLES2RENDERLAYOUT_HPP
#define _OGLES2RENDERLAYOUT_HPP

#pragma once

#include <KlayGE/RenderLayout.hpp>

namespace KlayGE
{
	class OGLES2RenderLayout : public RenderLayout
	{
	public:
		OGLES2RenderLayout();
		~OGLES2RenderLayout();

		void Active(ShaderObjectPtr const & so) const;
		void Deactive(ShaderObjectPtr const & so) const;
	};
}

#endif			// _OGLES2RENDERLAYOUT_HPP
