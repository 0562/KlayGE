// D3D9RenderView.hpp
// KlayGE D3D9��Ⱦ��ͼ�� ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ���ν��� (2006.5.31)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERVIEW_HPP
#define _D3D9RENDERVIEW_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderView.hpp>
#include <KlayGE/Texture.hpp>

#include <boost/utility.hpp>

#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9Texture1D;
	class D3D9Texture2D;
	class D3D9Texture3D;
	class D3D9TextureCube;
	class D3D9GraphicsBuffer;

	class D3D9RenderView : public RenderView, public D3D9Resource
	{
	public:
		virtual ~D3D9RenderView();

		ID3D9SurfacePtr D3DRenderSurface() const
		{
			return surface_;
		}

	protected:
		ID3D9SurfacePtr surface_;
	};

	typedef boost::shared_ptr<D3D9RenderView> D3D9RenderViewPtr;


	class D3D9Texture1DRenderView : public D3D9RenderView, boost::noncopyable
	{
	public:
		D3D9Texture1DRenderView(Texture& texture_1d, int level);

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3D9Texture1D& texture_1d_;
		int level_;
	};

	typedef boost::shared_ptr<D3D9Texture1DRenderView> D3D9Texture1DRenderViewPtr;


	class D3D9Texture2DRenderView : public D3D9RenderView, boost::noncopyable
	{
	public:
		D3D9Texture2DRenderView(Texture& texture_2d, int level);

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3D9Texture2D& texture_2d_;
		int level_;
	};

	typedef boost::shared_ptr<D3D9Texture2DRenderView> D3D9Texture2DRenderViewPtr;


	class D3D9TextureCubeRenderView : public D3D9RenderView, boost::noncopyable
	{
	public:
		D3D9TextureCubeRenderView(Texture& texture_cube, Texture::CubeFaces face, int level);

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3D9TextureCube& texture_cube_;
		Texture::CubeFaces face_;
		int level_;
	};

	typedef boost::shared_ptr<D3D9TextureCubeRenderView> D3D9TextureCubeRenderViewPtr;


	class D3D9Texture3DRenderView : public D3D9RenderView, boost::noncopyable
	{
	public:
		D3D9Texture3DRenderView(Texture& texture_3d, int slice, int level);

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

	private:
		ID3D9SurfacePtr CreateSurface(D3DPOOL pool);

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3D9Texture3D& texture_3d_;
		int slice_;
		int level_;
	};

	typedef boost::shared_ptr<D3D9Texture3DRenderView> D3D9Texture3DRenderViewPtr;


	class D3D9GraphicsBufferRenderView : public D3D9RenderView, boost::noncopyable
	{
	public:
		D3D9GraphicsBufferRenderView(GraphicsBuffer& gb,
							uint32_t width, uint32_t height, PixelFormat pf);

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

	private:
		ID3D9SurfacePtr CreateGBSurface(D3DPOOL pool);

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		GraphicsBuffer& gbuffer_;
	};

	typedef boost::shared_ptr<D3D9GraphicsBufferRenderView> D3D9GraphicsBufferRenderViewPtr;


	class D3D9DepthStencilRenderView : public D3D9RenderView, boost::noncopyable
	{
	public:
		D3D9DepthStencilRenderView(uint32_t width, uint32_t height,
			PixelFormat pf, uint32_t multi_sample);

		void OnAttached(FrameBuffer& fb, uint32_t att);
		void OnDetached(FrameBuffer& fb, uint32_t att);

	private:
		ID3D9SurfacePtr CreateSurface();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		D3DMULTISAMPLE_TYPE multi_sample_;
	};

	typedef boost::shared_ptr<D3D9DepthStencilRenderView> D3D9DepthStencilRenderViewPtr;
}

#endif			// _D3D9RENDERVIEW_HPP
