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

#include <KlayGE/PreDeclare.hpp>
#include <vector>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_Core_d.lib")
#else
	#pragma comment(lib, "KlayGE_Core.lib")
#endif

namespace KlayGE
{
	// Abstract class defining the interface all renderable objects must implement.
	class Renderable : public boost::enable_shared_from_this<Renderable>
	{
	public:
		virtual ~Renderable();

		virtual RenderEffectPtr GetRenderEffect() const = 0;
		virtual VertexBufferPtr GetVertexBuffer() const = 0;
		virtual std::wstring const & Name() const = 0;

		virtual void OnRenderBegin();
		virtual void OnRenderEnd();

		// These two functions are used for non-instancing rendering
		virtual void OnInstanceBegin(uint32_t id);
		virtual void OnInstanceEnd(uint32_t id);

		virtual Box GetBound() const = 0;

		virtual void AddToRenderQueue();

		void Render();

		template <typename Iterator>
		void AssignInstances(Iterator begin, Iterator end)
		{
			instances_.assign(begin, end);
		}
		void AddInstance(SceneObjectPtr obj);

	protected:
		virtual void UpdateInstanceStream();

	protected:
		std::vector<SceneObjectPtr> instances_;
	};
}

#endif		//_RENDERABLE_HPP
