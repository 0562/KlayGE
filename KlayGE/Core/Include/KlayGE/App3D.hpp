// App3D.hpp
// KlayGE App3D�� ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2003-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ����Core (2008.10.16)
//
// 3.7.0
// �Ľ���Update (2007.8.14)
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

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/RenderSettings.hpp>

namespace KlayGE
{
	// һ�����ڴ���3DӦ�ó����ܵĻ��ࡣ����һ��3DӦ�ó�����Ҫ�̳�����࣬
	//			Ȼ���������º���:
	//
	//			InitObjects()			- ��ʼ��3D�豸
	//			Update()				- ˢ�³���
	//			DelObjects()			- ���3D����
	/////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API App3DFramework
	{
		friend class SceneManager;

	public:
		enum UpdateRetVal
		{
			URV_Need_Flush = 1UL << 0,
			URV_Flushed = 1UL << 1,
			URV_Finished = 1UL << 2,
			URV_Only_New_Objs = 1UL << 3
		};

	public:
		App3DFramework(std::string const & name, RenderSettings const & settings);
		virtual ~App3DFramework();

		virtual void Create();
		virtual void Destroy();

		WindowPtr MakeWindow(std::string const & name, RenderSettings const & settings);
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
		void LookAt(float3 const & eye, float3 const & lookAt);
		void LookAt(float3 const & eye, float3 const & lookAt, float3 const & up);
		void Proj(float nearPlane, float farPlane);

	protected:
		virtual void InitObjects()
		{
		}

		uint32_t Update(uint32_t pass);
		virtual uint32_t DoUpdate(uint32_t pass) = 0;
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
