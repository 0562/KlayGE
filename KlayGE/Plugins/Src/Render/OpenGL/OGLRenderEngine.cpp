// OGLRenderEngine.cpp
// KlayGE OpenGL��Ⱦ������ ʵ���ļ�
// Ver 2.0.1
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.1
// ���ν��� (2003.10.11)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Memory.hpp>

#include <KlayGE/Light.hpp>
#include <KlayGE/Material.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/RenderBuffer.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/OpenGL/OGLRenderSettings.hpp>
#include <KlayGE/OpenGL/OGLRenderWindow.hpp>
#include <KlayGE/OpenGL/OGLTexture.hpp>
#include <KlayGE/OpenGL/OGLVertexStream.hpp>
#include <KlayGE/OpenGL/OGLIndexStream.hpp>

#include <cassert>
#include <algorithm>

#include <KlayGE/OpenGL/OGLRenderEngine.hpp>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

namespace KlayGE
{
	// ��RenderEngine::CompareFunctionת����D3DCMPFUNC
	/////////////////////////////////////////////////////////////////////////////////
	GLint Convert(RenderEngine::CompareFunction func)
	{
		GLint ret;
		switch (func)
		{
		case RenderEngine::CF_AlwaysFail:
			ret = GL_NEVER;
			break;

		case RenderEngine::CF_AlwaysPass:
			ret = GL_ALWAYS;
			break;

		case RenderEngine::CF_Less:
			ret = GL_LESS;
			break;

		case RenderEngine::CF_LessEqual:
			ret = GL_LEQUAL;
			break;

		case RenderEngine::CF_Equal:
			ret = GL_EQUAL;
			break;

		case RenderEngine::CF_NotEqual:
			ret = GL_NOTEQUAL;
			break;

		case RenderEngine::CF_GreaterEqual:
			ret = GL_GEQUAL;
			break;

		case RenderEngine::CF_Greater:
			ret = GL_GREATER;
			break;
		};

		return ret;
	}

