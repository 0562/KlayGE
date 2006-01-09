// D3D9VertexStream.hpp
// KlayGE D3D9�������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������CopyToMemory (2005.7.24)
//
// 2.4.0
// ��Ϊ������D3D9Resource (2005.3.3)
//
// 2.3.0
// ������OnLostDevice��OnResetDevice (2005.2.23)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9VERTEXSTREAM_HPP
#define _D3D9VERTEXSTREAM_HPP

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
	class D3D9VertexStream : public GraphicsBuffer, public D3D9Resource
	{
	public:
		explicit D3D9VertexStream(BufferUsage usage);

		void* Map(BufferAccess ba);
		void Unmap();

		boost::shared_ptr<IDirect3DVertexBuffer9> D3D9Buffer() const;

	private:
		void DoCreate();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		boost::shared_ptr<IDirect3DDevice9> d3d_device_;
		boost::shared_ptr<IDirect3DVertexBuffer9> buffer_;
	};

	typedef boost::shared_ptr<D3D9VertexStream> D3D9VertexStreamPtr;
}

#endif			// _D3D9VERTEXSTREAM_HPP
