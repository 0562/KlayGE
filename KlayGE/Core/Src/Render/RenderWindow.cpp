#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Viewport.hpp>
#include <KlayGE/RenderTarget.hpp>

#include <KlayGE/RenderWindow.hpp>

namespace KlayGE
{
	class NullRenderWindow : public RenderWindow
	{
	public:
		std::wstring const & Description() const
		{
			static std::wstring const desc(L"Null Render Window");
			return desc;
		}

		void Destroy()
		{
		}

		void Resize(uint32_t /*width*/, uint32_t /*height*/)
		{
		}
		void Reposition(uint32_t /*left*/, uint32_t /*top*/)
		{
		}

		bool Closed() const
		{
			return true;
		}

		void CustomAttribute(std::string const & /*name*/, void* /*data*/)
		{
		}

		bool RequiresTextureFlipping() const
		{
			return false;
		}

		void SwapBuffers()
		{
		}
	};

	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	RenderWindow::RenderWindow()
	{
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	RenderWindow::~RenderWindow()
	{
	}

	// ���ؿն���
	/////////////////////////////////////////////////////////////////////////////////
	RenderWindowPtr RenderWindow::NullObject()
	{
		static RenderWindowPtr obj(new NullRenderWindow);
		return obj;
	}

	// ��ȡ�Ƿ���ȫ��״̬
	/////////////////////////////////////////////////////////////////////////////////
	bool RenderWindow::FullScreen() const
	{
		return isFullScreen_;
	}
}
