// D3D9RenderEngine.cpp
// KlayGE D3D9��Ⱦ������ ʵ���ļ�
// Ver 2.0.3
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// �Ż���Render (2004.2.22)
//
// 2.0.1
// �ع���Render (2003.10.10)
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/SharePtr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/Engine.hpp>

#include <KlayGE/Light.hpp>
#include <KlayGE/Material.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/VertexBuffer.hpp>
#include <KlayGE/RenderTarget.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/D3D9/D3D9RenderWindow.hpp>
#include <KlayGE/D3D9/D3D9Texture.hpp>

#include <cassert>
#include <algorithm>

#include <KlayGE/D3D9/D3D9RenderEngine.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")

namespace KlayGE
{
	// ��KlayGE��Matrix4ת����D3DMATRIX
	/////////////////////////////////////////////////////////////////////////////////
	D3DMATRIX Convert(const Matrix4& mat)
	{
		D3DMATRIX d3dMat;
		memcpy(&d3dMat._11, &mat.begin()[0], sizeof(d3dMat));

		return d3dMat;
	}

	// ��D3DMATRIXת����KlayGE��Matrix4
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 Convert(const D3DMATRIX& mat)
	{
		return Matrix4(&mat.m[0][0]);
	}

	// ��RenderEngine::CompareFunctionת����D3DCMPFUNC
	/////////////////////////////////////////////////////////////////////////////////
	D3DCMPFUNC Convert(RenderEngine::CompareFunction func)
	{
		D3DCMPFUNC ret;
		switch (func)
		{
		case RenderEngine::CF_AlwaysFail:
			ret = D3DCMP_NEVER;
			break;

		case RenderEngine::CF_AlwaysPass:
			ret = D3DCMP_ALWAYS;
			break;

		case RenderEngine::CF_Less:
			ret = D3DCMP_LESS;
			break;

		case RenderEngine::CF_LessEqual:
			ret = D3DCMP_LESSEQUAL;
			break;

		case RenderEngine::CF_Equal:
			ret = D3DCMP_EQUAL;
			break;

		case RenderEngine::CF_NotEqual:
			ret = D3DCMP_NOTEQUAL;
			break;

		case RenderEngine::CF_GreaterEqual:
			ret = D3DCMP_GREATEREQUAL;
			break;

		case RenderEngine::CF_Greater:
			ret = D3DCMP_GREATER;
			break;
		};

		return ret;
	}

	// ��RenderEngine::StencilOperationת����D3DSTENCILOP
	/////////////////////////////////////////////////////////////////////////////////
	D3DSTENCILOP Convert(RenderEngine::StencilOperation op)
	{
		D3DSTENCILOP ret;

		switch (op)
		{
		case RenderEngine::SOP_Keep:
			ret = D3DSTENCILOP_KEEP;
			break;

		case RenderEngine::SOP_Zero:
			ret = D3DSTENCILOP_ZERO;
			break;

		case RenderEngine::SOP_Replace:
			ret = D3DSTENCILOP_REPLACE;
			break;

		case RenderEngine::SOP_Increment:
			ret = D3DSTENCILOP_INCRSAT;
			break;

		case RenderEngine::SOP_Decrement:
			ret = D3DSTENCILOP_DECRSAT;
			break;

		case RenderEngine::SOP_Invert:
			ret = D3DSTENCILOP_INVERT;
			break;
		};

		return ret;
	}

	// ��RenderEngine::TexFilteringת����D3D��MagFilter��־
	/////////////////////////////////////////////////////////////////////////////////
	U32 MagFilter(const D3DCAPS9& caps, RenderEngine::TexFiltering tf)
	{
		// NOTE: Fall through if device doesn't support requested type
		if ((RenderEngine::TF_Anisotropic == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC))
		{
			return D3DTEXF_ANISOTROPIC;
		}
		else
		{
			tf = RenderEngine::TF_Trilinear;
		}

		if ((RenderEngine::TF_Trilinear == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR))
		{
			return D3DTEXF_LINEAR;
		}
		else
		{
			tf = RenderEngine::TF_Bilinear;
		}

		if ((RenderEngine::TF_Bilinear == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR))
		{
			return D3DTEXF_LINEAR;
		}

		return D3DTEXF_POINT;
	}

