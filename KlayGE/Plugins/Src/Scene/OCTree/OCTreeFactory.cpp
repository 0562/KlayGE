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

extern "C"
{
	void MakeSceneManager(KlayGE::SceneManagerPtr& ptr, boost::program_options::variables_map const & vm)
	{
		ptr = KlayGE::MakeSharedPtr<KlayGE::OCTree>(vm["octree.depth"].as<int>());
	}	

	bool Match(char const * name, char const * compiler)
	{
		std::string cur_compiler_str = KLAYGE_COMPILER_TOOLSET;
#ifdef KLAYGE_DEBUG
		cur_compiler_str += "_d";
#endif

		if ((std::string("OCTree") == name) && (cur_compiler_str == compiler))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
