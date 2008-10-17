// Font.hpp
// KlayGE Font�� ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2003-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// �µĻ���distance�������ʽ (2008.2.13)
//
// 3.6.0
// ������Rect����ķ�ʽ (2007.6.5)
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

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/Rect.hpp>

#include <list>
#include <map>
#include <vector>

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
#include <boost/pool/pool_alloc.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

namespace KlayGE
{
	// ��3D�����л�������
	/////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API Font
	{
	public:
		// ���彨����־
		enum FontStyle
		{
			FS_TwoSided		= 1UL << 0,
			FS_Cullable		= 1UL << 1
		};

		enum FontAlign
		{
			FA_Hor_Left		= 1UL << 0,
			FA_Hor_Center	= 1UL << 1,
			FA_Hor_Right	= 1UL << 2,

			FA_Ver_Top		= 1UL << 3,
			FA_Ver_Middle	= 1UL << 4,
			FA_Ver_Bottom	= 1UL << 5
		};

	public:
		Font(std::string const & fontName, uint32_t fontHeight = 16, uint32_t flags = 0);

		Size_T<uint32_t> CalcSize(std::wstring const & text);
		void RenderText(float x, float y, Color const & clr,
			std::wstring const & text);
		void RenderText(float x, float y, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text);
		void RenderText(Rect const & rc, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text, uint32_t align);
		void RenderText(float4x4 const & mvp, Color const & clr, std::wstring const & text);

		uint32_t FontHeight() const;

	private:
		RenderablePtr	font_renderable_;
		uint32_t		font_height_;
		uint32_t		fso_attrib_;
	};
}

#endif		// _FONT_HPP
