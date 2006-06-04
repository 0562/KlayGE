// D3D9OcclusionQuery.hpp
// KlayGE D3D9�ڵ������ ʵ���ļ�
// Ver 3.0.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 3.0.0
// ���ν��� (2005.9.27)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9OCCLUSIONQUERY_HPP
#define _D3D9OCCLUSIONQUERY_HPP

#include <KlayGE/OcclusionQuery.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_D3D9.lib")
#endif

namespace KlayGE
{
	class D3D9OcclusionQuery : public OcclusionQuery
	{
	public:
		D3D9OcclusionQuery();

		void Begin();
		void End();

		uint32_t SamplesPassed();

	private:
		ID3D9QueryPtr query_;
	};
}

#endif		// _D3D9OCCLUSIONQUERY_HPP
