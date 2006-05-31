// GraphicsBuffer.hpp
// KlayGE ͼ�λ������� ͷ�ļ�
// Ver 3.2.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// ��IndexStream��VertexStream�ϲ���GraphicsBuffer (2006.1.9)
//
// 3.1.0
// ������ʵ���ͼ����� (2005.10.31)
//
// 3.0.0
// ֧���ڵ����д洢���ֳ�Ա (2005.10.15)
// ֧��instancing (2005.10.21)
//
// 2.8.0
// ������CopyToMemory (2005.7.24)
//
// 2.4.0
// ����ΪVertexBuffer (2005.3.7)
//
// 2.0.4
// �޸����������� (2004.3.16)
//
// 2.0.3
// ȥ����VO_2D (2004.3.1)
// ����vector������� (2004.3.13)
//
// 2.0.0
// ���ν��� (2003.8.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _GRAPHICSBUFFER_HPP
#define _GRAPHICSBUFFER_HPP

#include <KlayGE/PreDeclare.hpp>
#include <vector>
#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	enum BufferUsage
	{
		BU_Static,
		BU_Dynamic
	};
	enum BufferAccess
	{
		BA_Read_Only,
		BA_Write_Only,
		BA_Read_Write
	};

	class GraphicsBuffer
	{
	public:
		class Mapper : boost::noncopyable
		{
		public:
			Mapper(GraphicsBuffer& buffer, BufferAccess ba)
				: buffer_(buffer)
			{
				data_ = buffer_.Map(ba);
			}
			~Mapper()
			{
				buffer_.Unmap();
			}

			template <typename T>
			const T* Pointer() const
			{
				return static_cast<T*>(data_);
			}
			template <typename T>
			T* Pointer()
			{
				return static_cast<T*>(data_);
			}

		private:
			GraphicsBuffer& buffer_;
			void* data_;
		};

	public:
		explicit GraphicsBuffer(BufferUsage usage);
		virtual ~GraphicsBuffer();

		static GraphicsBufferPtr NullObject();

		void Resize(uint32_t size_in_byte);
		uint32_t Size() const
		{
			return size_in_byte_;
		}

		BufferUsage Usage() const
		{
			return usage_;
		}

		virtual void* Map(BufferAccess ba) = 0;
		virtual void Unmap() = 0;

		void CopyToBuffer(GraphicsBuffer& rhs);

		virtual RenderViewPtr CreateRenderView(uint32_t width, uint32_t height) = 0;

	private:
		virtual void DoResize() = 0;

	protected:
		BufferUsage usage_;

		uint32_t size_in_byte_;
	};
}

#endif		// _GRAPHICSBUFFER_HPP
