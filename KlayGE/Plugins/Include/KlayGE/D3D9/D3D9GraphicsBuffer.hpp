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
#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9Resource.hpp>
#include <KlayGE/D3D9/D3D9RenderView.hpp>

namespace KlayGE
{
	class D3D9GraphicsBuffer : public GraphicsBuffer, public D3D9Resource
	{
	public:
		explicit D3D9GraphicsBuffer(BufferUsage usage, uint32_t access_hint)
			: GraphicsBuffer(usage, access_hint),
				hw_buf_size_(0)
		{
		}

		virtual ~D3D9GraphicsBuffer()
		{
		}

	protected:
		uint32_t hw_buf_size_;
	};
	typedef boost::shared_ptr<D3D9GraphicsBuffer> D3D9GraphicsBufferPtr;


	class D3D9IndexBuffer : public D3D9GraphicsBuffer
	{
	public:
		explicit D3D9IndexBuffer(BufferUsage usage, uint32_t access_hint, ElementInitData* init_data);

		void CopyToBuffer(GraphicsBuffer& rhs);

		ID3D9IndexBufferPtr D3D9Buffer() const;
		void SwitchFormat(ElementFormat format);

	private:
		void DoResize();

		void* Map(BufferAccess ba);
		void Unmap();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		ElementFormat format_;

		ID3D9DevicePtr d3d_device_;
		ID3D9IndexBufferPtr buffer_;
	};
	typedef boost::shared_ptr<D3D9IndexBuffer> D3D9IndexBufferPtr;


	class D3D9VertexBuffer : public D3D9GraphicsBuffer
	{
	public:
		explicit D3D9VertexBuffer(BufferUsage usage, uint32_t access_hint, ElementInitData* init_data);

		void CopyToBuffer(GraphicsBuffer& rhs);

		ID3D9VertexBufferPtr D3D9Buffer() const;

	private:
		void DoResize();

		void* Map(BufferAccess ba);
		void Unmap();

	private:
		void DoOnLostDevice();
		void DoOnResetDevice();

	private:
		ID3D9DevicePtr d3d_device_;
		ID3D9VertexBufferPtr buffer_;
	};
	typedef boost::shared_ptr<D3D9VertexBuffer> D3D9VertexBufferPtr;
}

#endif			// _D3D9GRAPHICSBUFFER_HPP
