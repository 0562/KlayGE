// OGLRenderEngine.hpp
// KlayGE OpenGL��Ⱦ������ ͷ�ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
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

#include <vector>
#include <map>

#define NOMINMAX
#include <windows.h>
#include <glloader/glloader.h>

#include <KlayGE/RenderEngine.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLRenderEngine : public RenderEngine
	{
	public:
		OGLRenderEngine();
		~OGLRenderEngine();

		std::wstring const & Name() const;

		void ClearColor(Color const & clr);
		void Clear(uint32_t masks);

		void ShadingType(ShadeOptions so);

		RenderWindowPtr CreateRenderWindow(std::string const & name, RenderSettings const & settings);

		void CullingMode(CullMode mode);
		void PolygonMode(FillMode mode);

		void StartRendering();

		void BeginFrame();
		void EndFrame();

		void AlphaBlend(bool enabled);
		void AlphaBlendFunction(AlphaBlendFactor src_factor, AlphaBlendFactor dst_factor);

		void DepthBufferDepthTest(bool enabled);
		void DepthBufferDepthWrite(bool enabled);
		void DepthBufferFunction(CompareFunction depthFunction);
		void DepthBias(uint16_t bias);

		void AlphaTest(bool enabled);
		void AlphaFunction(CompareFunction alphaFunction, float refValue);

		void SetSampler(uint32_t stage, SamplerPtr const & sampler);
		void DisableSampler(uint32_t stage);

		void StencilCheckEnabled(bool enabled);
		bool HasHardwareStencil();

		uint16_t StencilBufferBitDepth();

		void StencilBufferFunction(CompareFunction func, uint32_t refValue, uint32_t mask);
		void StencilBufferOperation(StencilOperation fail, StencilOperation depth_fail, StencilOperation pass);

		void PointSpriteEnable(bool enable);
		void PointDistanceAttenuation(float quadratic0, float quadratic1, float quadratic2);
		void PointSize(float size);
		void PointMinMaxSize(float min_size, float max_size);

		void ScissorTest(bool enabled);
		void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	private:
		void DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget);

		void DoRender(RenderLayout const & rl);

		void FillRenderDeviceCaps();

	private:
		CullMode cullingMode_;

		glPointParameterfFUNC glPointParameterf_;
		glPointParameterfvFUNC glPointParameterfv_;
	};

	typedef boost::shared_ptr<OGLRenderEngine> OGLRenderEnginePtr;
}

#endif			// _OGLRENDERENGINE_HPP
