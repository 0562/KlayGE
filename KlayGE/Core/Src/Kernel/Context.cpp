// Context.cpp
// KlayGE ���泡���� ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ���ν��� (2007.12.19)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/AudioFactory.hpp>
#include <KlayGE/InputFactory.hpp>
#include <KlayGE/ShowFactory.hpp>

#include <KlayGE/Context.hpp>

namespace KlayGE
{
	Context::Context()
	{
#ifdef KLAYGE_COMPILER_MSVC
#ifdef KLAYGE_DEBUG
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
#endif

		sceneMgr_ = SceneManager::NullObject().get();

		renderFactory_ = RenderFactory::NullObject().get();
		audioFactory_ = AudioFactory::NullObject().get();
		inputFactory_ = InputFactory::NullObject().get();
		showFactory_ = ShowFactory::NullObject().get();
	}
}
