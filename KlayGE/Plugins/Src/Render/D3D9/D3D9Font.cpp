// D3D9Font.cpp
// KlayGE D3D9Font�� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// �����ʽ��ΪPF_A4L4 (2004.3.18)
//
// 2.0.3
// ������RenderText��Bug (2004.2.18)
// ����VertexShader���2D�任 (2004.3.1)
//
// 2.0.0
// ���ν��� (2003.8.18)
// ʹ��LRU�㷨 (2003.9.26)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#define NOMINMAX

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/SharePtr.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/Engine.hpp>

#include <cassert>
#include <algorithm>
#include <vector>

#include <d3d9types.h>

#include <KlayGE/D3D9/D3D9Font.hpp>

namespace
{
	using namespace KlayGE;

	String fontEffectStr("\
		texture texFont;\
		int halfWidth;\
		int halfHeight;\
		\
		void FontVS(float4 position : POSITION,\
			float2 texCoord : TEXCOORD0,\
			float4 color : DIFFUSE,\
		\
			out float4 oPosition : POSITION,\
			out float2 oTexCoord : TEXCOORD0,\
			out float4 oColor : COLOR)\
		{\
			oPosition.x = (position.x - halfWidth) / halfWidth;\
			oPosition.y = (halfHeight - position.y) / halfHeight;\
			oPosition.zw = position.zw;\
		\
			oColor = color;\
			oTexCoord = texCoord;\
		}\
		\
		technique fontTec\
		{\
			pass p0\
			{\
				Lighting = false;\
		\
				AlphaBlendEnable = true;\
				SrcBlend = SrcColor;\
				DestBlend = DestColor;\
				AlphaTestEnable = true;\
				AlphaRef = 0x08;\
				AlphaFunc = GreaterEqual;\
		\
				FillMode = Solid;\
				CullMode = CCW;\
				StencilEnable = false;\
				Clipping = true;\
				ClipPlaneEnable = 0;\
				VertexBlend = Disable;\
				IndexedVertexBlendEnable = false;\
				FogEnable = false;\
				ColorWriteEnable = RED | GREEN | BLUE | ALPHA;\
		\
				Texture[0] = <texFont>;\
				TextureTransformFlags[0] = Disable;\
				ColorOp[0] = Modulate;\
				ColorArg1[0] = Texture;\
				ColorArg2[0] = Diffuse;\
				AlphaOp[0] = Modulate;\
				AlphaArg1[0] = Texture;\
				AlphaArg2[0] = Diffuse;\
				MinFilter[0] = Point;\
				MagFilter[0] = Point;\
				MipFilter[0] = None;\
		\
				ColorOp[1] = Disable;\
				AlphaOp[1] = Disable;\
		\
				VertexShader = compile vs_1_1 FontVS();\
			}\
		}\
		");


	class D3D9FontRenderable : public Renderable
	{
	public:
		D3D9FontRenderable(const RenderEffectPtr& effect)
			: fontVB_(new VertexBuffer(VertexBuffer::BT_TriangleList)),
				fontEffect_(effect)
		{
			fontVB_->AddVertexStream(VST_Positions, sizeof(float), 3);
			fontVB_->AddVertexStream(VST_Diffuses, sizeof(D3DCOLOR), 1);
			fontVB_->AddVertexStream(VST_TextureCoords0, sizeof(float), 2);

			fontVB_->AddIndexStream();
		}

		const WString& Name() const
		{
			static WString name_(L"Direct3D9 Font");
			return name_;
		}

		RenderEffectPtr GetRenderEffect()
			{ return fontEffect_; }

		VertexBufferPtr GetVertexBuffer()
			{ return fontVB_; }

