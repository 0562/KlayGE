// OGLFont.hpp
// KlayGE OGLFont�� ͷ�ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// ���ν��� (2004.4.4)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLFONT_HPP
#define _OGLFONT_HPP

#include <KlayGE/Rect.hpp>

#include <list>
#include <map>

#include <KlayGE/Font.hpp>

#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")

namespace KlayGE
{
	// ��3D�����л�������
	/////////////////////////////////////////////////////////////////////////////////
	class OGLFont : public Font
	{
	public:
		// 2D & 3D �������ֵĺ���
		RenderablePtr RenderText(float x, float y, Color const & clr,
			std::wstring const & text, U32 flags = 0);
		RenderablePtr RenderText(float x, float y, float z, float xScale, float yScale,
			Color const & clr, std::wstring const & text, U32 flags = 0);
		//RenderablePtr RenderText(std::wstring const & text, U32 flags = 0);

		U32 FontHeight() const;

		OGLFont(std::wstring const & fontName, U32 fontHeight = 12, U32 flags = 0);

	public:
		struct CharInfo
		{
			Rect_T<float>	texRect;
			U32				width;
		};

		typedef std::map<wchar_t, CharInfo> CharInfoMapType;
		typedef std::list<wchar_t> CharLRUType;

	public:
		void UpdateTexture(std::wstring const & text);

		LOGFONTW logFont_;

		RenderBufferPtr rb_;
		RenderEffectPtr	effect_;
		TexturePtr		theTexture_;

		CharInfoMapType charInfoMap_;
		CharLRUType charLRU_;

		U32		curX_, curY_;
	};
}

#endif		// _OGLFONT_HPP
