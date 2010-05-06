// OGLES2GraphicsBuffer.hpp
// KlayGE OpenGL ES 2ͼ�λ������� ʵ���ļ�
// Ver 3.10.0
// ��Ȩ����(C) ������, 2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// ���ν��� (2010.1.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <algorithm>

#include <KlayGE/OpenGLES2/OGLES2GraphicsBuffer.hpp>

namespace KlayGE
{
	OGLES2GraphicsBuffer::OGLES2GraphicsBuffer(BufferUsage usage, uint32_t access_hint, GLenum target, ElementInitData* init_data)
			: GraphicsBuffer(usage, access_hint),
				target_(target)
	{
		BOOST_ASSERT((GL_ARRAY_BUFFER == target) || (GL_ELEMENT_ARRAY_BUFFER == target));
		if (glloader_GLES_OES_mapbuffer())
		{
			THR(boost::system::posix_error::not_supported);
		}

		glGenBuffers(1, &vb_);

		if (init_data != NULL)
		{
			size_in_byte_ = init_data->row_pitch;

			glBindBuffer(target_, vb_);
			glBufferData(target_,
					static_cast<GLsizeiptr>(size_in_byte_), init_data->data,
					(BU_Static == usage_) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
			buf_data_.assign(static_cast<uint8_t const *>(init_data->data),
				static_cast<uint8_t const *>(init_data->data) + size_in_byte_);
		}
	}

	OGLES2GraphicsBuffer::~OGLES2GraphicsBuffer()
	{
		glDeleteBuffers(1, &vb_);
	}

	void OGLES2GraphicsBuffer::DoResize()
	{
		BOOST_ASSERT(size_in_byte_ != 0);

		glBindBuffer(target_, vb_);
		glBufferData(target_,
				static_cast<GLsizeiptr>(size_in_byte_), NULL,
				(BU_Static == usage_) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		buf_data_.resize(size_in_byte_);
	}

	void* OGLES2GraphicsBuffer::Map(BufferAccess ba)
	{
		GLenum flag = 0;
		switch (ba)
		{
		case BA_Write_Only:
			flag = GL_WRITE_ONLY_OES;
			break;

		default:
			BOOST_ASSERT(false);
			break;
		}

		if (glloader_GLES_OES_mapbuffer())
		{
			glBindBuffer(target_, vb_);
			return glMapBufferOES(target_, flag);
		}
		else
		{
			return &buf_data_[0];
		}
	}

	void OGLES2GraphicsBuffer::Unmap()
	{
		glBindBuffer(target_, vb_);
		if (glloader_GLES_OES_mapbuffer())
		{
			glUnmapBufferOES(target_);
		}
		else
		{
			glBufferSubData(target_, 0, static_cast<GLsizeiptr>(size_in_byte_), &buf_data_[0]);
		}
	}

	void OGLES2GraphicsBuffer::Active()
	{
		glBindBuffer(target_, vb_);
	}

	void OGLES2GraphicsBuffer::CopyToBuffer(GraphicsBuffer& rhs)
	{
		GraphicsBuffer::Mapper lhs_mapper(*this, BA_Read_Only);
		GraphicsBuffer::Mapper rhs_mapper(rhs, BA_Write_Only);
		std::copy(lhs_mapper.Pointer<uint8_t>(), lhs_mapper.Pointer<uint8_t>() + size_in_byte_,
			rhs_mapper.Pointer<uint8_t>());
	}
}
