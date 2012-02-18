// ShaderObject.cpp
// KlayGE shader������ ʵ���ļ�
// Ver 3.5.0
// ��Ȩ����(C) ������, 2006
// Homepage: http://www.klayge.org
//
// 3.5.0
// ���ν��� (2006.11.2)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>

#include <string>
#include <vector>

#include <KlayGE/ShaderObject.hpp>

namespace KlayGE
{
	class NullShaderObject : public ShaderObject
	{
	public:
		void AttachShader(ShaderType /*type*/, RenderEffect const & /*effect*/, std::vector<uint32_t> const & /*shader_desc_ids*/)
		{
			is_validate_ = true;
		}

		void AttachShader(ShaderType /*type*/, RenderEffect const & /*effect*/, ShaderObjectPtr const & /*shared_so*/)
		{
			is_validate_ = true;
		}

		void LinkShaders(RenderEffect const & /*effect*/)
		{
			is_validate_ = true;
		}

		ShaderObjectPtr Clone(RenderEffect const & /*effect*/)
		{
			return ShaderObject::NullObject();
		}

		void Bind()
		{
		}

		void Unbind()
		{
		}
	};

	ShaderObjectPtr ShaderObject::NullObject()
	{
		static ShaderObjectPtr obj = MakeSharedPtr<NullShaderObject>();
		return obj;
	}
}
