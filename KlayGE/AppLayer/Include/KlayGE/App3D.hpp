// App3D.hpp
// KlayGE App3D�� ͷ�ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2003-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ������MakeWindow (2007.6.26)
//
// 3.1.0
// ������OnResize (2005.11.20)
//
// 3.0.0
// �����˶����� (2005.8.16)
//
// 2.7.0
// ������Quit (2005.6.28)
//
// 2.0.0
// ���ν��� (2003.7.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _APP3D_HPP
#define _APP3D_HPP

#define KLAYGE_LIB_NAME KlayGE_AppLayer
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Window.hpp>

namespace KlayGE
{
	// һ�����ڴ���3DӦ�ó����ܵĻ��ࡣҪ����һ��D3DӦ�ó���ֻҪ�̳�����࣬
	//			Ȼ���������º���:
	//
	//			InitObjects()			- ��ʼ��3D�豸
	//			NumPasses()				- ���س�����Ҫ���Ƶı���
	//			Update()				- ˢ�³���
	//			DelObjects()			- ���3D����
	/////////////////////////////////////////////////////////////////////////////////
	class App3DFramework
	{
		friend class SceneManager;

	public:
		App3DFramework(std::string const & name, RenderSettings const & settings);
		virtual ~App3DFramework();

		virtual void Create();

		WindowPtr MakeWindow(std::string const & name, int32_t left, int32_t top,
			uint32_t width, uint32_t height);
		WindowPtr MainWnd() const
		{
			return main_wnd_;
		}

		Camera const & ActiveCamera() const;
		Camera& ActiveCamera();

		float FPS() const;

		void Run();
		void Quit();

		virtual void OnResize(uint32_t width, uint32_t height);

	protected:
		void LookAt(float3 const & eye, float3 const & lookAt, float3 const & up = float3(0, 1, 0));
		void Proj(float nearPlane, float farPlane);

	protected:
		virtual void InitObjects()
		{
		}

		virtual uint32_t NumPasses() const
		{
			return 1;
		}
		void Update(uint32_t pass);
		virtual void DoUpdate(uint32_t pass) = 0;
		void UpdateStats();

		virtual void RenderOver()
		{
		}

		virtual void DelObjects()
		{
		}

	protected:
		std::string name_;
		RenderSettings settings_;

		// Stats
		float	fps_;
		float	frame_time_;
		float	accumulate_time_;
		uint32_t num_frames_;

		Timer timer_;

		WindowPtr main_wnd_;
	};
}

#endif		// _APP3D_HPP
