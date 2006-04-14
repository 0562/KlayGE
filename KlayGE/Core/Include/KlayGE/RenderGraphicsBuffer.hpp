// RenderGraphicsBuffer.hpp
// KlayGE ��Ⱦͼ�λ������� ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ���ν��� (2006.4.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERGRAPHICSBUFFER_HPP
#define _RENDERGRAPHICSBUFFER_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderTarget.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{	
	class RenderGraphicsBuffer : public RenderTarget
	{
	public:
		virtual ~RenderGraphicsBuffer()
		{
		}

		static RenderGraphicsBufferPtr NullObject();

		virtual void Attach(GraphicsBufferPtr gb) = 0;
		virtual void Detach() = 0;

		void SwapBuffers()
		{
		}

		bool IsTexture() const
		{
			return false;
		}

	protected:
		GraphicsBufferPtr vs_;
	};
}

#endif			// _RENDERGRAPHICSBUFFER_HPP