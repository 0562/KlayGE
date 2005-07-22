// OGLRenderVertexStream.hpp
// KlayGE OpenGL��Ⱦ���������� ͷ�ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ���ν��� (2005.7.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDERVERTEXSTREAM_HPP
#define _OGLRENDERVERTEXSTREAM_HPP

#include <KlayGE/RenderVertexStream.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLRenderVertexStream : public RenderVertexStream
	{
	public:
		OGLRenderVertexStream();

		void Attach(VertexStreamPtr vs);
		void Detach();

		virtual void CustomAttribute(std::string const & name, void* pData);

		bool RequiresTextureFlipping() const
			{ return false; }
	};

	typedef boost::shared_ptr<OGLRenderVertexStream> OGLRenderVertexStreamPtr;
}

#endif			// _OGLRENDERVERTEXSTREAM_HPP
