#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Sampler.hpp>
#include <KlayGE/KMesh.hpp>
#include <KlayGE/SceneObjectHelper.hpp>
#include <KlayGE/Script.hpp>

#include <KlayGE/OCTree/OCTree.hpp>

#include <KlayGE/D3D9/D3D9RenderFactory.hpp>
#include <KlayGE/OpenGL/OGLRenderFactory.hpp>

#include <KlayGE/Input.hpp>
#include <KlayGE/DInput/DInputFactory.hpp>

#include <vector>
#include <sstream>
#include <ctime>
#include <boost/bind.hpp>

#include "Instancing.hpp"

using namespace std;
using namespace KlayGE;

namespace
{
	int32_t const NUM_INSTANCE = 400;

	class Teapot : public SceneObjectHelper
	{
	private:
		struct InstData
		{
			float4 col[3];
			Color clr;
		};

	public:
		Teapot()
			: SceneObjectHelper(SOA_Cullable)
		{
			instance_format_.push_back(vertex_element(VEU_TextureCoord, 1, EF_ABGR32F));
			instance_format_.push_back(vertex_element(VEU_TextureCoord, 2, EF_ABGR32F));
			instance_format_.push_back(vertex_element(VEU_TextureCoord, 3, EF_ABGR32F));
			instance_format_.push_back(vertex_element(VEU_Diffuse, 0, EF_ABGR32F));
		}

		void Instance(float4x4 const & mat, Color const & clr)
		{
			mat_ = mat;
			float4x4 matT = MathLib::transpose(mat);

			inst_.col[0] = matT.Row(0);
			inst_.col[1] = matT.Row(1);
			inst_.col[2] = matT.Row(2);
			inst_.clr = clr;
		}

		void const * InstanceData() const
		{
			return &inst_;
		}

		void SetRenderable(RenderablePtr ra)
		{
			renderable_ = ra;
		}

		float4x4 GetModelMatrix() const
		{
			return mat_;
		}

	private:
		InstData inst_;
		float4x4 mat_;
	};

	class RenderInstance : public KMesh
	{
	public:
		RenderInstance(RenderModelPtr model, std::wstring const & /*name*/)
			: KMesh(model, L"Instance")
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			technique_ = rf.LoadEffect("Instancing.kfx")->TechniqueByName("Instance");
		}

		void BuildMeshInfo()
		{
		}

		void OnRenderBegin()
		{
			App3DFramework const & app = Context::Instance().AppInstance();

			float4x4 const & model = float4x4::Identity();
			float4x4 const & view = app.ActiveCamera().ViewMatrix();
			float4x4 const & proj = app.ActiveCamera().ProjMatrix();

			*(technique_->Effect().ParameterByName("ViewProj")) = model * view * proj;
			*(technique_->Effect().ParameterByName("light_in_world")) = float3(2, 2, -3);
		}
	};

	class RenderNormalMesh : public KMesh
	{
	private:
		struct InstData
		{
			float4 col[3];
			Color clr;
		};

	public:
		RenderNormalMesh(RenderModelPtr model, std::wstring const & /*name*/)
			: KMesh(model, L"NormalMesh")
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			technique_ = rf.LoadEffect("Instancing.kfx")->TechniqueByName("NormalMesh");
		}

		void BuildMeshInfo()
		{
		}

		void OnRenderBegin()
		{
			App3DFramework const & app = Context::Instance().AppInstance();

			float4x4 const & view = app.ActiveCamera().ViewMatrix();
			float4x4 const & proj = app.ActiveCamera().ProjMatrix();

			*(technique_->Effect().ParameterByName("ViewProj")) = view * proj;
			*(technique_->Effect().ParameterByName("light_in_world")) = float3(2, 2, -3);
		}

		void OnInstanceBegin(uint32_t id)
		{
			InstData const * data = static_cast<InstData const *>(instances_[id].lock()->InstanceData());

			float4x4 model;
			model.Col(0, data->col[0]);
			model.Col(1, data->col[1]);
			model.Col(2, data->col[2]);
			model.Col(3, float4(0, 0, 0, 1));

			*(technique_->Effect().ParameterByName("modelmat")) = model;
			*(technique_->Effect().ParameterByName("color")) = float4(data->clr.r(), data->clr.g(), data->clr.b(), data->clr.a());
		}

	private:
		void UpdateInstanceStream()
		{
		}
	};

	enum
	{
		UseInstanceing
	};

	enum
	{
		Exit
	};

	InputActionDefine actions[] = 
	{
		InputActionDefine(Exit, KS_Escape)
	};

	bool ConfirmDevice(RenderDeviceCaps const & caps)
	{
		if (caps.max_shader_model < 2)
		{
			return false;
		}
		return true;
	}
}


int main()
{
	OCTree sceneMgr(3);

	Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());
	Context::Instance().SceneManagerInstance(sceneMgr);
	Context::Instance().InputFactoryInstance(DInputFactoryInstance());

	RenderSettings settings;
	settings.width = 800;
	settings.height = 600;
	settings.color_fmt = EF_ARGB8;
	settings.full_screen = false;
	settings.ConfirmDevice = ConfirmDevice;

	Instancing app("Instance", settings);
	app.Create();
	app.Run();

	return 0;
}

Instancing::Instancing(std::string const & name, RenderSettings const & settings)
				: App3DFramework(name, settings)
{
	ResLoader::Instance().AddPath("../../media/Common");
	ResLoader::Instance().AddPath("../../media/Instancing");
}

