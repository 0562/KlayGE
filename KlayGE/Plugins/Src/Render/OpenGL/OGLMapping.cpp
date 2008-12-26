// OGLMapping.hpp
// KlayGE RenderEngine��OpenGL����֮���ӳ�� ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ������TAM_Border (2005.8.30)
//
// 2.8.0
// ���ν��� (2005.7.19)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Vector.hpp>
#include <KlayGE/Matrix.hpp>
#include <KlayGE/Color.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderLayout.hpp>

#include <boost/assert.hpp>

#include <glloader/glloader.h>

#include <KlayGE/OpenGL/OGLTexture.hpp>
#include <KlayGE/OpenGL/OGLMapping.hpp>

namespace KlayGE
{
	// ��KlayGE��Colorת����float[4]
	/////////////////////////////////////////////////////////////////////////////////
	void OGLMapping::Mapping(GLfloat* clr4, Color const & clr)
	{
		clr4[0] = clr.r();
		clr4[1] = clr.g();
		clr4[2] = clr.b();
		clr4[3] = clr.a();
	}

	// ��RenderEngine::CompareFunctionת����GLenum
	/////////////////////////////////////////////////////////////////////////////////
	GLenum OGLMapping::Mapping(CompareFunction func)
	{
		switch (func)
		{
		case CF_AlwaysFail:
			return GL_NEVER;

		case CF_AlwaysPass:
			return GL_ALWAYS;

		case CF_Less:
			return GL_LESS;

		case CF_LessEqual:
			return GL_LEQUAL;

		case CF_Equal:
			return GL_EQUAL;

		case CF_NotEqual:
			return GL_NOTEQUAL;

		case CF_GreaterEqual:
			return GL_GEQUAL;

		case CF_Greater:
			return GL_GREATER;

		default:
			BOOST_ASSERT(false);
			return GL_EQUAL;
		};
	}

	// ��RenderEngine::AlphaBlendFactorת����GLenum
	/////////////////////////////////////////////////////////////////////////////////
	GLenum OGLMapping::Mapping(AlphaBlendFactor factor)
	{
		switch (factor)
		{
		case ABF_Zero:
			return GL_ZERO;

		case ABF_One:
			return GL_ONE;

		case ABF_Src_Alpha:
			return GL_SRC_ALPHA;

		case ABF_Dst_Alpha:
			return GL_DST_ALPHA;

		case ABF_Inv_Src_Alpha:
			return GL_ONE_MINUS_SRC_ALPHA;

		case ABF_Inv_Dst_Alpha:
			return GL_ONE_MINUS_DST_ALPHA;

		case ABF_Src_Color:
			return GL_SRC_COLOR;

		case ABF_Dst_Color:
			return GL_DST_COLOR;

		case ABF_Inv_Src_Color:
			return GL_ONE_MINUS_SRC_COLOR;

		case ABF_Inv_Dst_Color:
			return GL_ONE_MINUS_DST_COLOR;

		case ABF_Src_Alpha_Sat:
			return GL_SRC_ALPHA_SATURATE;

		default:
			BOOST_ASSERT(false);
			return GL_ZERO;
		}
	}

	// ��RenderEngine::StencilOperationת����GLenum
	/////////////////////////////////////////////////////////////////////////////////
	GLenum OGLMapping::Mapping(StencilOperation op)
	{
		switch (op)
		{
		case SOP_Keep:
			return GL_KEEP;

		case SOP_Zero:
			return GL_ZERO;

		case SOP_Replace:
			return GL_REPLACE;

		case SOP_Increment:
			return GL_INCR;

		case SOP_Decrement:
			return GL_DECR;

		case SOP_Invert:
			return GL_INVERT;

		default:
			BOOST_ASSERT(false);
			return GL_KEEP;
		};
	}

	GLenum OGLMapping::Mapping(PolygonMode mode)
	{
		switch (mode)
		{
		case PM_Point:
			return GL_POINT;

		case PM_Line:
			return GL_LINE;

		case PM_Fill:
			return GL_FILL;

		default:
			BOOST_ASSERT(false);
			return GL_FILL;
		}
	}

