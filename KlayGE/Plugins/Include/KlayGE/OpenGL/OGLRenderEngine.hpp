// OGLRenderEngine.hpp
// KlayGE OpenGL��Ⱦ������ ͷ�ļ�
// Ver 3.12.0
// ��Ȩ����(C) ������, 2003-2010
// Homepage: http://www.klayge.org
//
// 3.11.0
// Remove TexelToPixelOffset (2010.9.26)
//
// 3.4.0
// ������TexelToPixelOffset (2006.8.27)
//
// 3.3.0
// ֻ֧��OpenGL 2.0 (2006.5.21)
//
// 3.0.0
// ȥ���˹̶���ˮ�� (2005.8.18)
//
// 2.8.0
// ������RenderDeviceCaps (2005.7.17)
// ����StencilBuffer��ز��� (2005.7.20)
//
// 2.7.0
// ȥ����TextureCoordSet (2005.6.26)
// TextureAddressingMode, TextureFiltering��TextureAnisotropy�Ƶ�Texture�� (2005.6.27)
//
// 2.4.0
// ������PolygonMode (2005.3.20)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERENGINE_HPP
#define _OGLRENDERENGINE_HPP

#pragma once

#include <KlayGE/Vector.hpp>

#include <vector>
#include <map>
#include <set>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 6385)
#endif
#include <boost/array.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif
#include <boost/tuple/tuple.hpp>

#include <glloader/glloader.h>

#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/ShaderObject.hpp>

namespace KlayGE
{
	class OGLRenderEngine : public RenderEngine
	{
	public:
		OGLRenderEngine();
		~OGLRenderEngine();

		std::wstring const & Name() const;

		void StartRendering();

		void BeginFrame();
		void EndFrame();
		void BeginPass();
		void EndPass();
		
		void ForceFlush();

		uint16_t StencilBufferBitDepth();

		void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		bool FullScreen() const;
		void FullScreen(bool fs);

		void AdjustPerspectiveMatrix(float4x4& pers_mat);

		void MipMapLodBias(uint32_t stage, float bias);
		void ActiveTexture(GLenum tex_unit);
		void BindBuffer(GLenum target, GLuint buffer);

		void ClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		void ClearDepth(GLfloat depth);
		void ClearStencil(GLuint stencil);

		void GetFBOForBlit(GLuint& src, GLuint& dst) const
		{
			src = fbo_blit_src_;
			dst = fbo_blit_dst_;
		}

		void BindFramebuffer(GLuint fbo, bool force = false);
		GLuint BindFramebuffer() const
		{
			return cur_fbo_;
		}

		bool HackForNV() const
		{
			return hack_for_nv_;
		}
		bool HackForATI() const
		{
			return hack_for_ati_;
		}

	private:
		void DoCreateRenderWindow(std::string const & name, RenderSettings const & settings);
		void DoBindFrameBuffer(FrameBufferPtr const & fb);
		void DoBindSOBuffers(RenderLayoutPtr const & rl);
		void DoRender(RenderTechnique const & tech, RenderLayout const & rl);
		void DoDispatch(RenderTechnique const & tech, uint32_t tgx, uint32_t tgy, uint32_t tgz);
		void DoResize(uint32_t width, uint32_t height);

		void FillRenderDeviceCaps();
		void InitRenderStates();

		bool VertexFormatSupport(ElementFormat elem_fmt);
		bool TextureFormatSupport(ElementFormat elem_fmt);
		bool RenderTargetFormatSupport(ElementFormat elem_fmt, uint32_t sample_count, uint32_t sample_quality);

	private:
		GLuint fbo_blit_src_;
		GLuint fbo_blit_dst_;

		boost::array<GLfloat, 4> clear_clr_;
		GLfloat clear_depth_;
		GLuint clear_stencil_;

		GLint vp_x_, vp_y_;
		GLsizei vp_width_, vp_height_;

		GLuint cur_fbo_;

		RenderLayoutPtr so_rl_;
		GLenum so_primitive_mode_;
		std::vector<std::string> so_vars_;
		std::vector<char const *> so_vars_ptrs_;
		std::vector<GLuint> so_buffs_;

		std::vector<GLfloat> mip_map_lod_bias_;
		GLenum active_tex_unit_;
		std::map<GLenum, GLuint> binded_buffer_;

		GLuint restart_index_;

		std::set<ElementFormat> vertex_format_;
		std::set<ElementFormat> texture_format_;
		std::set<ElementFormat> rendertarget_format_;
		uint32_t max_samples_;

		bool hack_for_nv_;
		bool hack_for_ati_;
	};

	typedef boost::shared_ptr<OGLRenderEngine> OGLRenderEnginePtr;
}

#endif			// _OGLRENDERENGINE_HPP
