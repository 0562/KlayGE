#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/RenderBuffer.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderWindow.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>

#include <KlayGE/D3D9/D3D9RenderSettings.hpp>
#include <KlayGE/D3D9/D3D9RenderFactory.hpp>

#include <KlayGE/OCTree/OCTree.hpp>

#include <iostream>
#include <sstream>

#include "Torus.hpp"
#include "Cartoon.hpp"

using namespace std;
using namespace KlayGE;

namespace
{
	struct RenderTorus : public Renderable
	{
		RenderTorus(TexturePtr const & texture0, TexturePtr const & texture1)
			: rb_(new RenderBuffer(RenderBuffer::BT_TriangleList))
		{
			effect_ = LoadRenderEffect("Cartoon.fx");
			*(effect_->ParameterByName("toon")) = texture0;
			*(effect_->ParameterByName("edge")) = texture1;
			effect_->SetTechnique("cartoonTec");

			MathLib::ComputeBoundingBox(box_, reinterpret_cast<Vector3*>(&Pos[0]),
				reinterpret_cast<Vector3*>(&Pos[0] + sizeof(Pos) / sizeof(float)));

			rb_->AddVertexStream(VST_Positions, sizeof(float), 3, true);
			rb_->AddVertexStream(VST_Normals, sizeof(float), 3, true);

			rb_->GetVertexStream(VST_Positions)->Assign(Pos, sizeof(Pos) / sizeof(float) / 3);
			rb_->GetVertexStream(VST_Normals)->Assign(Normal, sizeof(Normal) / sizeof(float) / 3);

			rb_->AddIndexStream(true);
			rb_->GetIndexStream()->Assign(Index, sizeof(Index) / sizeof(uint16_t));
		}

		RenderEffectPtr GetRenderEffect() const
			{ return effect_; }

		RenderBufferPtr GetRenderBuffer() const
			{ return rb_; }

		Box GetBound() const
			{ return box_; }

		std::wstring const & Name() const
		{
			static const std::wstring name(L"Torus");
			return name;
		}

		KlayGE::RenderBufferPtr rb_;
		KlayGE::RenderEffectPtr effect_;

		Box box_;
	};

	boost::shared_ptr<RenderTorus> renderTorus;
}


class TheRenderSettings : public D3D9RenderSettings
{
private:
	bool DoConfirmDevice(D3DCAPS9 const & caps, uint32_t behavior, D3DFORMAT format) const
	{
		if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		{
			return false;
		}
		if (caps.PixelShaderVersion < D3DPS_VERSION(1, 1))
		{
			return false;
		}
		return true;
	}
};

int main()
{
	Cartoon app;
	OCTree sceneMgr(Box(Vector3(-20, -20, -20), Vector3(20, 20, 20)));

	Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());
	Context::Instance().SceneManagerInstance(sceneMgr);

	TheRenderSettings settings;
	settings.width = 800;
	settings.height = 600;
	settings.colorDepth = 32;
	settings.fullScreen = false;

	app.Create("��ͨ��Ⱦ����", settings);
	app.Run();

	return 0;
}

Cartoon::Cartoon()
			: rotX(0), rotY(0)
{
	ResLoader::Instance().AddPath("../media");
	ResLoader::Instance().AddPath("../media/Cartoon");
}

void Cartoon::InitObjects()
{
	font_ = Context::Instance().RenderFactoryInstance().MakeFont(L"����", 16);

	uint8_t cartoolShadeData0[16] = { 120, 120, 120, 120, 120, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255 };
	TexturePtr texture0 = Context::Instance().RenderFactoryInstance().MakeTexture(sizeof(cartoolShadeData0) / sizeof(cartoolShadeData0[0]), 1, 0, PF_L8);
	texture0->CopyMemoryToTexture(cartoolShadeData0, PF_L8);

	uint8_t cartoolShadeData1[4] = { 0, 255, 255, 255 };
	TexturePtr texture1 = Context::Instance().RenderFactoryInstance().MakeTexture(sizeof(cartoolShadeData1) / sizeof(cartoolShadeData1[0]), 1, 0, PF_L8);
	texture1->CopyMemoryToTexture(cartoolShadeData1, PF_L8);

	renderTorus = boost::shared_ptr<RenderTorus>(new RenderTorus(texture0, texture1));

	RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());

	renderEngine.ClearColor(Color(0.2f, 0.4f, 0.6f, 1));

	this->LookAt(Vector3(0, 0, -6), Vector3(0, 0, 0));
	this->Proj(0.1f, 20.0f);

	view_ = renderEngine.ViewMatrix();
	proj_ = renderEngine.ProjectionMatrix();

	*(renderTorus->GetRenderEffect()->ParameterByName("proj")) = proj_;
	*(renderTorus->GetRenderEffect()->ParameterByName("lightPos")) = Vector4(2, 2, -3, 1);
	*(renderTorus->GetRenderEffect()->ParameterByName("eyePos")) = Vector4(0, 0, -6, 1);
}

void Cartoon::Update()
{
	RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
	SceneManager& sceneMgr(Context::Instance().SceneManagerInstance());

	rotX += 0.003f;
	rotY += 0.003f;

	Matrix4 mat, matY;
	MathLib::RotationX(mat, rotX);
	MathLib::RotationY(matY, rotY);
	mat *= matY;
	mat *= view_;

	*(renderTorus->GetRenderEffect()->ParameterByName("worldview")) = mat;

	MathLib::Transpose(mat, mat);
	MathLib::Inverse(mat, mat);
	*(renderTorus->GetRenderEffect()->ParameterByName("worldviewIT")) = mat;

	std::wostringstream stream;
	stream << (*renderEngine.ActiveRenderTarget())->FPS();

	sceneMgr.PushRenderable(renderTorus);

	RenderWindow* rw = static_cast<RenderWindow*>(renderEngine.ActiveRenderTarget()->get());

	sceneMgr.PushRenderable(font_->RenderText(0, 0, Color(1, 1, 0, 1), L"��ͨ��Ⱦ����"));
	sceneMgr.PushRenderable(font_->RenderText(0, 18, Color(1, 1, 0, 1), rw->Description()));
	sceneMgr.PushRenderable(font_->RenderText(0, 36, Color(1, 1, 0, 1), stream.str().c_str()));
}
