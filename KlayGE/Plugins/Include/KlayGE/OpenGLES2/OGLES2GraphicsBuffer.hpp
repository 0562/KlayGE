// OGLES2GraphicsBuffer.hpp
// KlayGE OpenGL ES 2ͼ�λ������� ͷ�ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://www.klayge.org
//
// 3.10.0
// ���ν��� (2010.1.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLES2GRAPHICSBUFFERHPP
#define _OGLES2GRAPHICSBUFFERHPP

#pragma once

#include <vector>
#include <glloader/glloader.h>

#include <KlayGE/GraphicsBuffer.hpp>

namespace KlayGE
{
	class OGLES2GraphicsBuffer : public GraphicsBuffer
	{
	public:
		explicit OGLES2GraphicsBuffer(BufferUsage usage, uint32_t access_hint, GLenum target, ElementInitData* init_data);
		~OGLES2GraphicsBuffer();

		void CopyToBuffer(GraphicsBuffer& rhs);

		void Active();

		GLuint OGLvbo() const
		{
			return vb_;
		}

	private:
		void DoResize();

		void* Map(BufferAccess ba);
		void Unmap();

	private:
		GLuint vb_;
		GLenum target_;
		std::vector<uint8_t> buf_data_;
	};
}

#endif			// _OGLES2GRAPHICSBUFFERHPP
