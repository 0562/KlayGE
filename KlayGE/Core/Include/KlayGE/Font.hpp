// Font.hpp
// KlayGE Font�� ͷ�ļ�
// Ver 2.3.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
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

#include <KlayGE/Font.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef _DEBUG
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
		};

		// ������Ⱦ��־
		enum FontAttrib
		{
			FA_TwoSided		= 1UL << 1,
			FA_Filtered		= 1UL << 2,
		};

	public:
		Font(std::string const & fontName, uint32_t fontHeight = 12, uint32_t flags = 0);
		~Font();

		RenderablePtr RenderText(float x, float y, Color const & clr,
			std::wstring const & text, uint32_t flags = 0);
		RenderablePtr RenderText(float x, float y, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text, uint32_t flags = 0);
		//RenderablePtr RenderText(std::wstring const & text, uint32_t flags = 0);

		uint32_t FontHeight() const;

	public:
		struct CharInfo
		{
			Rect_T<float>	texRect;
			uint32_t		width;
		};

		typedef std::map<wchar_t, CharInfo> CharInfoMapType;
		typedef std::list<wchar_t> CharLRUType;

	private:
		void UpdateTexture(std::wstring const & text);

		RenderBufferPtr		rb_;
		RenderEffectPtr		effect_;
		TexturePtr			theTexture_;

		CharInfoMapType		charInfoMap_;
		CharLRUType			charLRU_;

		uint32_t curX_, curY_;
		uint32_t fontHeight_;

		::FT_Library	ftLib_;
		::FT_Face		face_;
		::FT_GlyphSlot	slot_;
	};
}

#endif		// _FONT_HPP
