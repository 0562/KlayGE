// Heightmap.hpp
// KlayGE Heightmap���������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://klayge.sourceforge.net
//
// 2.0.0
// ���ν��� (2003.10.5)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _HEIGHTMAP_HPP
#define _HEIGHTMAP_HPP

#include <vector>
#include <KlayGE/Vector3.hpp>

namespace KlayGE
{
	// �߶�ͼ��������
	/////////////////////////////////////////////////////////////////////////////////
	class Heightmap
	{
	public:
		template <typename HeightFunc>
		void Terrain(float startX, float startY, float width, float height, float spanX, float spanY,
			std::vector<Vector3>& vertices, std::vector<uint16_t>& indices,
			HeightFunc& Height)
		{
			vertices.clear();
			indices.clear();

			for (float y = startY; y < startY + height; y += spanY)
			{
				for (float x = startX; x < startX + width; x += spanX)
				{
					Vector3 vec(x, y, Height(x, y));

					std::vector<Vector3>::iterator iter(std::find(vertices.begin(), vertices.end(), vec));
					if (iter == vertices.end())
					{
						indices.push_back(vertices.size());
						vertices.push_back(vec);
					}
					else
					{
						indices.push_back(std::distance(iter, vertices.begin()));
					}
				}
			}
		}
	};
}

#endif		// _HEIGHTMAP_HPP

