// D3D9Font.hpp
// KlayGE D3D9Font�� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9FONT_HPP
#define _D3D9FONT_HPP

#include <KlayGE/Rect.hpp>

#include <list>
#include <map>

#include <KlayGE/Font.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")

namespace KlayGE
{
	// ��3D�����л�������
	/////////////////////////////////////////////////////////////////////////////////
	class D3D9Font : public Font
	{
	public:
		// 2D & 3D �������ֵĺ���
		RenderablePtr RenderText(float x, float y, Color const & clr,
			std::wstring const & text, uint32_t flags = 0);
		RenderablePtr RenderText(float x, float y, float z, float xScale, float yScale,
			Color const & clr, std::wstring const & text, uint32_t flags = 0);
		//RenderablePtr RenderText(std::wstring const & text, uint32_t flags = 0);

		uint32_t FontHeight() const;

		D3D9Font(std::string const & fontName, uint32_t fontHeight = 12, uint32_t flags = 0);

	public:
		struct CharInfo
		{
			Rect_T<float>	texRect;
			uint32_t				width;
		};

		typedef std::map<wchar_t, CharInfo> CharInfoMapType;
		typedef std::list<wchar_t> CharLRUType;

	private:
		void UpdateTexture(std::wstring const & text);

		RenderBufferPtr rb_;
		RenderEffectPtr	effect_;
		TexturePtr		theTexture_;

		CharInfoMapType charInfoMap_;
		CharLRUType charLRU_;

		uint32_t curX_, curY_;
		uint32_t fontHeight_;

		FT_Library ftLib_;
		FT_Face face_;
		FT_GlyphSlot slot_;
	};
}

#endif		// _D3D9FONT_HPP
