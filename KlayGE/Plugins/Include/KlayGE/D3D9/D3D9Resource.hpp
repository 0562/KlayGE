// D3D9Resource.hpp
// KlayGE D3D9��Դ�� ͷ�ļ�
// Ver 2.3.1
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.3.1
// ���ν��� (2005.3.2)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RESOURCE_HPP
#define _D3D9RESOURCE_HPP

#include <boost/smart_ptr.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9Resource
	{
	public:
		D3D9Resource()
			: reseted_(true)
		{
		}

		virtual ~D3D9Resource()
		{
		}

		void OnLostDevice()
		{
			if (reseted_)
			{
				this->DoOnLostDevice();
				reseted_ = false;
			}
		}
		void OnResetDevice()
		{
			if (!reseted_)
			{
				this->DoOnResetDevice();
				reseted_ = true;
			}
		}

	private:
		virtual void DoOnLostDevice() = 0;
		virtual void DoOnResetDevice() = 0;

	private:
		bool reseted_;
	};

	typedef boost::shared_ptr<D3D9Resource> D3D9ResourcePtr;
}

#endif			// _D3D9RESOURCE_HPP
