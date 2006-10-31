// D3D9VertexBuffer.cpp
// KlayGE D3D9���㻺������ ʵ���ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.1.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/COMPtr.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/Context.hpp>

#include <algorithm>

#include <KlayGE/D3D9/D3D9RenderEngine.hpp>
#include <KlayGE/D3D9/D3D9RenderView.hpp>
#include <KlayGE/D3D9/D3D9GraphicsBuffer.hpp>

namespace KlayGE
{
	D3D9VertexBuffer::D3D9VertexBuffer(BufferUsage usage)
			: D3D9GraphicsBuffer(usage)
	{
	}

	void D3D9VertexBuffer::DoResize()
	{
		BOOST_ASSERT(size_in_byte_ != 0);

		if (this->Size() > hw_buf_size_)
		{
			D3D9RenderEngine const & renderEngine(*checked_cast<D3D9RenderEngine const *>(&Context::Instance().RenderFactoryInstance().RenderEngineInstance()));
			d3d_device_ = renderEngine.D3DDevice();

			IDirect3DVertexBuffer9* buffer;
			TIF(d3d_device_->CreateVertexBuffer(static_cast<UINT>(this->Size()),
					0, 0, D3DPOOL_MANAGED, &buffer, NULL));
			buffer_ = MakeCOMPtr(buffer);
			hw_buf_size_ = this->Size();
		}
	}

	void* D3D9VertexBuffer::Map(BufferAccess ba)
	{
		BOOST_ASSERT(buffer_);

		uint32_t flags = 0;
		switch (ba)
		{
		case BA_Read_Only:
			flags |= D3DLOCK_READONLY;
			break;

		case BA_Write_Only:
			break;

		case BA_Read_Write:
			break;
		}

		void* ret;
		TIF(buffer_->Lock(0, 0, &ret, D3DLOCK_NOSYSLOCK | flags));
		return ret;
	}

	void D3D9VertexBuffer::Unmap()
	{
		BOOST_ASSERT(buffer_);

		buffer_->Unlock();
	}

	ID3D9VertexBufferPtr D3D9VertexBuffer::D3D9Buffer() const
	{
		return buffer_;
	}

	void D3D9VertexBuffer::DoOnLostDevice()
	{
	}
	
	void D3D9VertexBuffer::DoOnResetDevice()
	{
	}
}
