// D3D10OcclusionQuery.hpp
// KlayGE D3D10�ڵ������ ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2008.9.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D10OCCLUSIONQUERY_HPP
#define _D3D10OCCLUSIONQUERY_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_D3D10
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/OcclusionQuery.hpp>

namespace KlayGE
{
	class D3D10OcclusionQuery : public OcclusionQuery
	{
	public:
		D3D10OcclusionQuery();

		void Begin();
		void End();

		uint64_t SamplesPassed();

	private:
		ID3D10QueryPtr query_;
	};
}

#endif		// _D3D10OCCLUSIONQUERY_HPP
