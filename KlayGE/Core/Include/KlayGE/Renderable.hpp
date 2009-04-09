// Renderable.hpp
// KlayGE ����Ⱦ������ ͷ�ļ�
// Ver 3.1.0
// ��Ȩ����(C) ������, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 3.1.0
// ���SceneObject (2005.11.2)
//
// 2.7.0
// GetWorld����ΪGetModelMatrix (2005.6.17)
//
// 2.3.0
// ������Render (2005.1.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDERABLE_HPP
#define _RENDERABLE_HPP

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>
#include <vector>

namespace KlayGE
{
	// Abstract class defining the interface all renderable objects must implement.
	class KLAYGE_CORE_API Renderable : public boost::enable_shared_from_this<Renderable>
	{
	public:
		virtual ~Renderable();

		virtual RenderTechniquePtr const & GetRenderTechnique() const = 0;
		virtual RenderLayoutPtr const & GetRenderLayout() const = 0;
		virtual std::wstring const & Name() const = 0;

		virtual void OnRenderBegin();
		virtual void OnRenderEnd();

		// These two functions are used for non-instancing rendering
		virtual void OnInstanceBegin(uint32_t id);
		virtual void OnInstanceEnd(uint32_t id);

		virtual Box const & GetBound() const = 0;

		virtual void AddToRenderQueue();

		virtual void Render();

		template <typename Iterator>
		void AssignInstances(Iterator begin, Iterator end)
		{
			instances_.resize(0);
			for (Iterator iter = begin; iter != end; ++ iter)
			{
				this->AddInstance(*iter);
			}
		}
		void AddInstance(SceneObjectPtr obj);

	protected:
		virtual void UpdateInstanceStream();

	protected:
		std::vector<boost::weak_ptr<SceneObject> > instances_;
	};
}

#endif		//_RENDERABLE_HPP
