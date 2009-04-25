// OCTreeFactory.cpp
// KlayGE OCTree���������� ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2008.10.17)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/SceneManager.hpp>

#include <KlayGE/OCTree/OCTree.hpp>
#include <KlayGE/OCTree/OCTreeFactory.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4702)
#endif
#include <boost/lexical_cast.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <rapidxml/rapidxml.hpp>

void MakeSceneManager(KlayGE::SceneManagerPtr& ptr, void* extra_param)
{
	using namespace rapidxml;

	xml_node<>* node = static_cast<xml_node<>*>(extra_param);
	int octree_depth = 3;
	if (node != NULL)
	{
		xml_attribute<>* attr = node->first_attribute("depth");
		if (attr != NULL)
		{
			octree_depth = boost::lexical_cast<int>(attr->value());
		}
	}

	ptr = KlayGE::MakeSharedPtr<KlayGE::OCTree>(octree_depth);
}
