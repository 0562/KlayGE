// Timer.cpp
// KlayGE ��ʱ�� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.6)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <ctime>

#include <KlayGE/Timer.hpp>

namespace KlayGE
{
	// ��ȡ��ʱ��ʵ��������ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	Timer& Timer::Instance()
	{
		static Timer timer;
		return timer;
	}

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	Timer::Timer()
			: baseTime_(0),
				stopTime_(0),
				timerStopped_(false)
	{
	}

	// ��ʱ����λ
	/////////////////////////////////////////////////////////////////////////////////
	void Timer::Reset()
	{
		baseTime_		= this->Time();
		stopTime_		= 0;
		timerStopped_	= false;
	}

	// ������ʱ��
	/////////////////////////////////////////////////////////////////////////////////
	void Timer::Start()
	{
		if (timerStopped_)
		{
			baseTime_ += this->AbsoluteTime() - stopTime_;
		}
		stopTime_		= 0;
		timerStopped_	= false;
	}

	// ֹͣ��ʱ��
	/////////////////////////////////////////////////////////////////////////////////
	void Timer::Stop()
	{
		stopTime_		= this->Time();
		timerStopped_	= true;
	}

	// ��ȡϵͳ���������ڵ�ʱ��
	/////////////////////////////////////////////////////////////////////////////////
	float Timer::AbsoluteTime()
	{
		return std::clock() * 0.001f;
	}

	// ��ȡ��ʱ����λ�����ڵ�ʱ��
	/////////////////////////////////////////////////////////////////////////////////
	float Timer::AppTime() const
	{
		return this->Time() - baseTime_;
	}

	// ��ȡʱ��
	/////////////////////////////////////////////////////////////////////////////////
	float Timer::Time() const
	{
		if (stopTime_ != 0)
		{
			return stopTime_;
		}
		else
		{
			return this->AbsoluteTime();
		}
	}
}
