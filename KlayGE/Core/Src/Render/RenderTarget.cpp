#include <KlayGE/KlayGE.hpp>
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/SceneManager.hpp>

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
		SceneManager::Instance().Update();

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
		static float lastSecond = 0; // in second
		static float lastTime = 0; 
		static long numFrames  = 0;
		static bool firstRun = true ;
		bool needUpdate;

		if (firstRun)
		{ 
			firstRun = false;
			needUpdate = true;

			Timer::Instance().Reset();
		}
		else
		{ 
			// measure statistics
			needUpdate = false;
			++ numFrames;
			float thisTime(Timer::Instance().AppTime());

			// check frame time
			float frameTime(thisTime - lastTime);
			lastTime = thisTime;

			// check if new second
			if (thisTime - lastSecond > 1)
			{ 
				// new second - not 100% precise
				needUpdate = true ;
				FPS_ = static_cast<float>(numFrames) / (thisTime - lastSecond);

				lastSecond = thisTime;
				numFrames  = 0;
			}
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
