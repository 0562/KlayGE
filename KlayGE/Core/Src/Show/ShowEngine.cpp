// ShowEngine.cpp
// KlayGE �������� ʵ���ļ�
// Ver 1.2.8.11
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.10
// ��string�����ַ���ָ�� (2002.10.27)
//
// 1.2.8.11
// ����UNICODE���� (2002.11.7)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <KlayGE/Show.hpp>

namespace KlayGE
{
	ShowEngine::~ShowEngine()
	{
	}

	// ���Բ���
	/////////////////////////////////////////////////////////////////////////////////
	bool ShowEngine::CanPlay() const
	{
		return (SS_Stopped == this->state_) || (SS_Paused == this->state_);
	}

	// ����ֹͣ
	/////////////////////////////////////////////////////////////////////////////////
	bool ShowEngine::CanStop() const
	{
		return (SS_Playing == this->state_) || (SS_Paused == this->state_);
	}

	// ������ͣ
	/////////////////////////////////////////////////////////////////////////////////
	bool ShowEngine::CanPause() const
	{
		return (SS_Playing == this->state_) || (SS_Paused == this->state_);
	}

	// ��ʼ�����
	/////////////////////////////////////////////////////////////////////////////////
	bool ShowEngine::IsInitialized() const
	{
		return this->state_ != SS_Uninit;
	}

	// ����
	/////////////////////////////////////////////////////////////////////////////////
	void ShowEngine::Play()
	{
		if (this->CanPlay())
		{
			this->DoPlay();

			state_ = SS_Playing;
		}
	}

	// ��ͣ����
	/////////////////////////////////////////////////////////////////////////////////
	void ShowEngine::Pause()
	{
		if (this->CanPause())
		{
			this->DoPause();

			state_ = SS_Paused;
		}
	}

	// ֹͣ����
	/////////////////////////////////////////////////////////////////////////////////
	void ShowEngine::Stop()
	{
		if (this->CanStop())
		{
			this->DoStop();

			state_ = SS_Stopped;
		}
	}
}