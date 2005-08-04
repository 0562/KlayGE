// RenderFactory.cpp
// KlayGE ��Ⱦ������ ʵ���ļ�
// Ver 2.8.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// ������LoadEffect (2005.7.25)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Font.hpp>

#include <KlayGE/RenderFactory.hpp>

namespace KlayGE
{
	RenderFactory::~RenderFactory()
	{
	}

	FontPtr RenderFactory::MakeFont(std::string const & fontName, uint32_t fontHeight, uint32_t flags)
	{
		return FontPtr(new Font(fontName, fontHeight, flags));
	}

	RenderEffectPtr RenderFactory::LoadEffect(std::string const & effectName)
	{
		RenderEffectPtr ret;

		for (effect_pool_type::iterator iter = effect_pool_.begin(); iter != effect_pool_.end();)
		{
			if (!(iter->second.lock()))
			{
				iter = effect_pool_.erase(iter);
			}
			else
			{
				++ iter;
			}
		}

		effect_pool_type::iterator eiter = effect_pool_.find(effectName);
		if (eiter == effect_pool_.end())
		{
			ResIdentifierPtr file(ResLoader::Instance().Load(effectName));

			file->seekg(0, std::ios_base::end);
			std::vector<char> data(file->tellg());
			file->seekg(0);
			file->read(&data[0], static_cast<std::streamsize>(data.size()));

			ret = this->DoMakeRenderEffect(std::string(data.begin(), data.end()));
			if (ret)
			{
				effect_pool_.insert(std::make_pair(effectName, boost::weak_ptr<RenderEffect>(ret)));
			}
		}
		else
		{
			ret = RenderEffectPtr(eiter->second);
		}

		return ret;
	}
}
