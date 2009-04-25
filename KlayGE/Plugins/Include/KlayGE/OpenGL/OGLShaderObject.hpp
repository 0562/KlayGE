// OGLShaderObject.hpp
// KlayGE OpenGL shader������ ͷ�ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2006-2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ��Ϊֱ�Ӵ���RenderEffect (2008.7.4)
//
// 3.5.0
// ���ν��� (2006.11.2)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLSHADEROBJECT_HPP
#define _OGLSHADEROBJECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/MapVector.hpp>
#include <KlayGE/ShaderObject.hpp>

#include <boost/function.hpp>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace KlayGE
{
	class OGLShaderObject : public ShaderObject
	{
	public:
		OGLShaderObject();
		~OGLShaderObject();

		std::string GenShaderText(RenderEffect const & effect) const;

		void SetShader(RenderEffect& effect, boost::shared_ptr<std::vector<shader_desc> > const & shader_descs);
		ShaderObjectPtr Clone(RenderEffect& effect);

		void Bind();
		void Unbind();

	private:
		struct parameter_bind_t
		{
			std::string combined_sampler_name;
			RenderEffectParameterPtr param;
			CGparameter cg_param;
			boost::function<void()> func;
		};
		typedef std::vector<parameter_bind_t> parameter_binds_t;

		parameter_bind_t GetBindFunc(CGparameter cg_param, RenderEffectParameterPtr const & param);

	private:
		boost::shared_ptr<std::vector<shader_desc> > shader_descs_;
		boost::shared_ptr<std::string> shader_text_;

		CGprogram combo_program_;
		boost::array<CGprofile, ST_NumShaderTypes> profiles_;

		parameter_binds_t param_binds_;
		boost::array<bool, ST_NumShaderTypes> is_shader_validate_;

		boost::array<std::vector<std::pair<TexturePtr, SamplerStateObjectPtr> >, ST_NumShaderTypes> samplers_;

		mutable std::vector<std::pair<std::string, std::pair<RenderEffectParameterPtr, RenderEffectParameterPtr> > > tex_sampler_binds_;
	};

	typedef boost::shared_ptr<OGLShaderObject> OGLShaderObjectPtr;
}

#endif			// _OGLSHADEROBJECT_HPP
