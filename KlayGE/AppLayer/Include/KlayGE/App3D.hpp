// 3DApp.hpp
// KlayGE 3DApp�� ͷ�ļ�
// Ver 1.2.8.11
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.8
// ��inline���������� (2002.9.24)
//
// 1.2.8.11
// ����˻��� CAppFramework (2002.11.7)
// ����UNICODE���� (2002.11.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _APP3D_HPP
#define _APP3D_HPP

#include <KlayGE/Camera.hpp>
#include <KlayGE/RenderEngine.hpp>

#pragma comment(lib, "KlayGE_AppLayer.lib")

namespace KlayGE
{
	// һ�����ڴ���3DӦ�ó����ܵĻ��ࡣҪ����һ��D3DӦ�ó���ֻҪ�̳�����࣬
	//			Ȼ���������º���:
	//
	//			InitObjects()			- ��ʼ��3D�豸
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

		void Run();

	protected:
		void LookAt(Vector3 const & eye, Vector3 const & lookAt, Vector3 const & up = Vector3(0, 1, 0));
		void Proj(float nearPlane, float farPlane);

	protected:
		virtual void InitObjects()
			{ }

		virtual void Update()
			{ }

		virtual void RenderOver()
			{ }

		virtual void DelObjects()
			{ }
	};
}

#endif		// _APP3D_HPP
