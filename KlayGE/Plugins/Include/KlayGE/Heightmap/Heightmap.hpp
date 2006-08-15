// HeightMap.hpp
// KlayGE HeightMap���������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ʹ��boost::function (2006.7.23)
//
// 2.0.0
// ���ν��� (2003.10.5)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _HEIGHTMAP_HPP
#define _HEIGHTMAP_HPP

#define KLAYGE_LIB_NAME KlayGE_Terrain_HeightMap
#include <KlayGE/config/auto_link.hpp>

#include <vector>
#include <boost/function.hpp>

namespace KlayGE
{
	// �߶�ͼ��������
	/////////////////////////////////////////////////////////////////////////////////
	class HeightMap
	{
	public:
		void BuildTerrain(float start_x, float start_y, float end_x, float end_y, float span_x, float span_y,
			std::vector<float3>& vertices, std::vector<uint16_t>& indices,
			boost::function<float(float, float)> HeightFunc);
	};
}

#endif		// _HEIGHTMAP_HPP