	// ��RenderEngine::StencilOperationת����D3DSTENCILOP
	/////////////////////////////////////////////////////////////////////////////////
	GLint Convert(RenderEngine::StencilOperation op)
	{
		GLint ret;
		switch (op)
		{
		case RenderEngine::SOP_Keep:
			ret = GL_KEEP;
			break;

		case RenderEngine::SOP_Zero:
			ret = GL_ZERO;
			break;

		case RenderEngine::SOP_Replace:
			ret = GL_REPLACE;
			break;

		case RenderEngine::SOP_Increment:
			ret = GL_INCR;
			break;

		case RenderEngine::SOP_Decrement:
			ret = GL_DECR;
			break;

		case RenderEngine::SOP_Invert:
			ret = GL_INVERT;
			break;
		};

		return ret;
	}
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	OGLRenderEngine::OGLRenderEngine()
		: cullingMode_(RenderEngine::Cull_None)
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
		static const std::wstring name(L"OpenGL Render System");
		return name;
	}

	// ��ʼ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StartRendering()
	{
		bool gotMsg;
		MSG  msg;

		::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

		while (WM_QUIT != msg.message)
		{
			// ��������Ǽ���ģ��� PeekMessage()�Ա����ǿ����ÿ���ʱ����Ⱦ����
			// ��Ȼ, �� GetMessage() ���� CPU ռ����
			if ((*RenderEngine::ActiveRenderTarget())->Active())
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
				if ((*RenderEngine::ActiveRenderTarget())->Active())
				{
					(*RenderEngine::ActiveRenderTarget())->Update();
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
		GLenum shadeMode;
		switch (so)
		{
		case SO_Flat:
			shadeMode = GL_FLAT;
			break;

		case SO_Gouraud:
			shadeMode = GL_SMOOTH;
			break;

		case SO_Phong:
			shadeMode = GL_SMOOTH;
			break;
		}

		glShadeModel(shadeMode);
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
		RenderWindowPtr win(new OGLRenderWindow(name,
			static_cast<OGLRenderSettings const &>(settings)));

		this->ActiveRenderTarget(this->AddRenderTarget(win));

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
		case Cull_None:
			glDisable(GL_CULL_FACE);
			break;

		case Cull_Clockwise:
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			break;

		case Cull_AntiClockwise:
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);
			break;
		}
	}

	// ���ù�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::SetLight(U32 index, Light const & light)
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

		GLfloat ambient[4] = { light.ambient.r(), light.ambient.g(), light.ambient.b(), light.ambient.a() };
		glLightfv(lightIndex, GL_AMBIENT, ambient);

		GLfloat diffuse[4] = { light.diffuse.r(), light.diffuse.g(), light.diffuse.b(), light.diffuse.a() };
		glLightfv(lightIndex, GL_DIFFUSE, diffuse);

		GLfloat specular[4] = { light.specular.r(), light.specular.g(), light.specular.b(), light.specular.a() };
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
	void OGLRenderEngine::LightEnable(U32 index, bool enabled)
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
		Matrix4 oglMat;
		MathLib::LHToRH(oglMat, projMat_);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&oglMat(0, 0));
		glMatrixMode(GL_MODELVIEW);
	}

	// ���ò���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::SetMaterial(Material const & material)
	{
		GLfloat ambient[4] = { material.ambient.r(), material.ambient.g(), material.ambient.b(), material.ambient.a() };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

		GLfloat diffuse[4] = { material.diffuse.r(), material.diffuse.g(), material.diffuse.b(), material.diffuse.a() };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

		GLfloat specular[4] = { material.specular.r(), material.specular.g(), material.specular.b(), material.specular.a() };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
	}

	// ���õ�ǰ��ȾĿ�꣬����ȾĿ������Ѿ����б���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::ActiveRenderTarget(RenderTargetListIterator iter)
	{
		RenderEngine::ActiveRenderTarget(iter);

		this->CullingMode(cullingMode_);

		Viewport const & vp((*iter)->GetViewport());
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
	void OGLRenderEngine::Render(RenderBuffer const & vb)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		for (RenderBuffer::VertexStreamConstIterator iter = vb.VertexStreamBegin();
			iter != vb.VertexStreamEnd(); ++ iter)
		{
			OGLVertexStream& stream(static_cast<OGLVertexStream&>(*(*iter)));
			VertexStreamType type(stream.Type());

			std::vector<U8> const & data(stream.OGLBuffer());

			switch (type)
			{
			// Vertex xyzs
			case VST_Positions:
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, &data[0]);
				break;
		
			case VST_Normals:
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, &data[0]);
				break;

			case VST_Diffuses:
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, &data[0]);
				break;

			case VST_TextureCoords0:
			case VST_TextureCoords1:
			case VST_TextureCoords2:
			case VST_TextureCoords3:
			case VST_TextureCoords4:
			case VST_TextureCoords5:
			case VST_TextureCoords6:
			case VST_TextureCoords7:
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(static_cast<GLint>(stream.ElementsPerVertex()),
					GL_FLOAT, 0, &data[0]);
				break;
			}
		}

		GLenum mode;
		switch (vb.Type())
		{
		case RenderBuffer::BT_PointList:
			mode = GL_POINTS;
			break;

		case RenderBuffer::BT_LineList:
			mode = GL_LINES;
			break;

		case RenderBuffer::BT_LineStrip:
			mode = GL_LINE_STRIP;
			break;

		case RenderBuffer::BT_TriangleList:
			mode = GL_TRIANGLES;
			break;

		case RenderBuffer::BT_TriangleStrip:
			mode = GL_TRIANGLE_STRIP;
			break;

		case RenderBuffer::BT_TriangleFan:
			mode = GL_TRIANGLE_FAN;
			break;
		}

		if (vb.UseIndices())
		{
			OGLIndexStream& stream(static_cast<OGLIndexStream&>(*vb.GetIndexStream()));
			std::vector<U16> const & data(stream.OGLBuffer());
			glDrawElements(mode, static_cast<GLsizei>(vb.NumIndices()),
				GL_UNSIGNED_SHORT, &data[0]);
		}
		else
		{
			glDrawArrays(mode, 0, static_cast<GLsizei>(vb.NumVertices()));
		}
	}

	// ����һ֡
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::EndFrame()
	{
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
		glDepthFunc(Convert(func));
	}

	// �������ƫ��
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DepthBias(U16 bias)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_POLYGON_OFFSET_POINT);
		glEnable(GL_POLYGON_OFFSET_LINE);
		// Bias is in {0, 16}, scale the unit addition appropriately
		glPolygonOffset(1.0f, bias);
	}

	// ������Ч��
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::Fog(FogMode mode, Color const & color,
		float expDensity, float linearStart, float linearEnd)
	{
		GLint fogMode;
		switch (mode)
		{
		case Fog_Exp:
			fogMode = GL_EXP;
			break;

		case Fog_Exp2:
			fogMode = GL_EXP2;
			break;

		case Fog_Linear:
			fogMode = GL_LINEAR;
			break;

		default:
			// Give up on it
			glDisable(GL_FOG);
			return;
		}

		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, fogMode);
		GLfloat fogColor[4] = { color.r(), color.g(), color.b(), color.a() };
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, expDensity);
		glFogf(GL_FOG_START, linearStart);
		glFogf(GL_FOG_END, linearEnd);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::SetTexture(U32 stage, TexturePtr const & texture)
	{
		if (!texture)
		{
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, OGLTexturePtr(static_cast<OGLTexture*>(texture.get()))->GLTexture());
		}
	}

	// �����������꼯
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::TextureCoordSet(U32 stage, int index)
	{
	}

	// ��ȡ�������׶���
	/////////////////////////////////////////////////////////////////////////////////
	U32 OGLRenderEngine::MaxTextureStages()
	{
		return 1;
	}

	// �ر�ĳ������׶�
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::DisableTextureStage(U32 stage)
	{
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::TextureCoordCalculation(U32 stage, TexCoordCalcMethod m)
	{
	}

	// ��������Ѱַģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::TextureAddressingMode(U32 stage, TexAddressingMode tam)
	{
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::TextureMatrix(U32 stage, Matrix4 const & mat)
	{
	}

	// �����������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::TextureFiltering(U32 stage, TexFiltering texFiltering)
	{
	}

	// �����������Թ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::TextureAnisotropy(U32 stage, U32 maxAnisotropy)
	{
	}

	// ��ȡ���������
	/////////////////////////////////////////////////////////////////////////////////
	U32 OGLRenderEngine::MaxVertexBlendMatrices()
	{
		return 1;
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
	U16 OGLRenderEngine::StencilBufferBitDepth()
	{
		return 8;
	}

	// ����ģ��ȽϺ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferFunction(CompareFunction func)
	{
	}

	// ����ģ�建�����ο�ֵ
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferReferenceValue(U32 refValue)
	{
	}

	// ����ģ�建��������
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferMask(U32 mask)
	{
	}

	// ����ģ�建��������ʧ�ܺ�Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferFailOperation(StencilOperation op)
	{
	}

	// ����ģ�建������Ȳ���ʧ�ܺ�Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferDepthFailOperation(StencilOperation op)
	{
	}

	// ����ģ�建����ͨ����Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void OGLRenderEngine::StencilBufferPassOperation(StencilOperation op)
	{
	}
}
