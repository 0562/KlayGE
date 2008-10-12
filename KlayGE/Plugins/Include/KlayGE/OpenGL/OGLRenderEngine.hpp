// OGLRenderEngine.hpp
// KlayGE OpenGL��Ⱦ������ ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
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

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_OpenGL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/Vector.hpp>

#include <vector>
#include <map>

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

		void CreateRenderWindow(std::string const & name, RenderSettings const & settings);
		
		void StartRendering();

		void BeginFrame();
		void EndFrame();

		uint16_t StencilBufferBitDepth();

		void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		float4 TexelToPixelOffset() const
		{
			return float4(0, 0, 0, 0);
		}

		void Resize(uint32_t width, uint32_t height);
		bool FullScreen() const;
		void FullScreen(bool fs);

		void TexParameter(GLenum target, GLenum pname, GLint param);
		void TexEnv(GLenum target, GLenum pname, GLfloat param);

		void GetFBOForBlit(GLuint& src, GLuint& dst)
		{
			src = fbo_blit_src_;
			dst = fbo_blit_dst_;
		}

	private:
		void DoBindFrameBuffer(FrameBufferPtr fb);
		void DoRender(RenderTechnique const & tech, RenderLayout const & rl);

		void FillRenderDeviceCaps();
		void InitRenderStates();

	private:
		GLuint fbo_blit_src_;
		GLuint fbo_blit_dst_;
	};

	typedef boost::shared_ptr<OGLRenderEngine> OGLRenderEnginePtr;
}

#endif			// _OGLRENDERENGINE_HPP
