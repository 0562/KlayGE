// Font.hpp
// KlayGE Font�� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FONT_HPP
#define _FONT_HPP

#include <KlayGE/PreDeclare.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ��3D�����л�������
	/////////////////////////////////////////////////////////////////////////////////
	class Font
	{
	public:
		// ���彨����־
		enum FontStyle
		{
			FS_Bold			= 1UL << 0,
			FS_Italic		= 1UL << 1,
			FS_Underline	= 1UL << 2,
			FS_Strikeout	= 1UL << 3,
		};

		// ������Ⱦ��־
		enum FontAttrib
		{
			FA_TwoSided		= 1UL << 1,
			FA_Filtered		= 1UL << 2,
		};

	public:
		virtual RenderablePtr RenderText(float x, float y, Color const & clr,
			std::wstring const & text, U32 flags = 0) = 0;
		virtual RenderablePtr RenderText(float x, float y, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text, U32 flags = 0) = 0;
		//RenderablePtr RenderText(std::wstring const & text, U32 flags = 0);

		virtual U32 FontHeight() const = 0;

		virtual ~Font()
			{ }
	};
}

#endif		// _FONT_HPP
