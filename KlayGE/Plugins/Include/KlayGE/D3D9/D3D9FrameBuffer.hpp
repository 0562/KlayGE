// D3D9FrameBuffer.hpp
// KlayGE D3D9��Ⱦ������ ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ��ΪFrameBuffer (2006.5.30)
//
// 2.4.0
// ȥ����OnLostDevice��OnResetDevice������texture���� (2005.3.3)
//
// 2.3.0
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9FRAMEBUFFER_HPP
#define _D3D9FRAMEBUFFER_HPP

#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9FrameBuffer : public FrameBuffer, public D3D9Resource
	{
	public:
		D3D9FrameBuffer();

		ID3D9SurfacePtr D3DRenderSurface(uint32_t n) const;
		ID3D9SurfacePtr D3DRenderZBuffer() const;

		void OnBind();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();
	};

	typedef boost::shared_ptr<D3D9FrameBuffer> D3D9FrameBufferPtr;
}

#endif			// _D3D9RENDERTEXTURE_HPP
