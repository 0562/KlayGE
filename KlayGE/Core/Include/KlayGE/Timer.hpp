// Timer.hpp
// KlayGE ��ʱ�� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.8.6)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TIMER_HPP
#define _TIMER_HPP

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ������ʱ�������ص�λ: �룬����ģʽ
	/////////////////////////////////////////////////////////////////////////////////
	class Timer
	{
	public:
		static Timer& Instance();

		void Reset();
		void Start();
		void Stop();
		float AbsoluteTime();
		float AppTime();

	private:
		Timer();

		float Time();

		float	baseTime_;
		float	stopTime_;
		bool	timerStopped_;
	};
}

#endif		// _TIMER_HPP