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
		void SetShader(RenderEffect& /*effect*/, boost::shared_ptr<std::vector<uint32_t> > const & /*shader_desc_ids*/,
			uint32_t /*tech_index*/, uint32_t /*pass_index*/)
		{
			is_validate_ = true;
		}

		ShaderObjectPtr Clone(RenderEffect& /*effect*/)
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
