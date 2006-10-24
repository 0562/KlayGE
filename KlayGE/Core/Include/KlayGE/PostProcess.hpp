// PostProcess.hpp
// KlayGE ���ڴ����� ͷ�ļ�
// Ver 3.3.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://klayge.sourceforge.net
//
// 3.3.0
// ���ν��� (2006.6.23)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _POSTPROCESS_HPP
#define _POSTPROCESS_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderableHelper.hpp>

namespace KlayGE
{
	class PostProcess : public RenderableHelper
	{
	public:
		explicit PostProcess(RenderTechniquePtr tech);
		virtual ~PostProcess()
		{
		}

		virtual void Source(TexturePtr const & tex);
		virtual void Destinate(RenderTargetPtr const & rt);

		virtual void Apply();

		virtual void OnRenderBegin();

	protected:
		TexturePtr src_texture_;
		RenderTargetPtr render_target_;

		GraphicsBufferPtr pos_vb_;
	};
}

#endif		// _POSTPROCESS_HPP
