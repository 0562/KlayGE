// D3D9IBConverter.cpp
// KlayGE ��KlayGE::VertexBufferת����D3D9IB���� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/Engine.hpp>
#include <KlaygE/RenderBuffer.hpp>

#include <KlayGE/D3D9/D3D9IndexStream.hpp>
#include <KlayGE/D3D9/D3D9IBConverter.hpp>

namespace KlayGE
{
	// ����D3D9��VertexBuffer
	/////////////////////////////////////////////////////////////////////////////////
	COMPtr<IDirect3DIndexBuffer9> D3D9IBConverter::Update(const RenderBuffer& rb)
	{
		D3D9IndexStream& d3dis(static_cast<D3D9IndexStream&>(*rb.GetIndexStream()));

		COMPtr<IDirect3DIndexBuffer9> d3dib;
		if (d3dis.IsStatic())
		{
			size_t size;
			d3dis.D3D9Buffer(d3dib, size);
		}
		else
		{
			d3dis.D3D9Buffer(indexPool_.first, indexPool_.second);
			d3dib = indexPool_.first;
		}

		return d3dib;
	}
}
