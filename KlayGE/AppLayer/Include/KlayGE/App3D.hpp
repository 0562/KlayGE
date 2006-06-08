// App3Dp.hpp
// KlayGE App3D�� ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
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

#include <KlayGE/Camera.hpp>
#include <KlayGE/RenderEngine.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_AppLayer_d.lib")
#else
	#pragma comment(lib, "KlayGE_AppLayer.lib")
#endif

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
		App3DFramework();
		virtual ~App3DFramework();

		virtual void Create(std::string const & name, RenderSettings const & settings);

		Camera const & ActiveCamera() const;
		Camera& ActiveCamera();

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

		virtual void RenderOver()
		{
		}

		virtual void DelObjects()
		{
		}
	};
}

#endif		// _APP3D_HPP
