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

#define _WIN32_DCOM
#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Context.hpp>

#include <boost/assert.hpp>
#include <uuids.h>

#include <KlayGE/D3D9/D3D9RenderFactory.hpp>
#include <KlayGE/D3D9/D3D9RenderEngine.hpp>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/DShow/DShowVMR9Allocator.hpp>
#include <KlayGE/DShow/DShow.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "strmiids.lib")

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	DShowEngine::DShowEngine()
	{
		::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

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

		vmr_allocator_.reset();
		mediaEvent_.reset();
		mediaControl_.reset();
		filter_.reset();
		graph_.reset();
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
	void DShowEngine::Load(std::wstring const & fileName, TexturePtr tex)
	{
		this->Free();
		this->Init();

		IGraphBuilder* graph;
		::CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_ALL,
			IID_IGraphBuilder, reinterpret_cast<void**>(&graph));
		graph_ = MakeCOMPtr(graph);

		IBaseFilter* filter;
		::CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, reinterpret_cast<void**>(&filter));
		filter_ = MakeCOMPtr(filter);

		boost::shared_ptr<IVMRFilterConfig9> filter_config;
		{
			IVMRFilterConfig9* tmp;
			TIF(filter_->QueryInterface(IID_IVMRFilterConfig9, reinterpret_cast<void**>(&tmp)));
			filter_config = MakeCOMPtr(tmp);
		}

		TIF(filter_config->SetRenderingMode(VMR9Mode_Renderless));
		TIF(filter_config->SetNumberOfStreams(1));

		boost::shared_ptr<IVMRSurfaceAllocatorNotify9> vmr_surf_alloc_notify;
		{
			IVMRSurfaceAllocatorNotify9* tmp;
			TIF(filter_->QueryInterface(IID_IVMRSurfaceAllocatorNotify9, reinterpret_cast<void**>(&tmp)));
			vmr_surf_alloc_notify = MakeCOMPtr(tmp);
		}

		// create our surface allocator
		vmr_allocator_ = MakeCOMPtr(new DShowVMR9Allocator(tex));

		// let the allocator and the notify know about each other
		TIF(vmr_surf_alloc_notify->AdviseSurfaceAllocator(0xACDCACDC, vmr_allocator_.get()));
		TIF(vmr_allocator_->AdviseNotify(vmr_surf_alloc_notify.get()));

		TIF(graph_->AddFilter(filter_.get(), L"Video Mixing Renderer 9"));

		// ��ȡ DirectShow �ӿ�
		IMediaControl* mediaControl;
		TIF(graph_->QueryInterface(IID_IMediaControl,
			reinterpret_cast<void**>(&mediaControl)));
		mediaControl_ = MakeCOMPtr(mediaControl);

		IMediaEvent* mediaEvent;
		TIF(graph_->QueryInterface(IID_IMediaEvent,
			reinterpret_cast<void**>(&mediaEvent)));
		mediaEvent_ = MakeCOMPtr(mediaEvent);

		TIF(graph_->RenderFile(fileName.c_str(), NULL));

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
}
