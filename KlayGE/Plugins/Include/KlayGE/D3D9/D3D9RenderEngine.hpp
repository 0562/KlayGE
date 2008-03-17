// D3D9RenderEngine.hpp
// KlayGE D3D9��Ⱦ������ ͷ�ļ�
// Ver 3.4.0
// ��Ȩ����(C) ������, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.4.0
// ������TexelToPixelOffset (2006.8.27)
//
// 3.0.0
// ȥ���˹̶���ˮ�� (2005.8.18)
//
// 2.8.0
// ������RenderDeviceCaps (2005.7.17)
// ����StencilBuffer��ز��� (2005.7.20)
//
// 2.7.0
// ȥ����TextureCoordSet (2005.6.26)
// TextureAddressingMode, TextureFiltering��TextureAnisotropy�Ƶ�Texture�� (2005.6.27)
//
// 2.4.0
// ������PolygonMode (2005.3.20)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D9RENDERENGINE_HPP
#define _D3D9RENDERENGINE_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_D3D9
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/Vector.hpp>

#include <KlayGE/D3D9/D3D9AdapterList.hpp>

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <vector>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/ShaderObject.hpp>
#include <KlayGE/D3D9/D3D9Typedefs.hpp>
#include <KlayGE/D3D9/D3D9GraphicsBuffer.hpp>

namespace KlayGE
{
	class D3D9AdapterList;
	class D3D9Adapter;

	class D3D9RenderEngine : public RenderEngine
	{
	public:
		D3D9RenderEngine();
		~D3D9RenderEngine();

		std::wstring const & Name() const;

		ID3D9Ptr const & D3DObject() const;
		ID3D9DevicePtr const & D3DDevice() const;
		void AttachD3DDevice(ID3D9DevicePtr const & device, D3DDEVTYPE dev_type);

		void CreateRenderWindow(std::string const & name, RenderSettings const & settings);
		void SetStateObjects(RenderStateObject const & rs_obj, ShaderObject const & shader_obj);

		void StartRendering();

		void BeginFrame();
		void EndFrame();

		uint16_t StencilBufferBitDepth();

		void ScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		// Directly mapping texels to pixels by offset by 0.5
		// For more info see the doc page titled "Directly Mapping Texels to Pixels"
		float4 TexelToPixelOffset() const
		{
			return float4(-0.5f, 0.5f, 0, 0);
		}

		void Resize(uint32_t width, uint32_t height);
		bool FullScreen() const;
		void FullScreen(bool fs);

		void OnLostDevice();
		void OnResetDevice();

	private:
		void DoBindFrameBuffer(FrameBufferPtr fb);
		void DoRender(RenderTechnique const & tech, RenderLayout const & rl);

		void FillRenderDeviceCaps(D3DDEVTYPE dev_type);
		void InitRenderStates();

		void DoRenderSWInstance(RenderTechnique const & tech, RenderLayout const & rl);
		void DoRenderHWInstance(RenderTechnique const & tech, RenderLayout const & rl);
		void RenderRLSWInstance(RenderTechnique const & tech, RenderLayout const & rl);
		void RenderRL(RenderTechnique const & tech, RenderLayout const & rl);

	private:
		D3D9AdapterList const & D3DAdapters() const;
		D3D9Adapter const & ActiveAdapter() const;

		// Direct3D rendering device
		// Only created after top-level window created
		ID3D9Ptr		d3d_;
		ID3D9DevicePtr	d3dDevice_;

		// List of D3D drivers installed (video cards)
		// Enumerates itself
		D3D9AdapterList adapterList_;

		uint32_t last_num_vertex_stream_;

		boost::function<void (RenderTechnique const &, RenderLayout const &)> RenderInstance;

		RenderStateObject cur_render_state_obj_;
		boost::array<std::vector<Sampler>, ShaderObject::ST_NumShaderTypes> cur_samplers_;
	};

	typedef boost::shared_ptr<D3D9RenderEngine> D3D9RenderEnginePtr;
}

#endif			// _D3D9RENDERENGINE_HPP