	GLenum OGLMapping::Mapping(ShadeMode mode)
	{
		switch (mode)
		{
		case SM_Flat:
			return GL_FLAT;

		case SM_Gouraud:
			return GL_SMOOTH;

		default:
			BOOST_ASSERT(false);
			return GL_FLAT;
		}
	}

	GLenum OGLMapping::Mapping(BlendOperation bo)
	{
		switch (bo)
		{
		case BOP_Add:
			return GL_FUNC_ADD;

		case BOP_Sub:
			return GL_FUNC_SUBTRACT;

		case BOP_Rev_Sub:
			return GL_FUNC_REVERSE_SUBTRACT;

		case BOP_Min:
			return GL_MIN;

		case BOP_Max:
			return GL_MAX;

		default:
			BOOST_ASSERT(false);
			return GL_FUNC_ADD;
		}
	}

	GLint OGLMapping::Mapping(TexAddressingMode mode)
	{
		switch (mode)
		{
		case TAM_Wrap:
			return GL_REPEAT;

		case TAM_Mirror:
			return GL_MIRRORED_REPEAT;

		case TAM_Clamp:
			return GL_CLAMP_TO_EDGE;

		case TAM_Border:
			return GL_CLAMP_TO_BORDER;

		default:
			BOOST_ASSERT(false);
			return GL_REPEAT;
		}
	}

	void OGLMapping::Mapping(GLenum& primType, uint32_t& primCount, RenderLayout const & rl)
	{
		uint32_t const vertexCount = static_cast<uint32_t>(rl.UseIndices() ? rl.NumIndices() : rl.NumVertices());
		primType = GL_POINTS;
		primCount = vertexCount;
		switch (rl.TopologyType())
		{
		case RenderLayout::TT_PointList:
			primType = GL_POINTS;
			primCount = vertexCount;
			break;

		case RenderLayout::TT_LineList:
			primType = GL_LINES;
			primCount = vertexCount / 2;
			break;

		case RenderLayout::TT_LineStrip:
			primType = GL_LINE_STRIP;
			primCount = vertexCount - 1;
			break;

		case RenderLayout::TT_TriangleList:
			primType = GL_TRIANGLES;
			primCount = vertexCount / 3;
			break;

		case RenderLayout::TT_TriangleStrip:
			primType = GL_TRIANGLE_STRIP;
			primCount = vertexCount - 2;
			break;

		default:
			THR(boost::system::posix_error::not_supported);
		}
	}

	void OGLMapping::MappingFormat(GLint& internalFormat, GLenum& glformat, GLenum& gltype, ElementFormat ef)
	{
		switch (ef)
		{
		case EF_L8:
			internalFormat = GL_LUMINANCE8;
			glformat = GL_LUMINANCE;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_A8:
			internalFormat = GL_ALPHA8;
			glformat = GL_ALPHA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_AL4:
			internalFormat = GL_LUMINANCE4_ALPHA4;
			glformat = GL_LUMINANCE_ALPHA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_L16:
			internalFormat = GL_LUMINANCE16;
			glformat = GL_LUMINANCE;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_AL8:
			internalFormat = GL_LUMINANCE8_ALPHA8;
			glformat = GL_LUMINANCE_ALPHA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_AL16:
			internalFormat = GL_LUMINANCE16_ALPHA16;
			glformat = GL_LUMINANCE_ALPHA;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_R5G6B5:
			internalFormat = GL_RGB5;
			glformat = GL_BGR;
			gltype = GL_UNSIGNED_SHORT_5_6_5_REV;
			break;

		case EF_ARGB4:
			internalFormat = GL_RGBA4;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_SHORT_4_4_4_4_REV;
			break;

		case EF_RGB8:
			internalFormat = GL_RGB8;
			glformat = GL_BGR;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_ARGB8:
			internalFormat = GL_RGBA8;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_INT_8_8_8_8_REV;
			break;

		case EF_ABGR8:
			internalFormat = GL_RGBA8;
			glformat = GL_RGBA;
			gltype = GL_UNSIGNED_INT_8_8_8_8;
			break;

		case EF_A2BGR10:
			internalFormat = GL_RGB10_A2;
			glformat = GL_RGBA;
			gltype = GL_UNSIGNED_INT_10_10_10_2;
			break;

		case EF_BGR16:
			internalFormat = GL_RGB16;
			glformat = GL_BGR;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_ABGR16:
			internalFormat = GL_RGBA16;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_R16F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_R16F;
				glformat = GL_R;
				gltype = GL_HALF_FLOAT_ARB;
			}
			else
			{
				internalFormat = GL_LUMINANCE16F_ARB;
				glformat = GL_LUMINANCE;
				gltype = GL_HALF_FLOAT_ARB;
			}
			break;

		case EF_GR16F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_RG16F;
				glformat = GL_RG;
				gltype = GL_HALF_FLOAT_ARB;
			}
			else
			{
				internalFormat = GL_LUMINANCE_ALPHA16F_ARB;
				glformat = GL_LUMINANCE_ALPHA;
				gltype = GL_FLOAT;
			}
			break;

