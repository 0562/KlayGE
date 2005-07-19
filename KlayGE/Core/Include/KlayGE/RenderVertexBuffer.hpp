// RenderVertexBuffer.hpp
// KlayGE ��Ⱦ�����㻺����s�� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.19)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERVERTEXBUFFER_HPP
#define _RENDERVERTEXBUFFER_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderTarget.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{	
	class RenderVertexBuffer : public RenderTarget
	{
	public:
		virtual void AttachVertexBuffer(VertexBufferPtr vb) = 0;
		virtual void DeattachTexture() = 0;

		void SwapBuffers()
		{
		}

		bool IsTexture() const
		{
			return false;
		}

	protected:
		VertexBufferPtr vb_;
	};
}

#endif			// _RENDERVERTEXBUFFER_HPP