		void RenderText(U32 fontHeight, D3D9Font::CharInfoMapType& charInfoMap, float sx, float sy, float sz,
			float xScale, float yScale, U32 clr, const WString& text, U32 flags)
		{
			// ���ù�������
			if (flags & Font::FA_Filtered)
			{
				Engine::RenderFactoryInstance().RenderEngineInstance().TextureFiltering(0, RenderEngine::TF_Bilinear);
			}

			const float h(fontHeight * yScale);
			const size_t maxSize(text.length() - std::count(text.begin(), text.end(), L'\n'));
			float x(sx), y(sy);

			std::vector<U32, alloc<U32> > clrs(maxSize * 4, clr);

			std::vector<float, alloc<float> > xyzs;
			std::vector<float, alloc<float> > texs;
			std::vector<U16, alloc<U16> > indices;

			xyzs.reserve(maxSize * 3 * 4);
			texs.reserve(maxSize * 2 * 4);
			indices.reserve(maxSize * 6);

			U16 lastIndex(0);
			for (WString::const_iterator citer = text.begin(); citer != text.end(); ++ citer)
			{
				const wchar_t& ch(*citer);
				const float w(charInfoMap[ch].width * xScale);

				if (ch != L'\n')
				{
					const Rect_T<float>& texRect(charInfoMap[ch].texRect);

					xyzs.push_back(x);
					xyzs.push_back(y);
					xyzs.push_back(sz);

					xyzs.push_back(x + w);
					xyzs.push_back(y);
					xyzs.push_back(sz);

					xyzs.push_back(x + w);
					xyzs.push_back(y + h);
					xyzs.push_back(sz);

					xyzs.push_back(x);
					xyzs.push_back(y + h);
					xyzs.push_back(sz);


					texs.push_back(texRect.left());
					texs.push_back(texRect.top());

					texs.push_back(texRect.right());
					texs.push_back(texRect.top());

					texs.push_back(texRect.right());
					texs.push_back(texRect.bottom());

					texs.push_back(texRect.left());
					texs.push_back(texRect.bottom());


					indices.push_back(lastIndex + 0);
					indices.push_back(lastIndex + 1);
					indices.push_back(lastIndex + 2);
					indices.push_back(lastIndex + 2);
					indices.push_back(lastIndex + 3);
					indices.push_back(lastIndex + 0);
					lastIndex += 4;

					x += w;
				}
				else
				{
					y += h;
					x = sx;
				}
			}

			fontVB_->GetVertexStream(VST_Positions)->Assign(&xyzs[0], xyzs.size() / 3);
			fontVB_->GetVertexStream(VST_Diffuses)->Assign(&clrs[0], clrs.size());
			fontVB_->GetVertexStream(VST_TextureCoords0)->Assign(&texs[0], texs.size() / 2);

			fontVB_->GetIndexStream()->Assign(&indices[0], indices.size());
		}

	private:
		RenderEffectPtr		fontEffect_;
		VertexBufferPtr		fontVB_;
	};
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9Font::D3D9Font(const WString& fontName, U32 height, U32 flags)
				: curX_(0), curY_(0),
					theTexture_(Engine::RenderFactoryInstance().MakeTexture(1024, 1024, 1, PF_A4L4))
	{
		effect_ = Engine::RenderFactoryInstance().MakeRenderEffect(fontEffectStr);
		effect_->SetTexture("texFont", theTexture_);
		effect_->SetTechnique("fontTec");

		RenderEngine& renderEngine(Engine::RenderFactoryInstance().RenderEngineInstance());
		const Viewport& viewport((*renderEngine.ActiveRenderTarget())->GetViewport());
		effect_->SetInt("halfWidth", viewport.width / 2);
		effect_->SetInt("halfHeight", viewport.height / 2);

		logFont_.lfHeight			= height;
		logFont_.lfWidth			= 0;
		logFont_.lfEscapement		= 0;
		logFont_.lfOrientation		= 0;
		logFont_.lfWeight			= flags & Font::FS_Bold ? FW_BOLD : FW_NORMAL;
		logFont_.lfItalic			= flags & Font::FS_Italic ? TRUE : FALSE;
		logFont_.lfUnderline		= flags & Font::FS_Underline ? TRUE : FALSE;
		logFont_.lfStrikeOut		= flags & Font::FS_Strikeout ? TRUE : FALSE;
		logFont_.lfCharSet			= DEFAULT_CHARSET;
		logFont_.lfOutPrecision		= OUT_DEFAULT_PRECIS; 
		logFont_.lfClipPrecision	= CLIP_DEFAULT_PRECIS; 
		logFont_.lfQuality			= ANTIALIASED_QUALITY;
		logFont_.lfPitchAndFamily	= VARIABLE_PITCH;
		fontName.copy(logFont_.lfFaceName, fontName.length());
	}

	// ��ȡ����߶�
	/////////////////////////////////////////////////////////////////////////////////
	U32 D3D9Font::FontHeight() const
	{
		return logFont_.lfHeight;
	}