		case EF_BGR16F:
			internalFormat = GL_RGB16F_ARB;
			glformat = GL_RGB;
			gltype = GL_HALF_FLOAT_ARB;
			break;

		case EF_ABGR16F:
			internalFormat = GL_RGBA16F_ARB;
			glformat = GL_RGBA;
			gltype = GL_HALF_FLOAT_ARB;
			break;

		case EF_R32F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_R32F;
				glformat = GL_R;
				gltype = GL_FLOAT;
			}
			else
			{
				internalFormat = GL_LUMINANCE32F_ARB;
				glformat = GL_LUMINANCE;
				gltype = GL_FLOAT;
			}
			break;

		case EF_GR32F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_RG32F;
				glformat = GL_RG;
				gltype = GL_FLOAT;
			}
			else
			{
				internalFormat = GL_LUMINANCE_ALPHA32F_ARB;
				glformat = GL_LUMINANCE_ALPHA;
				gltype = GL_FLOAT;
			}
			break;

		case EF_BGR32F:
			internalFormat = GL_RGB32F_ARB;
			glformat = GL_RGB;
			gltype = GL_FLOAT;
			break;

		case EF_ABGR32F:
			internalFormat = GL_RGBA32F_ARB;
			glformat = GL_RGBA;
			gltype = GL_FLOAT;
			break;

		case EF_BC1:
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			glformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC2:
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			glformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC3:
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			glformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC4:
			internalFormat = GL_COMPRESSED_LUMINANCE_LATC1_EXT;
			glformat = GL_COMPRESSED_LUMINANCE;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC5:
			internalFormat = GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
			glformat = GL_COMPRESSED_LUMINANCE_ALPHA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_SIGNED_BC4:
			internalFormat = GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT;
			glformat = GL_COMPRESSED_LUMINANCE;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_SIGNED_BC5:
			internalFormat = GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT;
			glformat = GL_COMPRESSED_LUMINANCE_ALPHA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_ARGB8_SRGB:
			internalFormat = GL_SRGB8_ALPHA8_EXT;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_INT_8_8_8_8_REV;
			break;

		case EF_BC1_SRGB:
			internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC2_SRGB:
			internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC3_SRGB:
			internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC4_SRGB:
			internalFormat = GL_COMPRESSED_SLUMINANCE_EXT;
			glformat = GL_LUMINANCE;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_BC5_SRGB:
			internalFormat = GL_COMPRESSED_SLUMINANCE_ALPHA_EXT;
			glformat = GL_LUMINANCE_ALPHA;
			gltype = GL_UNSIGNED_BYTE;
			break;

		case EF_D16:
			internalFormat = GL_DEPTH_COMPONENT16;
			glformat = GL_DEPTH_COMPONENT;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_D24S8:
			internalFormat = GL_DEPTH24_STENCIL8_EXT;
			glformat = GL_DEPTH_STENCIL_EXT;
			gltype = GL_UNSIGNED_INT_24_8_EXT;
			break;

		case EF_D32F:
			internalFormat = GL_DEPTH_COMPONENT32F;
			glformat = GL_DEPTH_COMPONENT;
			gltype = GL_FLOAT;
			break;

		default:
			THR(boost::system::posix_error::not_supported);
		}
	}
}
