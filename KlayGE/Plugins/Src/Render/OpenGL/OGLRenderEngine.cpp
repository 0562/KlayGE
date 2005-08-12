// OGLRenderEngine.cpp
// KlayGE OpenGL��Ⱦ������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2004-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������RenderDeviceCaps (2005.7.17)
// ����StencilBuffer��ز��� (2005.7.20)
// ֻ֧��vbo (2005.7.31)
// ֻ֧��OpenGL 1.5������ (2005.8.12)
//
// 2.7.0
// ֧��vertex_buffer_object (2005.6.19)
// ֧��OpenGL 1.3������ (2005.6.26)
// ȥ����TextureCoordSet (2005.6.26)
// TextureAddressingMode, TextureFiltering��TextureAnisotropy�Ƶ�Texture�� (2005.6.27)
//
// 2.4.0
// ������PolygonMode (2005.3.20)
//
// 2.0.1
// ���ν��� (2003.10.11)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Light.hpp>
#include <KlayGE/Material.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderSettings.hpp>

#include <glloader/glloader.h>

#include <algorithm>
#include <cstring>
#include <boost/assert.hpp>

#include <KlayGE/OpenGL/OGLMapping.hpp>
#include <KlayGE/OpenGL/OGLRenderWindow.hpp>
#include <KlayGE/OpenGL/OGLTexture.hpp>
#include <KlayGE/OpenGL/OGLVertexStream.hpp>
#include <KlayGE/OpenGL/OGLIndexStream.hpp>
#include <KlayGE/OpenGL/OGLRenderEngine.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "glloader_d.lib")
#else
	#pragma comment(lib, "glloader.lib")