	// ��������ʹ��LRU�㷨
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9Font::UpdateTexture(const WString& text)
	{
		::SIZE size;
		for (WString::const_iterator citer = text.begin(); citer != text.end(); ++ citer)
		{
			const wchar_t& ch(*citer);

			if (charInfoMap_.find(ch) != charInfoMap_.end())
			{
				// �������������ҵ���

				if (ch != charLRU_.front())
				{
					charLRU_.remove(ch);
					charLRU_.push_front(ch);
				}
			}
			else
			{
				// �������������Ҳ��������Ե��������������������

				if (ch != L'\n')
				{
					// Ϊ���彨�� DC �� bitmap
					HDC hDC(::CreateCompatibleDC(NULL));

					// ��������
					HFONT hFont(::CreateFontIndirectW(&logFont_));
					if (NULL == hFont)
					{
						::DeleteDC(hDC);
						THR(E_FAIL);
					}
					HGDIOBJ hOldFont(::SelectObject(hDC, hFont));

					::GetTextExtentPoint32W(hDC, &ch, 1, &size);

					BITMAPINFO bmi;
					Engine::MemoryInstance().Zero(&bmi.bmiHeader, sizeof(bmi.bmiHeader));
					bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
					bmi.bmiHeader.biWidth		= size.cx;
					bmi.bmiHeader.biHeight		= -size.cy;
					bmi.bmiHeader.biPlanes		= 1;
					bmi.bmiHeader.biBitCount	= 32;
					bmi.bmiHeader.biCompression = BI_RGB;

					U32* bitmapBits(NULL);
					HBITMAP hBitmap(::CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,
						reinterpret_cast<void**>(&bitmapBits), NULL, 0));
					if ((NULL == hBitmap) || (NULL == bitmapBits))
					{
						::DeleteObject(::SelectObject(hDC, hOldFont));
						::DeleteDC(hDC);
						THR(E_FAIL);
					}

					HGDIOBJ hOldBitmap(::SelectObject(hDC, hBitmap));

					// ������������
					::SetTextColor(hDC, RGB(255, 255, 255));
					::SetBkColor(hDC, RGB(0, 0, 0));
					::SetTextAlign(hDC, TA_TOP);

					::TextOutW(hDC, 0, 0, &ch, 1);

					::RECT charRect;
					CharInfo charInfo;
					if ((curX_ < theTexture_->Width()) && (curY_ < theTexture_->Height()))
					{
						// �����пռ�
						charRect.left	= curX_;
						charRect.top	= curY_;
						charRect.right	= curX_ + size.cx;
						charRect.bottom = curY_ + size.cy;

						charInfo.texRect.left()		= static_cast<float>(charRect.left) / theTexture_->Width();
						charInfo.texRect.top()		= static_cast<float>(charRect.top) / theTexture_->Height();
						charInfo.texRect.right()	= static_cast<float>(charRect.right) / theTexture_->Width();
						charInfo.texRect.bottom()	= static_cast<float>(charRect.bottom) / theTexture_->Height();
						charInfo.width				= size.cx;

						curX_ += this->FontHeight();
						if (curX_ >= theTexture_->Width())
						{
							curX_ = 0;
							curY_ += this->FontHeight();
						}
					}
					else
					{
						// �ҵ�ʹ���ʱ��û��ʹ�õ���
						CharInfoMapType::iterator iter(charInfoMap_.find(charLRU_.back()));
						assert(iter != charInfoMap_.end());

						// �õ�ǰ�ַ��滻
						charInfo.texRect	= iter->second.texRect;
						charInfo.width		= size.cx;

						charLRU_.pop_back();
						charInfoMap_.erase(iter);

						charRect.left	= static_cast<long>(charInfo.texRect.left() * theTexture_->Width());
						charRect.top	= static_cast<long>(charInfo.texRect.top() * theTexture_->Height());
						charRect.right	= charRect.left + this->FontHeight();
						charRect.bottom	= charRect.top + this->FontHeight();
					}

					std::vector<U8, alloc<U8> > dst;
					dst.reserve(this->FontHeight() * this->FontHeight());
					// �������棬�� alpha ֵд������
					for (long y = charRect.top; y < charRect.bottom; ++ y)
					{
						for (long x = charRect.left; x < charRect.right; ++ x, ++ bitmapBits)
						{
							dst.push_back(static_cast<U8>(*bitmapBits) & 0xF0 | 0x0F);
						}
					}
					theTexture_->CopyMemoryToTexture(&dst[0], PF_A4L4, charRect.right - charRect.left,
						charRect.bottom - charRect.top, 0, charRect.left, charRect.top);

					// �Ѿ������������������
					::DeleteObject(::SelectObject(hDC, hOldBitmap));
					::DeleteObject(::SelectObject(hDC, hOldFont));
					::DeleteDC(hDC);

					charInfoMap_.insert(std::make_pair(ch, charInfo));
					charLRU_.push_front(ch);
				}
			}
		}
	}

	// ��ָ��λ�û�������
	/////////////////////////////////////////////////////////////////////////////////
	RenderablePtr D3D9Font::RenderText(float sx, float sy, const Color& clr, 
		const WString& text, U32 flags)
	{
		return this->RenderText(sx, sy, 0.5f, 1, 1, clr, text, flags);
	}

	// ��ָ��λ�û�������������
	/////////////////////////////////////////////////////////////////////////////////
	RenderablePtr D3D9Font::RenderText(float sx, float sy, float sz,
		float xScale, float yScale, const Color& clr,
		const WString& text, U32 flags)
	{
		if (text.empty())
		{
			return RenderablePtr(NULL);
		}

		this->UpdateTexture(text);

		U8 r, g, b, a;
		clr.RGBA(r, g, b, a);

		SharePtr<D3D9FontRenderable> renderable(new D3D9FontRenderable(effect_));
		renderable->RenderText(this->FontHeight(), charInfoMap_,
			sx, sy, sz, xScale, yScale, D3DCOLOR_ARGB(a, r, g, b), text, flags);
		return renderable;
	}
}
