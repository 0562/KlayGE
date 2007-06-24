// OGLMapping.hpp
// KlayGE RenderEngine和OpenGL本地之间的映射 实现文件
// Ver 3.0.0
// 版权所有(C) 龚敏敏, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// 增加了TAM_Border (2005.8.30)
//
// 2.8.0
// 初次建立 (2005.7.19)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Vector.hpp>
#include <KlayGE/Matrix.hpp>
#include <KlayGE/Color.hpp>
#include <KlayGE/RenderLayout.hpp>

#include <boost/assert.hpp>

#include <glloader/glloader.h>

#include <KlayGE/OpenGL/OGLTexture.hpp>
#include <KlayGE/OpenGL/OGLMapping.hpp>

namespace KlayGE
{
	// 从KlayGE的Color转换到float[4]
	/////////////////////////////////////////////////////////////////////////////////
	void OGLMapping::Mapping(GLfloat* clr4, Color const & clr)
	{
		clr4[0] = clr.r();
		clr4[1] = clr.g();
		clr4[2] = clr.b();
		clr4[3] = clr.a();
	}

	// 从RenderEngine::CompareFunction转换到GLenum
	/////////////////////////////////////////////////////////////////////////////////
	GLenum OGLMapping::Mapping(RenderStateObject::CompareFunction func)
	{
		switch (func)
		{
		case RenderStateObject::CF_AlwaysFail:
			return GL_NEVER;

		case RenderStateObject::CF_AlwaysPass:
			return GL_ALWAYS;

		case RenderStateObject::CF_Less:
			return GL_LESS;

		case RenderStateObject::CF_LessEqual:
			return GL_LEQUAL;

		case RenderStateObject::CF_Equal:
			return GL_EQUAL;

		case RenderStateObject::CF_NotEqual:
			return GL_NOTEQUAL;

		case RenderStateObject::CF_GreaterEqual:
			return GL_GEQUAL;

		case RenderStateObject::CF_Greater:
			return GL_GREATER;

		default:
			BOOST_ASSERT(false);
			return GL_EQUAL;
		};
	}

	// 从RenderEngine::AlphaBlendFactor转换到GLenum
	/////////////////////////////////////////////////////////////////////////////////
	GLenum OGLMapping::Mapping(RenderStateObject::AlphaBlendFactor factor)
	{
		switch (factor)
		{
		case RenderStateObject::ABF_Zero:
			return GL_ZERO;

		case RenderStateObject::ABF_One:
			return GL_ONE;

		case RenderStateObject::ABF_Src_Alpha:
			return GL_SRC_ALPHA;

		case RenderStateObject::ABF_Dst_Alpha:
			return GL_DST_ALPHA;

		case RenderStateObject::ABF_Inv_Src_Alpha:
			return GL_ONE_MINUS_SRC_ALPHA;

		case RenderStateObject::ABF_Inv_Dst_Alpha:
			return GL_ONE_MINUS_DST_ALPHA;

		case RenderStateObject::ABF_Src_Color:
			return GL_SRC_COLOR;

		case RenderStateObject::ABF_Dst_Color:
			return GL_DST_COLOR;

		case RenderStateObject::ABF_Inv_Src_Color:
			return GL_ONE_MINUS_SRC_COLOR;

		case RenderStateObject::ABF_Inv_Dst_Color:
			return GL_ONE_MINUS_DST_COLOR;

		case RenderStateObject::ABF_Src_Alpha_Sat:
			return GL_SRC_ALPHA_SATURATE;

		default:
			BOOST_ASSERT(false);
			return GL_ZERO;
		}
	}

	// 从RenderEngine::StencilOperation转换到GLenum
	/////////////////////////////////////////////////////////////////////////////////
	GLenum OGLMapping::Mapping(RenderStateObject::StencilOperation op)
	{
		switch (op)
		{
		case RenderStateObject::SOP_Keep:
			return GL_KEEP;

		case RenderStateObject::SOP_Zero:
			return GL_ZERO;

		case RenderStateObject::SOP_Replace:
			return GL_REPLACE;

		case RenderStateObject::SOP_Increment:
			return GL_INCR;

		case RenderStateObject::SOP_Decrement:
			return GL_DECR;

		case RenderStateObject::SOP_Invert:
			return GL_INVERT;

		default:
			BOOST_ASSERT(false);
			return GL_KEEP;
		};
	}

	GLenum OGLMapping::Mapping(RenderStateObject::PolygonMode mode)
	{
		switch (mode)
		{
		case RenderStateObject::PM_Point:
			return GL_POINT;

		case RenderStateObject::PM_Line:
			return GL_LINE;

		case RenderStateObject::PM_Fill:
			return GL_FILL;

		default:
			BOOST_ASSERT(false);
			return GL_FILL;
		}
	}

	GLenum OGLMapping::Mapping(RenderStateObject::ShadeMode mode)
	{
		switch (mode)
		{
		case RenderStateObject::SM_Flat:
			return GL_FLAT;

		case RenderStateObject::SM_Gouraud:
			return GL_SMOOTH;

		default:
			BOOST_ASSERT(false);
			return GL_FLAT;
		}
	}

	GLenum OGLMapping::Mapping(RenderStateObject::BlendOperation bo)
	{
		switch (bo)
		{
		case RenderStateObject::BOP_Add:
			return GL_FUNC_ADD;

		case RenderStateObject::BOP_Sub:
			return GL_FUNC_SUBTRACT;

		case RenderStateObject::BOP_Rev_Sub:
			return GL_FUNC_REVERSE_SUBTRACT;

		case RenderStateObject::BOP_Min:
			return GL_MIN;

		case RenderStateObject::BOP_Max:
			return GL_MAX;

		default:
			BOOST_ASSERT(false);
			return GL_FUNC_ADD;
		}
	}

	GLint OGLMapping::Mapping(Sampler::TexAddressingMode mode)
	{
		switch (mode)
		{
		case Sampler::TAM_Wrap:
			return GL_REPEAT;

		case Sampler::TAM_Mirror:
			return GL_MIRRORED_REPEAT;

		case Sampler::TAM_Clamp:
			return GL_CLAMP_TO_EDGE;

		case Sampler::TAM_Border:
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
			BOOST_ASSERT(false);
			break;
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

		case EF_A2RGB10:
			internalFormat = GL_RGB10_A2;
			glformat = GL_BGRA;
			gltype = GL_UNSIGNED_INT_2_10_10_10_REV;
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
			internalFormat = GL_LUMINANCE16F_ARB;
			glformat = GL_LUMINANCE;
			gltype = GL_HALF_FLOAT_ARB;
			break;

		case EF_GR16F:
			internalFormat = GL_LUMINANCE_ALPHA16F_ARB;
			glformat = GL_LUMINANCE_ALPHA;
			gltype = GL_HALF_FLOAT_ARB;
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
			internalFormat = GL_LUMINANCE32F_ARB;
			glformat = GL_LUMINANCE;
			gltype = GL_FLOAT;
			break;

		case EF_GR32F:
			internalFormat = GL_LUMINANCE_ALPHA32F_ARB;
			glformat = GL_LUMINANCE_ALPHA;
			gltype = GL_FLOAT;
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

		case EF_D32:
			internalFormat = GL_DEPTH_COMPONENT32;
			glformat = GL_DEPTH_COMPONENT;
			gltype = GL_UNSIGNED_INT;
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}
	}
}