#endif
#pragma comment(lib, "glu32.lib")

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	OGLRenderEngine::OGLRenderEngine()
		: cullingMode_(RenderEngine::CM_None)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	OGLRenderEngine::~OGLRenderEngine()
	{
	}

	// ������Ⱦϵͳ������
	/////////////////////////////////////////////////////////////////////////////////
	std::wstring const & OGLRenderEngine::Name() const
	{
		static const std::wstring name(L"OpenGL Render Engine");
		return name;
	}

	// ��ʼ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StartRendering()
	{
		bool gotMsg;
		MSG  msg;

		::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

		RenderTarget& renderTarget = *this->ActiveRenderTarget(0);
		while (WM_QUIT != msg.message)
		{
			// ��������Ǽ���ģ��� PeekMessage()�Ա����ǿ����ÿ���ʱ����Ⱦ����
			// ��Ȼ, �� GetMessage() ���� CPU ռ����
			if (renderTarget.Active())
			{
				gotMsg = ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ? true : false;
			}
			else
			{
				gotMsg = ::GetMessage(&msg, NULL, 0, 0) ? true : false;
			}

			if (gotMsg)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				// �ڿ���ʱ����Ⱦ֡ (û�еȴ�����Ϣ)
				if (renderTarget.Active())
				{
					renderTarget.Update();
				}
			}
		}
	}

	// ���û�����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::AmbientLight(Color const & col)
	{
		GLfloat ambient[] = { col.r(), col.g(), col.b(), 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	}

	// ���������ɫ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::ClearColor(Color const & col)
	{
		glClearColor(col.r(), col.g(), col.b(), col.a());
	}

	// ���ù�Ӱ����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::ShadingType(ShadeOptions so)
	{
		glShadeModel(OGLMapping::Mapping(so));
	}

	// ��/�رչ�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::EnableLighting(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	RenderWindowPtr OGLRenderEngine::CreateRenderWindow(std::string const & name,
		RenderSettings const & settings)
	{
		RenderWindowPtr win(new OGLRenderWindow(name, settings));

		this->FillRenderDeviceCaps();
		renderTargets_.resize(caps_.max_simultaneous_rts);

		this->ActiveRenderTarget(0, win);

		this->DepthBufferDepthTest(settings.depthBuffer);
		this->DepthBufferDepthWrite(settings.depthBuffer);

		this->SetMaterial(Material(Color(1, 1, 1, 1)));

		return win;
	}

	// ���ü���ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::CullingMode(CullMode mode)
	{
		cullingMode_ = mode;

		switch (mode)
		{
		case CM_None:
			glDisable(GL_CULL_FACE);
			break;

		case CM_Clockwise:
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			break;

		case CM_AntiClockwise:
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);
			break;
		}
	}

	// ���ö�������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::PolygonMode(FillMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, OGLMapping::Mapping(mode));
	}

	// ���ù�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::SetLight(uint32_t index, Light const & light)
	{
		GLint lightIndex(GL_LIGHT0 + index);

		switch (light.lightType)
		{
		case Light::LT_Spot:
			glLightf(lightIndex, GL_SPOT_CUTOFF, light.spotOuter);
			break;

		default:
			glLightf(lightIndex, GL_SPOT_CUTOFF, 180.0f);
			break;
		}

		GLfloat ambient[4];
		OGLMapping::Mapping(ambient, light.ambient);
		glLightfv(lightIndex, GL_AMBIENT, ambient);

		GLfloat diffuse[4];
		OGLMapping::Mapping(diffuse, light.diffuse);
		glLightfv(lightIndex, GL_DIFFUSE, diffuse);

		GLfloat specular[4];
		OGLMapping::Mapping(specular, light.specular);
		glLightfv(lightIndex, GL_SPECULAR, specular);

		// Set position / direction
		GLfloat f4vals[4];
		switch (light.lightType)
		{
		case Light::LT_Point:
			f4vals[0] = light.position.x();
			f4vals[1] = light.position.y();
			f4vals[2] = light.position.z();
			f4vals[3] = 1.0f;
			glLightfv(lightIndex, GL_POSITION, f4vals);
			break;

		case Light::LT_Directional:
			f4vals[0] = -light.direction.x(); // GL light directions are in eye coords
			f4vals[1] = -light.direction.y();
			f4vals[2] = -light.direction.z(); // GL light directions are in eye coords
			f4vals[3] = 0.0f; // important!
			// In GL you set direction through position, but the
			//  w value of the vector being 0 indicates which it is
			glLightfv(lightIndex, GL_POSITION, f4vals);
			break;

		case Light::LT_Spot:
			f4vals[0] = light.position.x();
			f4vals[1] = light.position.y();
			f4vals[2] = light.position.z();
			f4vals[3] = 1.0f;
			glLightfv(lightIndex, GL_POSITION, f4vals);

			f4vals[0] = light.direction.x();
			f4vals[1] = light.direction.y();
			f4vals[2] = light.direction.z();
			f4vals[3] = 0.0f; 
			glLightfv(lightIndex, GL_SPOT_DIRECTION, f4vals);
		}

		glLightf(lightIndex, GL_CONSTANT_ATTENUATION, light.attenuationConst);
		glLightf(lightIndex, GL_LINEAR_ATTENUATION, light.attenuationLinear);
		glLightf(lightIndex, GL_QUADRATIC_ATTENUATION, light.attenuationQuad);
	}

	// ��/�ر�ĳ����Դ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::LightEnable(uint32_t index, bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_LIGHT0 + index);
		}
		else
		{
			glDisable(GL_LIGHT0 + index);
		}
	}

	// ʵ�������������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DoWorldMatrix()
	{
		glMatrixMode(GL_MODELVIEW);

		Matrix4 oglViewMat(viewMat_);

		oglViewMat(0, 2) = -oglViewMat(0, 2);
		oglViewMat(1, 2) = -oglViewMat(1, 2);
		oglViewMat(2, 2) = -oglViewMat(2, 2);
		oglViewMat(3, 2) = -oglViewMat(3, 2);

		Matrix4 const oglMat(worldMat_ * oglViewMat);
		glLoadMatrixf(&oglMat(0, 0));
	}

	// ���ù۲����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DoViewMatrix()
	{
		this->DoWorldMatrix();
	}

	// ʵ������Ͷ�����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DoProjectionMatrix()
	{
		Matrix4 oglMat(MathLib::LHToRH(projMat_));

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&oglMat(0, 0));
		glMatrixMode(GL_MODELVIEW);
	}

	// ���ò���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::SetMaterial(Material const & material)
	{
		GLfloat ambient[4];
		OGLMapping::Mapping(ambient, material.ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

		GLfloat diffuse[4];
		OGLMapping::Mapping(diffuse, material.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

		GLfloat specular[4];
		OGLMapping::Mapping(specular, material.specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
	}

	// ���õ�ǰ��ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DoActiveRenderTarget(uint32_t n, RenderTargetPtr renderTarget)
	{
		this->CullingMode(cullingMode_);

		Viewport const & vp(renderTarget->GetViewport());
		glViewport(vp.left, vp.top, vp.width, vp.height);
	}

	// ��ʼһ֡
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::BeginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DoRender(VertexBuffer const & vb)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		for (VertexBuffer::VertexStreamConstIterator iter = vb.VertexStreamBegin();
			iter != vb.VertexStreamEnd(); ++ iter)
		{
			OGLVertexStream& stream(static_cast<OGLVertexStream&>(*(*iter)));
			VertexStreamType type(stream.Type());

			switch (type)
			{
			// Vertex xyzs
			case VST_Positions:
				glEnableClientState(GL_VERTEX_ARRAY);
				stream.Active();
				glVertexPointer(3, GL_FLOAT, 0, NULL);
				break;
		
			case VST_Normals:
				glEnableClientState(GL_NORMAL_ARRAY);
				stream.Active();
				glNormalPointer(GL_FLOAT, 0, NULL);
				break;

			case VST_Diffuses:
				glEnableClientState(GL_COLOR_ARRAY);
				stream.Active();
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, NULL);
				break;

			case VST_TextureCoords0:
			case VST_TextureCoords1:
			case VST_TextureCoords2:
			case VST_TextureCoords3:
			case VST_TextureCoords4:
			case VST_TextureCoords5:
			case VST_TextureCoords6:
			case VST_TextureCoords7:
				glClientActiveTexture(GL_TEXTURE0 + type - VST_TextureCoords0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				stream.Active();
				glTexCoordPointer(static_cast<GLint>(stream.ElementsPerVertex()),
						GL_FLOAT, 0, NULL);
				break;
			}
		}

		size_t const vertexCount = vb.UseIndices() ? vb.NumIndices() : vb.NumVertices();
		GLenum mode;
		uint32_t primCount;
		OGLMapping::Mapping(mode, primCount, vb);

		numPrimitivesJustRendered_ += primCount;
		numVerticesJustRendered_ += vertexCount;

		if (vb.UseIndices())
		{
			OGLIndexStream& stream(static_cast<OGLIndexStream&>(*vb.GetIndexStream()));
			stream.Active();

			for (uint32_t i = 0; i < renderPasses_; ++ i)
			{
				renderEffect_->BeginPass(i);

				glDrawElements(mode, static_cast<GLsizei>(vb.NumIndices()),
					GL_UNSIGNED_SHORT, 0);

				renderEffect_->EndPass();
			}
		}
		else
		{
			for (uint32_t i = 0; i < renderPasses_; ++ i)
			{
				renderEffect_->BeginPass(i);
			
				glDrawArrays(mode, 0, static_cast<GLsizei>(vb.NumVertices()));

				renderEffect_->EndPass();
			}
		}
	}

	// ����һ֡
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::EndFrame()
	{
	}

	// ��/�ر���Ȳ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::AlphaBlend(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	// ����Alpha�������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::AlphaBlendFunction(AlphaBlendFactor src_factor, AlphaBlendFactor dst_factor)
	{
		glBlendFunc(OGLMapping::Mapping(src_factor), OGLMapping::Mapping(dst_factor));
	}

	// ��/�ر���Ȳ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DepthBufferDepthTest(bool enabled)
	{
		if (enabled)
		{
			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	// ��/�ر���Ȼ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DepthBufferDepthWrite(bool enabled)
	{
		glDepthMask(enabled ? GL_TRUE : GL_FALSE);
	}

	// ������ȱȽϺ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DepthBufferFunction(CompareFunction func)
	{
		glDepthFunc(OGLMapping::Mapping(func));
	}

	// �������ƫ��
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DepthBias(uint16_t bias)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_POLYGON_OFFSET_POINT);
		glEnable(GL_POLYGON_OFFSET_LINE);
		// Bias is in {0, 16}, scale the unit addition appropriately
		glPolygonOffset(1.0f, bias);
	}

	// ��/�ر�Alpha����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::AlphaTest(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_ALPHA_TEST);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
		}
	}
	
	// ����Alpha�ȽϺ����Ͳο�ֵ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::AlphaFunction(CompareFunction alphaFunction, float refValue)
	{
		glAlphaFunc(OGLMapping::Mapping(alphaFunction), refValue);
	}

	// ������Ч��
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::Fog(FogMode mode, Color const & color,
		float expDensity, float linearStart, float linearEnd)
	{
		if (Fog_None == mode)
		{
			glDisable(GL_FOG);
		}
		else
		{
			glEnable(GL_FOG);
			glFogi(GL_FOG_MODE, OGLMapping::Mapping(mode));

			GLfloat fogColor[4];
			OGLMapping::Mapping(fogColor, color);
			glFogfv(GL_FOG_COLOR, fogColor);

			glFogf(GL_FOG_DENSITY, expDensity);
			glFogf(GL_FOG_START, linearStart);
			glFogf(GL_FOG_END, linearEnd);
		}
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::SetSampler(uint32_t stage, SamplerPtr const & sampler)
	{
		TexturePtr texture = sampler->GetTexture();

		BOOST_ASSERT(dynamic_cast<OGLTexture const *>(texture.get()) != NULL);

		glActiveTexture(GL_TEXTURE0 + stage);

		OGLTexture& gl_tex = *static_cast<OGLTexture*>(texture.get());
		GLenum tex_type = gl_tex.GLType();
		if (!texture)
		{
			glDisable(tex_type);
		}
		else
		{
			glEnable(tex_type);
			gl_tex.GLBindTexture();

			glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, OGLMapping::Mapping(sampler->AddressingMode(Sampler::TAT_Addr_U)));
			glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, OGLMapping::Mapping(sampler->AddressingMode(Sampler::TAT_Addr_V)));
			glTexParameteri(tex_type, GL_TEXTURE_WRAP_R, OGLMapping::Mapping(sampler->AddressingMode(Sampler::TAT_Addr_W)));

			glTexParameterfv(tex_type, GL_TEXTURE_BORDER_COLOR, &sampler->BorderColor().r());

			switch (sampler->Filtering())
			{
			case Sampler::TFO_None:
			case Sampler::TFO_Point:
				glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;

			case Sampler::TFO_Bilinear:
				glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				break;

			case Sampler::TFO_Trilinear:
				glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;

			case Sampler::TFO_Anisotropic:
				if (caps_.max_texture_anisotropy != 0)
				{
					uint32_t anisotropy = std::min(caps_.max_texture_anisotropy, sampler->Anisotropy());
					glTexParameteri(tex_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
				}	
				break;

			default:
				BOOST_ASSERT(false);
				break;
			}

			glTexParameteri(tex_type, GL_TEXTURE_MAX_LEVEL, sampler->MaxMipLevel());

			{
				GLfloat bias;
				glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &bias);
				bias = std::min(sampler->MipMapLodBias(), bias);
				glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, bias);
			}

			{
				Matrix4 oglMat(MathLib::LHToRH(sampler->TextureMatrix()));

				glPushAttrib(GL_TRANSFORM_BIT);
				glMatrixMode(GL_TEXTURE);
				glLoadMatrixf(&oglMat(0, 0));
				glPopAttrib();
			}
		}
	}

	// �ر�ĳ������׶�
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DisableSampler(uint32_t stage)
	{
		glActiveTexture(GL_TEXTURE0 + stage);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glDisable(GL_TEXTURE_CUBE_MAP);
	}

	// ��ģ�建����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilCheckEnabled(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	// Ӳ���Ƿ�֧��ģ�建����
	/////////////////////////////////////////////////////////////////////////////////
	bool OGLRenderEngine::HasHardwareStencil()
	{
		return true;
	}

	// ����ģ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	uint16_t OGLRenderEngine::StencilBufferBitDepth()
	{
		return 8;
	}

		// ����ģ��ȽϺ������ο�ֵ������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferFunction(CompareFunction func, uint32_t refValue, uint32_t mask)
	{
		glStencilFunc(OGLMapping::Mapping(func), refValue, mask);
	}

	// ����ģ�建����ģ�����ʧ�ܣ���Ȳ���ʧ�ܺ�ͨ����Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferOperation(StencilOperation fail,
		StencilOperation depth_fail, StencilOperation pass)
	{
		glStencilOp(OGLMapping::Mapping(fail), OGLMapping::Mapping(depth_fail), OGLMapping::Mapping(pass));
	}

	// ����豸����
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::FillRenderDeviceCaps()
	{
		GLint temp;

		if (glloader_GL_VERSION_2_0() || glloader_GL_ARB_vertex_shader())
		{
			glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &temp);
			caps_.max_vertex_texture_units = temp;
		}
		else
		{
			caps_.max_vertex_texture_units = 0;
		}

		if (glloader_GL_VERSION_2_0()
			|| (glloader_GL_ARB_vertex_shader() && glloader_GL_ARB_fragment_shader()))
		{
			if (caps_.max_vertex_texture_units != 0)
			{
				caps_.max_shader_model = 3;
			}
			else
			{
				caps_.max_shader_model = 2;
			}
		}
		else
		{
			if (glloader_GL_ARB_vertex_program() && glloader_GL_ARB_fragment_program())
			{
				caps_.max_shader_model = 1;
			}
			else
			{
				caps_.max_shader_model = 0;
			}
		}

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &temp);
		caps_.max_texture_height = caps_.max_texture_width = temp;
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &temp);
		caps_.max_texture_depth = temp;
		
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &temp);
		caps_.max_texture_cube_size = temp;

		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &temp);
		caps_.max_textures_units = temp;

		if (glloader_GL_EXT_texture_filter_anisotropic())
		{
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &temp);
			caps_.max_texture_anisotropy = temp;
		}
		else
		{
			caps_.max_texture_anisotropy = 0;
		}

		caps_.max_user_clip_planes = 6;

		if (glloader_GL_VERSION_2_0() || glloader_GL_ARB_draw_buffers())
		{
			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &temp);
			caps_.max_simultaneous_rts	= temp;
		}
		else
		{
			caps_.max_simultaneous_rts	= 1;
		}

		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &temp);
		caps_.max_vertices = temp;
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &temp);
		caps_.max_indices = temp;

		caps_.texture_2d_filter_caps = Sampler::TFO_Point | Sampler::TFO_Bilinear | Sampler::TFO_Trilinear | Sampler::TFO_Anisotropic;
		caps_.texture_1d_filter_caps = caps_.texture_2d_filter_caps;
		caps_.texture_3d_filter_caps = caps_.texture_2d_filter_caps;
		caps_.texture_cube_filter_caps = caps_.texture_2d_filter_caps;
	}
}