void Instancing::InitObjects()
{
	// ��������
	font_ = Context::Instance().RenderFactoryInstance().MakeFont("gkai00mp.ttf", 16);

	ScriptEngine scriptEng;
	ScriptModule module("Instancing_init");

	renderInstance_ = LoadKModel("teapot.kmodel", CreateKModelFactory<RenderModel>(), CreateKMeshFactory<RenderInstance>())->Mesh(0);
	for (int32_t i = 0; i < 10; ++ i)
	{
		for (int32_t j = 0; j < NUM_INSTANCE / 10; ++ j)
		{
			PyObjectPtr py_pos = module.Call("get_pos", boost::make_tuple(i, j, NUM_INSTANCE));

			float3 pos;
			pos.x() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_pos.get(), 0)));
			pos.y() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_pos.get(), 1)));
			pos.z() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_pos.get(), 2)));

			PyObjectPtr py_clr = module.Call("get_clr", boost::make_tuple(i, j, NUM_INSTANCE));

			Color clr;
			clr.r() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_clr.get(), 0)));
			clr.g() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_clr.get(), 1)));
			clr.b() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_clr.get(), 2)));
			clr.a() = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(py_clr.get(), 3)));

			SceneObjectPtr so(new Teapot);
			checked_pointer_cast<Teapot>(so)->Instance(MathLib::translation(pos), clr);

			checked_pointer_cast<Teapot>(so)->SetRenderable(renderInstance_);
			so->AddToSceneManager();
			scene_objs_.push_back(so);
		}
	}
	use_instance_ = true;

	renderMesh_ = LoadKModel("teapot.kmodel", CreateKModelFactory<RenderModel>(), CreateKMeshFactory<RenderNormalMesh>())->Mesh(0);

	this->LookAt(float3(-1.8f, 1.9f, -1.8f), float3(0, 0, 0));
	this->Proj(0.1f, 100);

	fpcController_.AttachCamera(this->ActiveCamera());
	fpcController_.Scalers(0.05f, 0.1f);

	InputEngine& inputEngine(Context::Instance().InputFactoryInstance().InputEngineInstance());
	InputActionMap actionMap;
	actionMap.AddActions(actions, actions + sizeof(actions) / sizeof(actions[0]));

	action_handler_t input_handler(new input_signal);
	input_handler->connect(boost::bind(&Instancing::InputHandler, this, _1, _2));
	inputEngine.ActionMap(actionMap, input_handler, true);

	dialog_ = UIManager::Instance().MakeDialog();
	dialog_->AddControl(UIControlPtr(new UICheckBox(dialog_, UseInstanceing, L"Use instancing",
                            60, 550, 350, 24, false, 0, false)));
	dialog_->Control<UICheckBox>(UseInstanceing)->SetChecked(true);
	dialog_->Control<UICheckBox>(UseInstanceing)->OnChangedEvent().connect(boost::bind(&Instancing::CheckBoxHandler, this, _1));
}

void Instancing::OnResize(uint32_t width, uint32_t height)
{
	App3DFramework::OnResize(width, height);

	dialog_->GetControl(UseInstanceing)->SetLocation(60, height - 50);
}

void Instancing::InputHandler(InputEngine const & /*sender*/, InputAction const & action)
{
	switch (action.first)
	{
	case Exit:
		this->Quit();
		break;
	}
}

void Instancing::CheckBoxHandler(UICheckBox const & /*sender*/)
{
	use_instance_ = dialog_->Control<UICheckBox>(UseInstanceing)->GetChecked();

	if (use_instance_)
	{
		for (int i = 0; i < NUM_INSTANCE; ++ i)
		{
			checked_pointer_cast<Teapot>(scene_objs_[i])->SetRenderable(renderMesh_);
		}
	}
	else
	{
		for (int i = 0; i < NUM_INSTANCE; ++ i)
		{
			checked_pointer_cast<Teapot>(scene_objs_[i])->SetRenderable(renderInstance_);
		}
	}
}

uint32_t Instancing::DoUpdate(uint32_t /*pass*/)
{
	fpcController_.Update();
	UIManager::Instance().HandleInput();

	RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
	renderEngine.CurFrameBuffer()->Attached(FrameBuffer::ATT_Color0)->Clear(Color(0.2f, 0.4f, 0.6f, 1));
	renderEngine.CurFrameBuffer()->Attached(FrameBuffer::ATT_DepthStencil)->Clear(1.0f);

	UIManager::Instance().Render();

	std::wostringstream stream;
	stream << this->FPS();

	font_->RenderText(0, 0, Color(1, 1, 0, 1), L"������ʵ����");
	font_->RenderText(0, 18, Color(1, 1, 0, 1), stream.str());

	SceneManager& sceneMgr(Context::Instance().SceneManagerInstance());
	stream.str(L"");
	stream << sceneMgr.NumObjectsRendered() << " Scene objects "
		<< sceneMgr.NumRenderablesRendered() << " Renderables "
		<< sceneMgr.NumPrimitivesRendered() << " Primitives "
		<< sceneMgr.NumVerticesRendered() << " Vertices";
	font_->RenderText(0, 36, Color(1, 1, 1, 1), stream.str());

	if (use_instance_)
	{
		font_->RenderText(0, 54, Color(1, 1, 1, 1), L"Instancing is enabled");
	}
	else
	{
		font_->RenderText(0, 54, Color(1, 1, 1, 1), L"Instancing is disabled");
	}

	return App3DFramework::URV_Need_Flush | App3DFramework::URV_Finished;
}
