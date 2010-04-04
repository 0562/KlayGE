// OGLQuery.hpp
// KlayGE OpenGL��ѯ�� ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2005-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ������ConditionalRender (2008.10.11)
//
// 3.0.0
// ���ν��� (2005.9.27)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/RenderFactory.hpp>

#include <glloader/glloader.h>

#include <KlayGE/OpenGL/OGLQuery.hpp>

namespace KlayGE
{
	OGLOcclusionQuery::OGLOcclusionQuery()
	{
		glGenQueries(1, &query_);
	}

	OGLOcclusionQuery::~OGLOcclusionQuery()
	{
		glDeleteQueries(1, &query_);
	}

	void OGLOcclusionQuery::Begin()
	{
		glBeginQuery(GL_SAMPLES_PASSED, query_);
	}

	void OGLOcclusionQuery::End()
	{
		glEndQuery(GL_SAMPLES_PASSED);
	}

	uint64_t OGLOcclusionQuery::SamplesPassed()
	{
		GLuint ret;
		glGetQueryObjectuiv(query_, GL_QUERY_RESULT, &ret);
		return static_cast<uint64_t>(ret);
	}


	OGLConditionalRender::OGLConditionalRender()
	{
		glGenQueries(1, &query_);
	}

	OGLConditionalRender::~OGLConditionalRender()
	{
		glDeleteQueries(1, &query_);
	}

	void OGLConditionalRender::Begin()
	{
		if (glloader_GL_VERSION_3_3() || glloader_GL_ARB_occlusion_query2())
		{
			glBeginQuery(GL_ANY_SAMPLES_PASSED, query_);
		}
		else
		{
			glBeginQuery(GL_SAMPLES_PASSED, query_);
		}
	}

	void OGLConditionalRender::End()
	{
		if (glloader_GL_VERSION_3_3() || glloader_GL_ARB_occlusion_query2())
		{
			glEndQuery(GL_ANY_SAMPLES_PASSED);
		}
		else
		{
			glEndQuery(GL_SAMPLES_PASSED);
		}
	}

	void OGLConditionalRender::BeginConditionalRender()
	{
		if (glloader_GL_VERSION_3_0())
		{
			glBeginConditionalRender(query_, GL_QUERY_WAIT);
		}
		else
		{
			if (glloader_GL_NV_conditional_render())
			{
				glBeginConditionalRenderNV(query_, GL_QUERY_WAIT_NV);
			}
		}
	}

	void OGLConditionalRender::EndConditionalRender()
	{
		if (glloader_GL_VERSION_3_0())
		{
			glEndConditionalRender();
		}
		else
		{
			if (glloader_GL_NV_conditional_render())
			{
				glEndConditionalRenderNV();
			}
		}
	}

	bool OGLConditionalRender::AnySamplesPassed()
	{
		GLuint ret;
		glGetQueryObjectuiv(query_, GL_QUERY_RESULT, &ret);
		return (ret != 0);
	}
}
