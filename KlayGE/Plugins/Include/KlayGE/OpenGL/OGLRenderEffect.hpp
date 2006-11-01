// OGLRenderEffect.hpp
// KlayGE OpenGL��ȾЧ���� ͷ�ļ�
// Ver 3.5.0
// ��Ȩ����(C) ������, 2004-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.5.0
// ʹ�����µ�effectϵͳ (2006.11.1)
//
// 2.8.0
// ʹ��Cgʵ�� (2005.7.30)
//
// 2.5.0
// ȥ����Clone (2005.4.16)
//
// 2.0.4
// ���ν��� (2004.4.4)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLRENDEREFFECT_HPP
#define _OGLRENDEREFFECT_HPP

#define KLAYGE_LIB_NAME KlayGE_RenderEngine_OpenGL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/MapVector.hpp>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace KlayGE
{
	struct OGLRenderParameterDesc
	{
		RenderEffectParameterPtr param;

		CGparameter cg_handle;
	};

	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class OGLRenderEffect : public RenderEffect
	{
	public:

	private:
		RenderTechniquePtr MakeRenderTechnique();
	};
	typedef boost::shared_ptr<OGLRenderEffect> OGLRenderEffectPtr;

	class OGLRenderTechnique : public RenderTechnique
	{
	public:
		explicit OGLRenderTechnique(RenderEffect& effect)
			: RenderTechnique(effect)
		{
		}

	private:
		RenderPassPtr MakeRenderPass();

		void DoBegin();
		void DoEnd();
	};
	typedef boost::shared_ptr<OGLRenderTechnique> OGLRenderTechniquePtr;

	class OGLRenderPass : public RenderPass
	{
	public:
		explicit OGLRenderPass(RenderEffect& effect)
			: RenderPass(effect)
		{
		}
		~OGLRenderPass();

		int32_t AttribIndex(VertexElementUsage usage, uint8_t usage_index);

	private:
		void DoLoad();

		void DoBegin();
		void DoEnd();

	private:
		boost::array<CGprogram, ST_NUM_SHADER_TYPES> shaders_;
		boost::array<CGprofile, ST_NUM_SHADER_TYPES> profiles_;

		boost::array<std::vector<OGLRenderParameterDesc>, ST_NUM_SHADER_TYPES> param_descs_;
		
		MapVector<std::pair<VertexElementUsage, uint8_t>, uint8_t> vertex_varyings_;

	private:
		CGprogram compile_shader(CGprofile profile, std::string const & name, std::string const & text);
		void create_vertex_shader(CGprofile profile, std::string const & name, std::string const & text);
		void create_pixel_shader(CGprofile profile, std::string const & name, std::string const & text);

		std::string shader_text() const;
	};
	typedef boost::shared_ptr<OGLRenderPass> OGLRenderPassPtr;
}

#endif		// _OGLRENDEREFFECT_HPP