	// ��RenderEngine::TexFilteringת����D3D��MinFilter��־
	/////////////////////////////////////////////////////////////////////////////////
	U32 MinFilter(const D3DCAPS9& caps, RenderEngine::TexFiltering tf)
	{
		// NOTE: Fall through if device doesn't support requested type
		if ((RenderEngine::TF_Anisotropic == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC))
		{
			return D3DTEXF_ANISOTROPIC;
		}
		else
		{
			tf = RenderEngine::TF_Trilinear;
		}

		if ((RenderEngine::TF_Trilinear == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR))
		{
			return D3DTEXF_LINEAR;
		}
		else
		{
			tf = RenderEngine::TF_Bilinear;
		}

		if ((RenderEngine::TF_Bilinear == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR))
		{
			return D3DTEXF_LINEAR;
		}

		return D3DTEXF_POINT;
	}

	// ��RenderEngine::TexFilteringת����D3D��MipFilter��־
	/////////////////////////////////////////////////////////////////////////////////
	U32 MipFilter(const D3DCAPS9& caps, RenderEngine::TexFiltering tf)
	{
		// NOTE: Fall through if device doesn't support requested type
		if ((RenderEngine::TF_Anisotropic == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		{
			return D3DTEXF_LINEAR;
		}
		else
		{
			tf = RenderEngine::TF_Trilinear;
		}

		if ((RenderEngine::TF_Trilinear == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		{
			return D3DTEXF_LINEAR;
		}
		else
		{
			tf = RenderEngine::TF_Bilinear;
		}

		if ((RenderEngine::TF_Bilinear == tf) && (caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		{
			return D3DTEXF_POINT;
		}

		return D3DTEXF_NONE;
	}
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::D3D9RenderEngine()
						: cullingMode_(RenderEngine::Cull_None),
							clearFlags_(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER)
	{
		// Create our Direct3D object
		d3d_ = COMPtr<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION));
		Verify(d3d_.Get() != NULL);

		adapterList_.Enumerate(d3d_);
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	D3D9RenderEngine::~D3D9RenderEngine()
	{
	}

	// ������Ⱦϵͳ������
	/////////////////////////////////////////////////////////////////////////////////
	const WString& D3D9RenderEngine::Name() const
	{
		static WString name(L"Direct3D9 Render System");
		return name;
	}

	// ��ȡD3D�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	const COMPtr<IDirect3D9>& D3D9RenderEngine::D3D() const
	{
		return d3d_;
	}

	// ��ȡD3D Device�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	const COMPtr<IDirect3DDevice9>& D3D9RenderEngine::D3DDevice() const
	{
		return d3dDevice_;
	}

	// ��ȡD3D�������б�
	/////////////////////////////////////////////////////////////////////////////////
	const D3D9AdapterList& D3D9RenderEngine::D3DAdapters() const
	{
		return adapterList_;
	}

	// ��ȡ��ǰ������
	/////////////////////////////////////////////////////////////////////////////////
	const D3D9Adapter& D3D9RenderEngine::ActiveAdapter() const
	{
		return adapterList_.Adapter(adapterList_.CurrentAdapterIndex());
	}

	// ��ʼ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StartRendering()
	{
		bool gotMsg;
		MSG  msg;

		::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

		RenderTarget& renderTarget(**RenderEngine::ActiveRenderTarget());
		while (WM_QUIT != msg.message)
		{
			// ��������Ǽ���ģ��� PeekMessage()�Ա����ǿ����ÿ���ʱ����Ⱦ����
			// ��Ȼ, �� GetMessage() ���� CPU ռ����
			if (renderTarget.Active())
			{
				gotMsg = (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
			}
			else
			{
				gotMsg = (::GetMessage(&msg, NULL, 0, 0) != 0);
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
	void D3D9RenderEngine::AmbientLight(const Color& col)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(col.r(), col.g(), col.b(), 1.0f)));
	}

	// ���������ɫ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ClearColor(const Color& clr)
	{
		clearClr_ = D3DCOLOR_COLORVALUE(clr.r(), clr.g(), clr.b(), 1.0f);
	}

	// ���ù�Ӱ����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ShadingType(ShadeOptions so)
	{
		D3DSHADEMODE shadeMode;
		switch (so)
		{
		case SO_Flat:
			shadeMode = D3DSHADE_FLAT;
			break;

		case SO_Gouraud:
			shadeMode = D3DSHADE_GOURAUD;
			break;

		case SO_Phong:
			shadeMode = D3DSHADE_PHONG;
			break;
		}

		TIF(d3dDevice_->SetRenderState(D3DRS_SHADEMODE, shadeMode));
	}

	// ��/�رչ�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::EnableLighting(bool enabled)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_LIGHTING, enabled));
	}

	// ������Ⱦ����
	/////////////////////////////////////////////////////////////////////////////////
	RenderWindowPtr D3D9RenderEngine::CreateRenderWindow(const String& name,
		const RenderWindowSettings& settings)
	{
		RenderWindowPtr win(new D3D9RenderWindow(d3d_, this->ActiveAdapter(), name,
			static_cast<const D3D9RenderWindowSettings&>(settings)));

		IDirect3DDevice9* d3dDevice;
		win->CustomAttribute("D3DDEVICE", &d3dDevice);
		d3dDevice_ = COMPtr<IDirect3DDevice9>(d3dDevice);
		d3dDevice_->AddRef();

		this->ActiveRenderTarget(this->AddRenderTarget(win));

		this->DepthBufferDepthTest(settings.depthBuffer);
		this->DepthBufferDepthWrite(settings.depthBuffer);

		this->SetMaterial(Material(Color(1, 1, 1, 1)));

		// get caps
		d3d_->GetDeviceCaps(this->ActiveAdapter().AdapterNo(), D3DDEVTYPE_HAL, &caps_);

		return win;
	}

	// ���ü���ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::CullingMode(CullMode mode)
	{
		U32 d3dMode;

		cullingMode_ = mode;

		switch (mode)
		{
		case Cull_None:
			d3dMode = D3DCULL_NONE;
			break;

		case Cull_Clockwise:
			d3dMode = (*activeRenderTarget_)->RequiresTextureFlipping() ? D3DCULL_CCW : D3DCULL_CW;
			break;

		case Cull_AntiClockwise:
			d3dMode = (*activeRenderTarget_)->RequiresTextureFlipping() ? D3DCULL_CW : D3DCULL_CCW;
			break;
		}

		TIF(d3dDevice_->SetRenderState(D3DRS_CULLMODE, d3dMode));
	}

	// ���ù�Դ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetLight(U32 index, const Light& lt)
	{
		D3DLIGHT9 d3dLight;
		Engine::MemoryInstance().Zero(&d3dLight, sizeof(d3dLight));

		switch (lt.lightType)
		{
		case Light::LT_Point:
			d3dLight.Type = D3DLIGHT_POINT;
			break;

		case Light::LT_Directional:
			d3dLight.Type = D3DLIGHT_DIRECTIONAL;
			break;

		case Light::LT_Spot:
			d3dLight.Type		= D3DLIGHT_SPOT;
			d3dLight.Falloff	= lt.spotFalloff;
			d3dLight.Theta		= lt.spotInner;
			d3dLight.Phi		= lt.spotOuter;
			break;
		}

		d3dLight.Diffuse	= D3DXCOLOR(lt.diffuse.r(), lt.diffuse.g(), lt.diffuse.b(), lt.diffuse.a());
		d3dLight.Specular	= D3DXCOLOR(lt.specular.r(), lt.specular.g(), lt.specular.b(), lt.specular.a());
		d3dLight.Ambient	= D3DXCOLOR(lt.ambient.r(), lt.ambient.g(), lt.ambient.b(), lt.ambient.a());

		if (lt.lightType != Light::LT_Directional)
		{
			d3dLight.Position = D3DXVECTOR3(lt.position.x(), lt.position.y(), lt.position.z());
		}
		if (lt.lightType != Light::LT_Point)
		{
			d3dLight.Direction = D3DXVECTOR3(lt.direction.x(), lt.direction.y(), lt.direction.z());
		}

		d3dLight.Range = lt.range;
		d3dLight.Attenuation0 = lt.attenuationConst;
		d3dLight.Attenuation1 = lt.attenuationLinear;
		d3dLight.Attenuation2 = lt.attenuationQuad;

		TIF(d3dDevice_->SetLight(index, &d3dLight));
	}

	// ��/�ر�ĳ����Դ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::LightEnable(U32 index, bool enabled)
	{
		TIF(d3dDevice_->LightEnable(index, enabled));
	}

	// ��ȡ�������
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 D3D9RenderEngine::WorldMatrix() const
	{
		D3DMATRIX d3dmat;
		TIF(d3dDevice_->GetTransform(D3DTS_WORLD, &d3dmat));

		return Convert(d3dmat);
	}

	// �����������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::WorldMatrix(const Matrix4& mat)
	{
		D3DMATRIX d3dmat(Convert(mat));

		TIF(d3dDevice_->SetTransform(D3DTS_WORLD, &d3dmat));
	}

	// ���ö���������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::WorldMatrices(Matrix4* mats, size_t count)
	{
		for (size_t i = 0; i < count; ++ i)
		{
			D3DMATRIX d3dmat(Convert(mats[i]));

			TIF(d3dDevice_->SetTransform(D3DTS_WORLDMATRIX(i), &d3dmat));
		}
	}

	// ��ȡ�۲����
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 D3D9RenderEngine::ViewMatrix()
	{
		D3DMATRIX d3dmat;
		TIF(d3dDevice_->GetTransform(D3DTS_VIEW, &d3dmat));

		return Convert(d3dmat);
	}

	// ���ù۲����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ViewMatrix(const Matrix4& mat)
	{
		D3DMATRIX d3dMat(Convert(mat));

		TIF(d3dDevice_->SetTransform(D3DTS_VIEW, &d3dMat));
	}

	// ��ȡͶ�����
	/////////////////////////////////////////////////////////////////////////////////
	Matrix4 D3D9RenderEngine::ProjectionMatrix()
	{
		D3DMATRIX d3dmat;
		TIF(d3dDevice_->GetTransform(D3DTS_PROJECTION, &d3dmat));

		return Convert(d3dmat);
	}

	// ����Ͷ�����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ProjectionMatrix(const Matrix4& mat)
	{
		D3DMATRIX d3dMat(Convert(mat));

		if ((*activeRenderTarget_)->RequiresTextureFlipping())
		{
			d3dMat._22 = -d3dMat._22;
		}

		TIF(d3dDevice_->SetTransform(D3DTS_PROJECTION, &d3dMat));
	}

	// ���ò���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetMaterial(const Material& m)
	{
		D3DMATERIAL9 material;

		material.Diffuse	= D3DXCOLOR(m.diffuse.r(), m.diffuse.g(), m.diffuse.b(), m.diffuse.a());
		material.Ambient	= D3DXCOLOR(m.ambient.r(), m.ambient.g(), m.ambient.b(), m.ambient.a());
		material.Specular	= D3DXCOLOR(m.specular.r(), m.specular.g(), m.specular.b(), m.specular.a());
		material.Emissive	= D3DXCOLOR(m.emissive.r(), m.emissive.g(), m.emissive.b(), m.emissive.a());
		material.Power		= m.shininess;

		TIF(d3dDevice_->SetMaterial(&material));
	}

	// ���õ�ǰ��ȾĿ�꣬����ȾĿ������Ѿ����б���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::ActiveRenderTarget(RenderTargetListIterator iter)
	{
		RenderEngine::ActiveRenderTarget(iter);

		IDirect3DSurface9* back;
		(*activeRenderTarget_)->CustomAttribute("DDBACKBUFFER", &back);
		TIF(d3dDevice_->SetRenderTarget(0, back));
		IDirect3DSurface9* zBuffer;
		(*activeRenderTarget_)->CustomAttribute("D3DZBUFFER", &zBuffer);
		TIF(d3dDevice_->SetDepthStencilSurface(zBuffer));

		this->CullingMode(cullingMode_);

		const Viewport& vp((*iter)->GetViewport());
		D3DVIEWPORT9 d3dvp = { vp.left, vp.top, vp.width, vp.height, 0, 1 };
		TIF(d3dDevice_->SetViewport(&d3dvp));
	}

	// ��ʼһ֡
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::BeginFrame()
	{
		TIF(d3dDevice_->Clear(0, NULL, clearFlags_, clearClr_, 1, 0));

		TIF(d3dDevice_->BeginScene());
	}

	// ��Ⱦ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::Render(const VertexBuffer& vb)
	{
		vbConverter_.Update(vb);

		if (vb.UseIndices())
		{
			d3dDevice_->SetIndices(ibConverter_.Update(vb).Get());
			this->DrawIndexedPrimitive(vbConverter_.PrimType(), vb.NumVertices(), vbConverter_.PrimCount());
		}
		else
		{
			this->DrawPrimitive(vbConverter_.PrimType(), vbConverter_.PrimCount());
		}
	}

	// �������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DrawPrimitive(D3DPRIMITIVETYPE primType, U32 primCount)
	{
		for (UINT i = 0; i < renderPasses_; ++ i)
		{
			renderEffect_->Pass(i);
			TIF(d3dDevice_->DrawPrimitive(primType, 0, primCount));
		}
	}

	// �������Ķ����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DrawIndexedPrimitive(D3DPRIMITIVETYPE primType, U32 vertexCount, U32 primCount)
	{
		for (UINT i = 0; i < renderPasses_; ++ i)
		{
			renderEffect_->Pass(i);
			TIF(d3dDevice_->DrawIndexedPrimitive(primType, 0, 0, vertexCount, 0, primCount));
		}
	}

	// ����һ֡
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::EndFrame()
	{
		TIF(d3dDevice_->EndScene());
	}

	// ��/�ر���Ȳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferDepthTest(bool depthTest)
	{
		if (depthTest)
		{
			// Use w-buffer if abialable
			if (caps_.RasterCaps & D3DPRASTERCAPS_WBUFFER)
			{
				TIF(d3dDevice_->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW));
			}
			else
			{
				TIF(d3dDevice_->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE));
			}
		}
		else
		{
			TIF(d3dDevice_->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE));
		}
	}

	// ��/�ر���Ȼ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferDepthWrite(bool depthWrite)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_ZWRITEENABLE, depthWrite));
	}

	// ������ȱȽϺ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBufferFunction(CompareFunction depthFunction)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_ZFUNC, Convert(depthFunction)));
	}

