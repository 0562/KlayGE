// mesh_extractor.hpp
// KlayGE 3DSMax����������ȡ�� ʵ���ļ�
// Ver 2.5.0
// ��Ȩ����(C) ������, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.5.0
// ���ν��� (2005.5.1)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _MESHML_EXTRACTOR_HPP
#define _MESHML_EXTRACTOR_HPP

#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "meshml.hpp"

namespace KlayGE
{
	struct vertex_t
	{
		Point3 pos;
		std::vector<Point2> tex;
	};

	typedef std::vector<vertex_t> vertices_t;

	struct triangle_t
	{
		int	vertex_index[3];
	};

	typedef std::vector<triangle_t> triangles_t;

	typedef std::pair<std::string, std::string> texture_slot_t;
	typedef std::vector<texture_slot_t> texture_slots_t;

	struct object_info_t
	{
		std::string		name;

		texture_slots_t texture_slots;

		vertices_t		vertices;
		triangles_t		triangles;
	};

	class meshml_extractor
	{
	public:
		meshml_extractor();

		void export_objects(std::vector<INode*> const & nodes);
		void write_xml(std::basic_string<TCHAR> const & file_name);

	private:
		void extract_object(INode* node);

	private:
		typedef std::vector<object_info_t> objects_info_t;

		objects_info_t objs_info_;
	};
}

#endif		// _MESHML_EXTRACTOR_HPP
