// RenderTarget.cpp
// KlayGE ��ȾĿ���� ʵ���ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ����boost::timer��ʱ (2004.11.1)
//
// 2.1.2
// ����UpdateStats (2004.7.19)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>

#include <KlayGE/RenderTarget.hpp>

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	RenderTarget::RenderTarget()
					: active_(true),
						FPS_(0)
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	RenderTarget::~RenderTarget()
	{
	}

	// ��ȾĿ���������
	/////////////////////////////////////////////////////////////////////////////////
	int RenderTarget::Left() const
	{
		return left_;
	}

	// ��ȾĿ��Ķ�����
	/////////////////////////////////////////////////////////////////////////////////
	int RenderTarget::Top() const
	{
		return top_;
	}

	// ��ȾĿ��Ŀ��
	/////////////////////////////////////////////////////////////////////////////////
	int RenderTarget::Width() const
	{
		return width_;
	}

	// ��ȾĿ��ĸ߶�
	/////////////////////////////////////////////////////////////////////////////////
	int RenderTarget::Height() const
	{
		return height_;
	}

	// ��ȾĿ�����ɫ���
	/////////////////////////////////////////////////////////////////////////////////
	int RenderTarget::ColorDepth() const
	{
		return colorDepth_;
	}

	// ˢ����ȾĿ��
	/////////////////////////////////////////////////////////////////////////////////
	void RenderTarget::Update()
	{
		Context::Instance().SceneManagerInstance().Update();

		this->UpdateStats();

		this->SwapBuffers();
	}

	// ��ȡ�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	Viewport const & RenderTarget::GetViewport() const
	{
		return viewport_;
	}

	Viewport& RenderTarget::GetViewport()
	{
		return viewport_;
	}

	// �����ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	void RenderTarget::SetViewport(Viewport const & viewport)
	{
		viewport_ = viewport;
	}

	// ��ȡ��ȾĿ���ÿ��֡��
	/////////////////////////////////////////////////////////////////////////////////
	float RenderTarget::FPS() const
	{
		return FPS_;
	}

	// ����״̬
	/////////////////////////////////////////////////////////////////////////////////
	void RenderTarget::UpdateStats()
	{
		static float accumulateTime = 0;
		static long numFrames  = 0;
		
		// measure statistics
		++ numFrames;
		accumulateTime += static_cast<float>(timer_.elapsed());

		// check if new second
		if (accumulateTime > 1)
		{
			// new second - not 100% precise
			FPS_ = static_cast<float>(numFrames) / accumulateTime;

			accumulateTime = 0;
			numFrames  = 0;
		}

		timer_.restart();
	}

	// ��ȡ����ȾĿ���Ƿ��ڻ״̬
	/////////////////////////////////////////////////////////////////////////////////
	bool RenderTarget::Active() const
	{
		return active_;
	}
	
	// ���ø���ȾĿ���Ƿ��ڻ״̬
	/////////////////////////////////////////////////////////////////////////////////
	void RenderTarget::Active(bool state)
	{
		active_ = state;
	}
}        
