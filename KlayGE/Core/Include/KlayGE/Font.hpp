// Font.hpp
// KlayGE Font�� ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ֧����Ⱦ��3Dλ�� (2006.5.20)
//
// 2.8.0
// ������pool (2005.8.10)
//
// 2.3.0
// ʹ��FreeTypeʵ�������ȡ (2004.12.26)
// 
// 2.0.0
// ���ν��� (2003.8.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FONT_HPP
#define _FONT_HPP

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/Rect.hpp>

#include <list>
#include <map>
#include <vector>

#define NOMINMAX
#pragma warning(push)
#pragma warning(disable: 4127 4800)
#include <boost/pool/pool_alloc.hpp>
#pragma warning(pop)

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

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

			FS_TwoSided		= 1UL << 4,
			FS_Filtered		= 1UL << 5,
			FS_Cullable		= 1UL << 6,
		};

	public:
		Font(std::string const & fontName, uint32_t fontHeight = 12, uint32_t flags = 0);

		void RenderText(float x, float y, Color const & clr,
			std::wstring const & text);
		void RenderText(float x, float y, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text);
		void RenderText(float4x4 const & mvp, Color const & clr, std::wstring const & text);

		uint32_t FontHeight() const;

	private:
		RenderablePtr	font_renderable_;
		uint32_t		fso_attrib_;
	};
}

#endif		// _FONT_HPP
