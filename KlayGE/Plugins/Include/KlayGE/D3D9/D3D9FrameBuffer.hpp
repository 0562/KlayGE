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

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_D3D9
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

namespace KlayGE
{
	class D3D9FrameBuffer : public FrameBuffer, public D3D9Resource
	{
	public:
		D3D9FrameBuffer();
		virtual ~D3D9FrameBuffer();

		ID3D9SurfacePtr D3DRenderSurface(uint32_t n) const;
		ID3D9SurfacePtr D3DRenderZBuffer() const;

		virtual std::wstring const & Description() const;

		virtual void OnBind();

		bool RequiresFlipping() const
		{
			return true;
		}

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();
	};

	typedef boost::shared_ptr<D3D9FrameBuffer> D3D9FrameBufferPtr;
}

#endif			// _D3D9RENDERTEXTURE_HPP
