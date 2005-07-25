// D3D9RenderVertexStream.hpp
// KlayGE D3D9��Ⱦ���������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERVERTEXSTREAM_HPP
#define _D3D9RENDERVERTEXSTREAM_HPP

#include <KlayGE/RenderVertexStream.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9RenderVertexStream : public RenderVertexStream, public D3D9Resource
	{
		typedef boost::shared_ptr<IDirect3DSurface9> IDirect3DSurface9Ptr;

	public:
		D3D9RenderVertexStream(uint32_t width, uint32_t height);

		/// MUST call Detach before use the VertexStream
		void Attach(VertexStreamPtr vs);
		void Detach();

		boost::shared_ptr<IDirect3DSurface9> D3DRenderSurface() const;
		boost::shared_ptr<IDirect3DSurface9> D3DRenderZBuffer() const;

		void CustomAttribute(std::string const & name, void* pData);

		bool RequiresTextureFlipping() const
			{ return false; }

	private:
		IDirect3DSurface9Ptr CreateSurface(D3DPOOL pool);
		void CopyToVertexStream();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		boost::shared_ptr<IDirect3DDevice9> d3d_device_;

		IDirect3DSurface9Ptr render_surf_;
		D3DFORMAT fmt_;
	};

	typedef boost::shared_ptr<D3D9RenderVertexStream> D3D9RenderVertexStreamPtr;
}

#endif			// _D3D9RENDERVERTEXSTREAM_HPP
