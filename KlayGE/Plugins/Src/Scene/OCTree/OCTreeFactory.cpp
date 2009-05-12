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
#include <KlayGE/Math.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/XMLDom.hpp>

#include <KlayGE/OCTree/OCTree.hpp>
#include <KlayGE/OCTree/OCTreeFactory.hpp>

void MakeSceneManager(KlayGE::SceneManagerPtr& ptr, KlayGE::XMLNodePtr const & extra_param)
{
	int octree_depth = 3;
	if (extra_param)
	{
		KlayGE::XMLAttributePtr attr = extra_param->Attrib("depth");
		if (attr)
		{
			octree_depth = attr->ValueInt();
		}
	}

	ptr = KlayGE::MakeSharedPtr<KlayGE::OCTree>(octree_depth);
}