	// �������ƫ��
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DepthBias(U16 bias)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_DEPTHBIAS, bias));
	}

	// ������Ч��
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::Fog(FogMode mode, const Color& color,
		float expDensity, float linearStart, float linearEnd)
	{
		if (Fog_None == mode)
		{
			// just disable
			d3dDevice_->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
			d3dDevice_->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
		else
		{
			// Allow fog
			d3dDevice_->SetRenderState(D3DRS_FOGENABLE, TRUE);

			// Set pixel fog mode
			D3DFOGMODE d3dFogMode;
			switch (mode)
			{
			case Fog_Exp:
				d3dFogMode = D3DFOG_EXP;
				break;

			case Fog_Exp2:
				d3dFogMode = D3DFOG_EXP2;
				break;

			case Fog_Linear:
				d3dFogMode = D3DFOG_LINEAR;
				break;
			}

			d3dDevice_->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
			d3dDevice_->SetRenderState(D3DRS_FOGTABLEMODE, d3dFogMode);

			d3dDevice_->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_COLORVALUE(color.r(), color.g(), color.b(), color.a()));
			d3dDevice_->SetRenderState(D3DRS_FOGSTART, *reinterpret_cast<U32*>(&linearStart));
			d3dDevice_->SetRenderState(D3DRS_FOGEND, *reinterpret_cast<U32*>(&linearEnd));
			d3dDevice_->SetRenderState(D3DRS_FOGDENSITY, *reinterpret_cast<U32*>(&expDensity));
		}
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::SetTexture(U32 stage, const Texture& texture)
	{
		const D3D9Texture& d3d9Texture(reinterpret_cast<const D3D9Texture&>(texture));

		TIF(d3dDevice_->SetTexture(stage, d3d9Texture.D3DTexture().Get()));
	}

	// �����������꼯
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureCoordSet(U32 stage, int index)
	{
		TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, index));
	}

	// ��ȡ�������׶���
	/////////////////////////////////////////////////////////////////////////////////
	U32 D3D9RenderEngine::MaxTextureStages()
	{
		return caps_.MaxSimultaneousTextures;
	}

	// �ر�ĳ������׶�
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::DisableTextureStage(U32 stage)
	{
		TIF(d3dDevice_->SetTexture(stage, NULL));
		TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE));
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureCoordCalculation(U32 stage, TexCoordCalcMethod m)
	{
		HRESULT hr = S_OK;
		D3DXMATRIX matTrans;
		switch (m)
		{
		case TCC_None:
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			D3DXMatrixIdentity(&matTrans);
			hr = d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &matTrans);
			break;

		case TCC_EnvironmentMap:
			// Sets the flags required for an environment map effect
			hr = d3dDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			D3DXMatrixIdentity(&matTrans);
			matTrans._11 = 0.5f;
			matTrans._41 = 0.5f;
			matTrans._22 = -0.5f;
			matTrans._42 = 0.5f;
			hr = d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &matTrans);
			break;

		case TCC_EnvironmentMapPlanar:
			// Sets the flags required for an environment map effect
			hr = d3dDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
			hr = d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			D3DXMatrixIdentity(&matTrans);
			matTrans._11 = 0.5f;
			matTrans._41 = 0.5f;
			matTrans._22 = -0.5f;
			matTrans._42 = 0.5f;
			hr = d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &matTrans);
			break;
		}

		TIF(hr);
	}

	// ��������Ѱַģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureAddressingMode(U32 stage, TexAddressingMode tam)
	{
		D3DTEXTUREADDRESS d3dType;
		switch (tam)
		{
		case TAM_Wrap:
			d3dType = D3DTADDRESS_WRAP;
			break;

		case TAM_Mirror:
			d3dType = D3DTADDRESS_MIRROR;
			break;

		case TAM_Clamp:
			d3dType = D3DTADDRESS_CLAMP;
			break;
		}

		TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSU, d3dType));
		TIF(d3dDevice_->SetSamplerState(stage, D3DSAMP_ADDRESSV, d3dType));
	}

	// ������������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureMatrix(U32 stage, const Matrix4& mat)
	{
		if (Matrix4::Identity() == mat)
		{
			TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE));
		}
		else
		{
			// Set 2D input
			// TODO: deal with 3D coordinates when cubic environment mapping supported
			TIF(d3dDevice_->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));

			D3DMATRIX d3dMat(Convert(mat));
			TIF(d3dDevice_->SetTransform(static_cast<D3DTRANSFORMSTATETYPE>(D3DTS_TEXTURE0 + stage), &d3dMat));
		}
	}

	// �����������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureFiltering(U32 stage, TexFiltering texFiltering)
	{
		d3dDevice_->SetSamplerState(stage, D3DSAMP_MAGFILTER, MagFilter(caps_, texFiltering));
		d3dDevice_->SetSamplerState(stage, D3DSAMP_MINFILTER, MinFilter(caps_, texFiltering));
		d3dDevice_->SetSamplerState(stage, D3DSAMP_MIPFILTER, MipFilter(caps_, texFiltering));
	}

	// �����������Թ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::TextureAnisotropy(U32 stage, U32 maxAnisotropy)
	{
		maxAnisotropy = std::min(maxAnisotropy, caps_.MaxAnisotropy);

		U32 curAnisotropy;
		d3dDevice_->GetSamplerState(stage, D3DSAMP_MAXANISOTROPY, &curAnisotropy);
		if (maxAnisotropy != curAnisotropy)
		{
			d3dDevice_->SetSamplerState(stage, D3DSAMP_MAXANISOTROPY, maxAnisotropy);
		}
	}

	// ��ȡ���������
	/////////////////////////////////////////////////////////////////////////////////
	U32 D3D9RenderEngine::MaxVertexBlendMatrices()
	{
		return caps_.MaxVertexBlendMatrices;
	}

	// ��ģ�建����
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilCheckEnabled(bool enabled)
	{
		if (enabled)
		{
			clearFlags_ |= D3DCLEAR_STENCIL;
		}
		else
		{
			clearFlags_ &= ~D3DCLEAR_STENCIL;
		}

		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILENABLE, enabled));
	}

	// Ӳ���Ƿ�֧��ģ�建����
	/////////////////////////////////////////////////////////////////////////////////
	bool D3D9RenderEngine::HasHardwareStencil()
	{
		IDirect3DSurface9* surf;
		D3DSURFACE_DESC surfDesc;
		d3dDevice_->GetDepthStencilSurface(&surf);
		surf->GetDesc(&surfDesc);

		if (D3DFMT_D24S8 == surfDesc.Format)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// ����ģ��λ��
	/////////////////////////////////////////////////////////////////////////////////
	U16 D3D9RenderEngine::StencilBufferBitDepth()
	{
		return 8;
	}

	// ����ģ��ȽϺ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferFunction(CompareFunction func)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILFUNC, Convert(func)));
	}

	// ����ģ�建�����ο�ֵ
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferReferenceValue(U32 refValue)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILREF, refValue));
	}

	// ����ģ�建��������
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferMask(U32 mask)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILMASK, mask));
	}

	// ����ģ�建��������ʧ�ܺ�Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferFailOperation(StencilOperation op)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILFAIL, Convert(op)));
	}

	// ����ģ�建������Ȳ���ʧ�ܺ�Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferDepthFailOperation(StencilOperation op)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILZFAIL, Convert(op)));
	}

	// ����ģ�建����ͨ����Ĳ���
	/////////////////////////////////////////////////////////////////////////////////
	void D3D9RenderEngine::StencilBufferPassOperation(StencilOperation op)
	{
		TIF(d3dDevice_->SetRenderState(D3DRS_STENCILPASS, Convert(op)));
	}
}
