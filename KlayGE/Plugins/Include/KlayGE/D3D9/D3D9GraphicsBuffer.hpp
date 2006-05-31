// D3D9GraphicsBuffer.hpp
// KlayGE D3D9ͼ�λ������� ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.1.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9GRAPHICSBUFFER_HPP
#define _D3D9GRAPHICSBUFFER_HPP

#include <boost/smart_ptr.hpp>

#include <d3d9.h>

#include <KlayGE/GraphicsBuffer.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9IndexBuffer : public GraphicsBuffer, public D3D9Resource
	{
	public:
		explicit D3D9IndexBuffer(BufferUsage usage);

		void* Map(BufferAccess ba);
		void Unmap();

		RenderViewPtr CreateRenderView(uint32_t width, uint32_t height);

		boost::shared_ptr<IDirect3DIndexBuffer9> D3D9Buffer() const;
		void SwitchFormat(IndexFormat format);

	private:
		void DoResize();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		IndexFormat format_;

		boost::shared_ptr<IDirect3DDevice9> d3d_device_;
		boost::shared_ptr<IDirect3DIndexBuffer9> buffer_;
	};
	typedef boost::shared_ptr<D3D9IndexBuffer> D3D9IndexBufferPtr;

	class D3D9VertexBuffer : public GraphicsBuffer, public D3D9Resource
	{
	public:
		explicit D3D9VertexBuffer(BufferUsage usage);

		void* Map(BufferAccess ba);
		void Unmap();

		RenderViewPtr CreateRenderView(uint32_t width, uint32_t height);

		boost::shared_ptr<IDirect3DVertexBuffer9> D3D9Buffer() const;

	private:
		void DoResize();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		boost::shared_ptr<IDirect3DDevice9> d3d_device_;
		boost::shared_ptr<IDirect3DVertexBuffer9> buffer_;
	};
	typedef boost::shared_ptr<D3D9VertexBuffer> D3D9VertexBufferPtr;
}

#endif			// _D3D9GRAPHICSBUFFER_HPP
