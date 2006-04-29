// DShowEngine.cpp
// KlayGE DirectShow �������� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.9.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>

#include <boost/assert.hpp>
#include <uuids.h>

#include <KlayGE/DShow/DShow.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "strmiids.lib")

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DShowEngine::DShowEngine()
					: hWnd_(NULL)
	{
		::CoInitialize(0);

		this->Init();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	DShowEngine::~DShowEngine()
	{
		this->Free();

		::CoUninitialize();
	}

	// ��ʼ��
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::Init()
	{
		state_ = SS_Uninit;
	}

	// �ͷ�
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::Free()
	{
		this->Stop();

		videoWnd_.reset();
		mediaEvent_.reset();
		mediaControl_.reset();
		graph_.reset();
	}

	// ����Ƿ������Ƶ��
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::CheckVisibility()
	{
		audioOnly_ = false;

		if (!videoWnd_)
		{
			audioOnly_ = true;
			return;
		}

		long visible;
		HRESULT hr(videoWnd_->get_Visible(&visible));
		if (FAILED(hr))
		{
			// �����һ��ֻ������Ƶ���ļ�����get_Visible()������
			// ͬʱ�������Ƶ�����ò�֧�ֵķ�ʽѹ���ģ��Ͳ��ܿ�����Ƶ��
			// �������֧����Ƶ����ѹ����ʽ����Ƶ���Ϳ�����������
			if (E_NOINTERFACE == hr)
			{
				audioOnly_ = true;
			}
		}
	}

	// ����
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::DoPlay()
	{
		TIF(mediaControl_->Run());
	}

	// ��ͣ����
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::DoPause()
	{
		TIF(mediaControl_->Pause());
	}

	// ֹͣ����
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::DoStop()
	{
		TIF(mediaControl_->Stop());
	}

	// �����ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::Load(std::wstring const & fileName)
	{
		this->Free();
		this->Init();

		IGraphBuilder* graph;
		::CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_ALL,
			IID_IGraphBuilder, reinterpret_cast<void**>(&graph));
		graph_ = MakeCOMPtr(graph);

		TIF(graph_->RenderFile(fileName.c_str(), NULL));

		// ��ȡ DirectShow �ӿ�
		IMediaControl* mediaControl;
		TIF(graph_->QueryInterface(IID_IMediaControl,
			reinterpret_cast<void**>(&mediaControl)));
		mediaControl_ = MakeCOMPtr(mediaControl);

		IMediaEvent* mediaEvent;
		TIF(graph_->QueryInterface(IID_IMediaEvent,
			reinterpret_cast<void**>(&mediaEvent)));
		mediaEvent_ = MakeCOMPtr(mediaEvent);

		// ��ȡ��Ƶ�ӿڣ��������Ƶ�ļ������û����
		IVideoWindow* videoWnd;
		TIF(graph_->QueryInterface(IID_IVideoWindow,
			reinterpret_cast<void**>(&videoWnd)));
		videoWnd_ = MakeCOMPtr(videoWnd);

		this->CheckVisibility();

		hWnd_ = ::GetForegroundWindow();
		if (hWnd_ != NULL)
		{
			if (!audioOnly_)
			{
				videoWnd_->put_Owner(reinterpret_cast<OAHWND>(hWnd_));
				videoWnd_->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
			}

			RECT rect;
			::GetClientRect(hWnd_, &rect);
			TIF(videoWnd_->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom));

			::UpdateWindow(hWnd_);
			::SetForegroundWindow(hWnd_);
			::SetFocus(hWnd_);
		}

		state_ = SS_Stopped;
	}

	// ��鲥���Ƿ����
	/////////////////////////////////////////////////////////////////////////////////
	bool DShowEngine::IsComplete()
	{
		long lEventCode, lParam1, lParam2;
		bool ret(false);

		HRESULT hr(mediaEvent_->GetEvent(&lEventCode, reinterpret_cast<LONG_PTR*>(&lParam1),
							reinterpret_cast<LONG_PTR*>(&lParam2), 0));
		if (SUCCEEDED(hr))
		{
			if (1 == lEventCode)	// EC_COMPLETE
			{
				ret = true;
			}

			// �ͷź�����¼���ص��ڴ�
			TIF(mediaEvent_->FreeEventParams(lEventCode, lParam1, lParam2));
		}

		return ret;
	}

	// ��ȡ��ǰ״̬
	/////////////////////////////////////////////////////////////////////////////////
	ShowState DShowEngine::State(long msTimeout)
	{
		OAFilterState fs;
		HRESULT hr(mediaControl_->GetState(msTimeout, &fs));
		if (FAILED(hr))
		{
			return SS_Unkown;
		}

		state_ = SS_Unkown;
		switch (fs)
		{
		case State_Stopped:
			state_ = SS_Stopped;
			break;

		case State_Paused:
			state_ = SS_Paused;
			break;

		case State_Running:
			state_ = SS_Playing;
			break;
		}

		return state_;
	}

	// ȫ�� / ����ģʽ�л�
	/////////////////////////////////////////////////////////////////////////////////
	void DShowEngine::ToggleFullScreen()
	{
		static OAHWND hDrain(NULL);

		// ���ֻ����Ƶ�ļ����Ͳ����л�
		if (audioOnly_)
		{
			return;
		}

		BOOST_ASSERT(videoWnd_);

		// ��ȡ��ǰ״̬
		long mode;
		videoWnd_->get_FullScreenMode(&mode);

		if (0 == mode)
		{
			// ���浱ǰ��Ϣ��ˮ��
			TIF(videoWnd_->get_MessageDrain(&hDrain));

			// ����Ϣ��ˮ�����õ�������
			TIF(videoWnd_->put_MessageDrain(reinterpret_cast<OAHWND>(hWnd_)));

			// �л���ȫ��ģʽ
			TIF(videoWnd_->put_FullScreenMode(-1));
		}
		else
		{
			// �л��ش���ģʽ
			TIF(videoWnd_->put_FullScreenMode(0));

			// �ָ���Ϣ��ˮ��
			TIF(videoWnd_->put_MessageDrain(hDrain));

			// ��Ƶ���ڸ�λ
			TIF(videoWnd_->SetWindowForeground(-1));

			// ���ռ��̽���
			::UpdateWindow(hWnd_);
			::SetForegroundWindow(hWnd_);
			::SetFocus(hWnd_);
		}
	}
}