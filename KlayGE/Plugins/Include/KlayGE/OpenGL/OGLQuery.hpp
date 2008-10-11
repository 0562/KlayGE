// OGLOcclusionQuery.hpp
// KlayGE OpenGL�ڵ������ ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ���ν��� (2005.9.27)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLOCCLUSIONQUERY_HPP
#define _OGLOCCLUSIONQUERY_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_OpenGL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/Query.hpp>

namespace KlayGE
{
	class OGLOcclusionQuery : public OcclusionQuery
	{
	public:
		OGLOcclusionQuery();
		~OGLOcclusionQuery();

		void Begin();
		void End();

		uint64_t SamplesPassed();

	private:
		GLuint query_;
	};

	class OGLConditionalRender : public ConditionalRender
	{
	public:
		OGLConditionalRender();
		~OGLConditionalRender();

		void Begin();
		void End();

		void BeginConditionalRender();
		void EndConditionalRender();

	private:
		GLuint query_;
	};
}

#endif		// _OGLQUERY_HPP
