// RenderTarget.cpp
// KlayGE ��ȾĿ���� ʵ���ļ�
// Ver 2.1.2
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.2
// ����UpdateStats (2004.7.19)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Timer.hpp>
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
		Timer::Instance().Reset();
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
	}

	// ��ȡ�ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	const Viewport& RenderTarget::GetViewport() const
	{
		return viewport_;
	}

	Viewport& RenderTarget::GetViewport()
	{
		return viewport_;
	}

	// �����ӿ�
	/////////////////////////////////////////////////////////////////////////////////
	void RenderTarget::SetViewport(const Viewport& viewport)
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
		static float lastSecond = 0; 
		static long numFrames  = 0;
		
		// measure statistics
		++ numFrames;
		float thisTime(Timer::Instance().AppTime());

		// check if new second
		if (thisTime - lastSecond > 1)
		{ 
			// new second - not 100% precise
			FPS_ = static_cast<float>(numFrames) / (thisTime - lastSecond);

			lastSecond = thisTime;
			numFrames  = 0;
		}
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
