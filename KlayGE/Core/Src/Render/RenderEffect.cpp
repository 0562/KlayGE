// RenderEffect.cpp
// KlayGE 渲染效果类 实现文件
// Ver 3.11.0
// 版权所有(C) 龚敏敏, 2003-2010
// Homepage: http://www.klayge.org
//
// 3.11.0
// Thead-safe singleton (2010.7.6)
//
// 3.9.0
// 直接从fxml文件读取特效脚本 (2009.4.21)
// 支持Buffer类型 (2009.5.14)
// 支持macro标签 (2009.5.22)
//
// 3.8.0
// 支持CBuffer (2008.10.6)
//
// 3.6.0
// 增加了Clone (2007.6.11)
//
// 3.5.0
// 改用基于xml的特效格式 (2006.10.21)
//
// 3.2.0
// 支持了bool类型 (2006.3.8)
//
// 3.0.0
// 增加了RenderTechnique和RenderPass (2005.9.4)
//
// 2.8.0
// 增加了Do*函数，使用模板方法模式 (2005.7.24)
// 使用新的自动更新参数的方法 (2005.7.25)
//
// 2.2.0
// 统一使用istream作为资源标示符 (2004.10.26)
//
// 2.1.2
// 增加了Parameter (2004.5.26)
//
// 2.0.3
// 初次建立 (2003.3.2)
// 修改了SetTexture的参数 (2004.3.6)
//
// 修改记录
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>

#include <KFL/CXX2a/format.hpp>
#include <KFL/ErrorHandling.hpp>
#include <KFL/Util.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Context.hpp>
#include <KFL/Math.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderStateObject.hpp>
#include <KlayGE/RenderView.hpp>
#include <KlayGE/ShaderObject.hpp>
#include <KlayGE/Texture.hpp>
#include <KFL/XMLDom.hpp>
#include <KFL/Hash.hpp>
#include <KFL/CXX17/filesystem.hpp>

#include <fstream>
#include <iterator>
#include <string>
#ifdef KLAYGE_CXX17_LIBRARY_CHARCONV_SUPPORT
#include <charconv>
#endif

#include <boost/assert.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <KlayGE/RenderEffect.hpp>

namespace
{
	using namespace KlayGE;

	uint32_t const KFX_VERSION = 0x0150;

#if KLAYGE_IS_DEV_PLATFORM
	std::unique_ptr<RenderVariable> LoadVariable(
		RenderEffect const& effect, XMLNode const& node, RenderEffectDataType type, uint32_t array_size);
#endif
	std::unique_ptr<RenderVariable> StreamInVariable(
		RenderEffect const& effect, ResIdentifier& res, RenderEffectDataType type, uint32_t array_size);
#if KLAYGE_IS_DEV_PLATFORM
	void StreamOutVariable(std::ostream& os, RenderVariable const& var);
#endif

#if KLAYGE_IS_DEV_PLATFORM
	std::span<std::pair<char const *, size_t> const> GetTypeDefines()
	{
#define NAME_AND_HASH(name) std::make_pair(name, CT_HASH(name))
		static std::pair<char const *, size_t> const types[] =
		{
			NAME_AND_HASH("bool"),
			NAME_AND_HASH("string"),
			NAME_AND_HASH("texture1D"),
			NAME_AND_HASH("texture2D"),
			NAME_AND_HASH("texture2DMS"),
			NAME_AND_HASH("texture3D"),
			NAME_AND_HASH("textureCUBE"),
			NAME_AND_HASH("texture1DArray"),
			NAME_AND_HASH("texture2DArray"),
			NAME_AND_HASH("texture2DMSArray"),
			NAME_AND_HASH("texture3DArray"),
			NAME_AND_HASH("textureCUBEArray"),
			NAME_AND_HASH("sampler"),
			NAME_AND_HASH("shader"),
			NAME_AND_HASH("uint"),
			NAME_AND_HASH("uint2"),
			NAME_AND_HASH("uint3"),
			NAME_AND_HASH("uint4"),
			NAME_AND_HASH("int"),
			NAME_AND_HASH("int2"),
			NAME_AND_HASH("int3"),
			NAME_AND_HASH("int4"),
			NAME_AND_HASH("float"),
			NAME_AND_HASH("float2"),
			NAME_AND_HASH("float2x2"),
			NAME_AND_HASH("float2x3"),
			NAME_AND_HASH("float2x4"),
			NAME_AND_HASH("float3"),
			NAME_AND_HASH("float3x2"),
			NAME_AND_HASH("float3x3"),
			NAME_AND_HASH("float3x4"),
			NAME_AND_HASH("float4"),
			NAME_AND_HASH("float4x2"),
			NAME_AND_HASH("float4x3"),
			NAME_AND_HASH("float4x4"),
			NAME_AND_HASH("buffer"),
			NAME_AND_HASH("structured_buffer"),
			NAME_AND_HASH("byte_address_buffer"),
			NAME_AND_HASH("rw_buffer"),
			NAME_AND_HASH("rw_structured_buffer"),
			NAME_AND_HASH("rw_texture1D"),
			NAME_AND_HASH("rw_texture2D"),
			NAME_AND_HASH("rw_texture3D"),
			NAME_AND_HASH("rw_texture1DArray"),
			NAME_AND_HASH("rw_texture2DArray"),
			NAME_AND_HASH("rw_byte_address_buffer"),
			NAME_AND_HASH("append_structured_buffer"),
			NAME_AND_HASH("consume_structured_buffer"),
			NAME_AND_HASH("rasterizer_ordered_buffer"),
			NAME_AND_HASH("rasterizer_ordered_byte_address_buffer"),
			NAME_AND_HASH("rasterizer_ordered_structured_buffer"),
			NAME_AND_HASH("rasterizer_ordered_texture1D"),
			NAME_AND_HASH("rasterizer_ordered_texture1DArray"),
			NAME_AND_HASH("rasterizer_ordered_texture2D"),
			NAME_AND_HASH("rasterizer_ordered_texture2DArray"),
			NAME_AND_HASH("rasterizer_ordered_texture3D"),
			NAME_AND_HASH("struct"),
		};
#undef NAME_AND_HASH
		KLAYGE_STATIC_ASSERT(std::size(types) == REDT_count);

		return MakeSpan(types);
	}

	RenderEffectDataType TypeFromName(std::string_view name)
	{
		auto const types = GetTypeDefines();

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < types.size(); ++ i)
		{
			if (types[i].second == name_hash)
			{
				return static_cast<RenderEffectDataType>(i);
			}
		}

		KFL_UNREACHABLE("Invalid type name");
	}

	std::string_view TypeNameFromCode(RenderEffectDataType type)
	{
		auto const types = GetTypeDefines();
		if (type < types.size())
		{
			return types[type].first;
		}

		KFL_UNREACHABLE("Invalid type");
	}

	ShadeMode ShadeModeFromName(std::string_view name)
	{
		static size_t constexpr sms_hash[] =
		{
			CT_HASH("flat"),
			CT_HASH("gouraud")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(sms_hash); ++ i)
		{
			if (sms_hash[i] == name_hash)
			{
				return static_cast<ShadeMode>(i);
			}
		}

		KFL_UNREACHABLE("Invalid ShadeMode name");
	}

	CompareFunction CompareFunctionFromName(std::string_view name)
	{
		static size_t constexpr cfs_hash[] =
		{
			CT_HASH("always_fail"),
			CT_HASH("always_pass"),
			CT_HASH("less"),
			CT_HASH("less_equal"),
			CT_HASH("equal"),
			CT_HASH("not_equal"),
			CT_HASH("greater_equal"),
			CT_HASH("greater")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(cfs_hash); ++ i)
		{
			if (cfs_hash[i] == name_hash)
			{
				return static_cast<CompareFunction>(i);
			}
		}

		KFL_UNREACHABLE("Invalid CompareFunction name");
	}

	CullMode CullModeFromName(std::string_view name)
	{
		static size_t constexpr cms_hash[] =
		{
			CT_HASH("none"),
			CT_HASH("front"),
			CT_HASH("back")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(cms_hash); ++ i)
		{
			if (cms_hash[i] == name_hash)
			{
				return static_cast<CullMode>(i);
			}
		}

		KFL_UNREACHABLE("Invalid CullMode name");
	}

	PolygonMode PolygonModeFromName(std::string_view name)
	{
		static size_t constexpr pms_hash[] =
		{
			CT_HASH("point"),
			CT_HASH("line"),
			CT_HASH("fill")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(pms_hash); ++ i)
		{
			if (pms_hash[i] == name_hash)
			{
				return static_cast<PolygonMode>(i);
			}
		}

		KFL_UNREACHABLE("Invalid PolygonMode name");
	}

	AlphaBlendFactor AlphaBlendFactorFromName(std::string_view name)
	{
		static size_t constexpr abfs_hash[] =
		{
			CT_HASH("zero"),
			CT_HASH("one"),
			CT_HASH("src_alpha"),
			CT_HASH("dst_alpha"),
			CT_HASH("inv_src_alpha"),
			CT_HASH("inv_dst_alpha"),
			CT_HASH("src_color"),
			CT_HASH("dst_color"),
			CT_HASH("inv_src_color"),
			CT_HASH("inv_dst_color"),
			CT_HASH("src_alpha_sat"),
			CT_HASH("blend_factor"),
			CT_HASH("inv_blend_factor"),
			CT_HASH("src1_alpha"),
			CT_HASH("inv_src1_alpha"),
			CT_HASH("src1_color"),
			CT_HASH("inv_src1_color")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(abfs_hash); ++ i)
		{
			if (abfs_hash[i] == name_hash)
			{
				return static_cast<AlphaBlendFactor>(i);
			}
		}

		KFL_UNREACHABLE("Invalid AlphaBlendFactor name");
	}

	BlendOperation BlendOperationFromName(std::string_view name)
	{
		static size_t constexpr bops_hash[] =
		{
			CT_HASH("add"),
			CT_HASH("sub"),
			CT_HASH("rev_sub"),
			CT_HASH("min"),
			CT_HASH("max")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(bops_hash); ++ i)
		{
			if (bops_hash[i] == name_hash)
			{
				return static_cast<BlendOperation>(i + 1);
			}
		}

		KFL_UNREACHABLE("Invalid BlendOperation name");
	}

	StencilOperation StencilOperationFromName(std::string_view name)
	{
		static size_t constexpr sops_hash[] =
		{
			CT_HASH("keep"),
			CT_HASH("zero"),
			CT_HASH("replace"),
			CT_HASH("incr"),
			CT_HASH("decr"),
			CT_HASH("invert"),
			CT_HASH("incr_wrap"),
			CT_HASH("decr_wrap")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(sops_hash); ++ i)
		{
			if (sops_hash[i] == name_hash)
			{
				return static_cast<StencilOperation>(i);
			}
		}

		KFL_UNREACHABLE("Invalid StencilOperation name");
	}

	TexFilterOp TexFilterOpFromName(std::string_view name)
	{
		static size_t constexpr tfs_hash[] =
		{
			CT_HASH("min_mag_mip_point"),
			CT_HASH("min_mag_point_mip_linear"),
			CT_HASH("min_point_mag_linear_mip_point"),
			CT_HASH("min_point_mag_mip_linear"),
			CT_HASH("min_linear_mag_mip_point"),
			CT_HASH("min_linear_mag_point_mip_linear"),
			CT_HASH("min_mag_linear_mip_point"),
			CT_HASH("min_mag_mip_linear")
		};

		int cmp;
		std::string_view f;
		if (0 == name.find("cmp_"))
		{
			cmp = 1;
			f = name.substr(4);
		}
		else
		{
			cmp = 0;
			f = name;
		}
		size_t const f_hash = HashRange(f.begin(), f.end());
		for (uint32_t i = 0; i < std::size(tfs_hash); ++ i)
		{
			if (tfs_hash[i] == f_hash)
			{
				return static_cast<TexFilterOp>((cmp << 4) + i);
			}
		}
		if (CT_HASH("anisotropic") == f_hash)
		{
			return static_cast<TexFilterOp>((cmp << 4) + TFO_Anisotropic);
		}

		KFL_UNREACHABLE("Invalid TexFilterOp name");
	}

	TexAddressingMode TexAddressingModeFromName(std::string_view name)
	{
		static size_t constexpr tams_hash[] =
		{
			CT_HASH("wrap"),
			CT_HASH("mirror"),
			CT_HASH("clamp"),
			CT_HASH("border")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(tams_hash); ++ i)
		{
			if (tams_hash[i] == name_hash)
			{
				return static_cast<TexAddressingMode>(i);
			}
		}

		KFL_UNREACHABLE("Invalid TexAddressingMode name");
	}

	LogicOperation LogicOperationFromName(std::string_view name)
	{
		static size_t constexpr lops_hash[] =
		{
			CT_HASH("clear"),
			CT_HASH("set"),
			CT_HASH("copy"),
			CT_HASH("copy_inverted"),
			CT_HASH("noop"),
			CT_HASH("invert"),
			CT_HASH("and"),
			CT_HASH("nand"),
			CT_HASH("or"),
			CT_HASH("nor"),
			CT_HASH("xor"),
			CT_HASH("equiv"),
			CT_HASH("and_reverse"),
			CT_HASH("and_inverted"),
			CT_HASH("or_reverse"),
			CT_HASH("or_inverted")
		};

		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < std::size(lops_hash); ++ i)
		{
			if (lops_hash[i] == name_hash)
			{
				return static_cast<LogicOperation>(i);
			}
		}

		KFL_UNREACHABLE("Invalid LogicOperation name");
	}

	bool BoolFromStr(std::string_view name)
	{
		if (("true" == name) || ("1" == name))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int RetrieveIndex(XMLNode const & node)
	{
		int index = 0;
		XMLAttributePtr attr = node.Attrib("index");
		if (attr)
		{
			index = attr->ValueInt();
		}
		return index;
	}

	std::string RetrieveProfile(XMLNode const & node)
	{
		XMLAttributePtr attr = node.Attrib("profile");
		if (attr)
		{
			return std::string(attr->ValueString());
		}
		else
		{
			return "auto";
		}
	}

	std::string RetrieveFuncName(XMLNode const & node)
	{
		std::string_view value = node.Attrib("value")->ValueString();
		return std::string(value.substr(0, value.find("(")));
	}
#endif

	class RenderVariableIOable : public RenderVariable
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		virtual void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) = 0;
#endif

		virtual void StreamIn(RenderEffect const& effect, ResIdentifier& res) = 0;

#if KLAYGE_IS_DEV_PLATFORM
		virtual void StreamOut(std::ostream& os) const = 0;
#endif
	};

	template <typename T>
	class RenderVariableConcrete : public RenderVariableIOable
	{
	public:
		explicit RenderVariableConcrete(bool in_cbuff = false)
			: in_cbuff_(in_cbuff)
		{
			if (!in_cbuff_)
			{
				new (data_.val) T;
			}
		}

		~RenderVariableConcrete() override
		{
			if (!in_cbuff_)
			{
				this->RetriveT().~T();
			}
		}

		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = this->MakeInstance(in_cbuff_);
			auto& concrete = checked_cast<RenderVariableConcrete<T>&>(*ret);
			if (in_cbuff_)
			{
				concrete.data_ = data_;
			}
			T val;
			this->Value(val);
			concrete = val;
			return ret;
		}

		RenderVariable& operator=(T const& value) override
		{
			if (in_cbuff_)
			{
				auto& cbuff_desc = this->RetriveCBufferDesc();
				auto* cbuff = this->CBuffer();
				T& val_in_cbuff = *(cbuff->template VariableInBuff<T>(cbuff_desc.offset));
				if (val_in_cbuff != value)
				{
					val_in_cbuff = value;
					cbuff->Dirty(true);
				}
			}
			else
			{
				this->RetriveT() = value;
			}
			return *this;
		}

		void Value(T& val) const override
		{
			if (in_cbuff_)
			{
				auto const& cbuff_desc = this->RetriveCBufferDesc();
				auto* cbuff = this->CBuffer();
				val = *(cbuff->template VariableInBuff<T>(cbuff_desc.offset));
			}
			else
			{
				val = this->RetriveT();
			}
		}

		void BindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index, uint32_t offset, uint32_t stride) override
		{
			if (!in_cbuff_)
			{
				T val = this->RetriveT();
				this->RetriveT().~T();
				in_cbuff_ = true;
				CBufferDesc cbuff_desc;
				cbuff_desc.effect = &effect;
				cbuff_desc.cbuff_index = cbuff_index;
				cbuff_desc.offset = offset;
				cbuff_desc.stride = stride;
				this->RetriveCBufferDesc() = std::move(cbuff_desc);
				this->operator=(val);
			}
		}

		void RebindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index) override
		{
			BOOST_ASSERT(in_cbuff_);
			auto& cbuff_desc = this->RetriveCBufferDesc();
			cbuff_desc.effect = &effect;
			cbuff_desc.cbuff_index = cbuff_index;
		}

		bool InCBuffer() const override
		{
			return in_cbuff_;
		}
		RenderEffectConstantBuffer* CBuffer() const override
		{
			auto& cbuff_desc = this->RetriveCBufferDesc();
			return cbuff_desc.effect->CBufferByIndex(cbuff_desc.cbuff_index);
		}
		uint32_t CBufferIndex() const override
		{
			return this->RetriveCBufferDesc().cbuff_index;
		}
		uint32_t CBufferOffset() const override
		{
			return this->RetriveCBufferDesc().offset;
		}
		uint32_t Stride() const override
		{
			return this->RetriveCBufferDesc().stride;
		}

	protected:
		T& RetriveT()
		{
			union Raw2T
			{
				uint8_t* raw;
				T* t;
			} r2t;
			r2t.raw = data_.val;
			return *r2t.t;
		}
		T const& RetriveT() const
		{
			union Raw2T
			{
				uint8_t const* raw;
				T const* t;
			} r2t;
			r2t.raw = data_.val;
			return *r2t.t;
		}

		CBufferDesc& RetriveCBufferDesc()
		{
			return data_.cbuff_desc;
		}
		CBufferDesc const& RetriveCBufferDesc() const
		{
			return data_.cbuff_desc;
		}

		virtual std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) = 0;

	protected:
		bool in_cbuff_;
		union VarData
		{
			CBufferDesc cbuff_desc;
			uint8_t val[sizeof(T)];
		};
		VarData data_ = {};
	};

	class RenderVariableBool final : public RenderVariableConcrete<bool>
	{
	public:
		explicit RenderVariableBool(bool in_cbuff = false)
			: RenderVariableConcrete<bool>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			bool tmp = false;
			if (auto attr = node.Attrib("value"))
			{
				tmp = BoolFromStr(attr->ValueString());
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			bool tmp;
			res.read(&tmp, sizeof(tmp));
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			bool tmp;
			this->Value(tmp);
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<bool>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableBool>(in_cbuff);
		}
	};

	class RenderVariableUInt final : public RenderVariableConcrete<uint32_t>
	{
	public:
		explicit RenderVariableUInt(bool in_cbuff = false)
			: RenderVariableConcrete<uint32_t>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			uint32_t tmp = 0;
			if (auto attr = node.Attrib("value"))
			{
				tmp = attr->ValueUInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t tmp;
			res.read(&tmp, sizeof(tmp));
			*this = LE2Native(tmp);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			uint32_t tmp;
			this->Value(tmp);
			tmp = Native2LE(tmp);
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<uint32_t>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt>(in_cbuff);
		}
	};

	class RenderVariableInt final : public RenderVariableConcrete<int32_t>
	{
	public:
		explicit RenderVariableInt(bool in_cbuff = false)
			: RenderVariableConcrete<int32_t>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			int32_t tmp = 0;
			if (auto attr = node.Attrib("value"))
			{
				tmp = attr->ValueInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			int32_t tmp;
			res.read(&tmp, sizeof(tmp));
			*this = LE2Native(tmp);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			int32_t tmp;
			this->Value(tmp);
			tmp = Native2LE(tmp);
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<int32_t>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt>(in_cbuff);
		}
	};

	class RenderVariableFloat final : public RenderVariableConcrete<float>
	{
	public:
		explicit RenderVariableFloat(bool in_cbuff = false)
			: RenderVariableConcrete<float>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			float tmp = 0;
			if (auto attr = node.Attrib("value"))
			{
				tmp = attr->ValueFloat();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			float tmp;
			res.read(&tmp, sizeof(tmp));
			*this = LE2Native(tmp);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			float tmp;
			this->Value(tmp);
			tmp = Native2LE(tmp);
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<float>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat>(in_cbuff);
		}
	};

	class RenderVariableUInt2 final : public RenderVariableConcrete<uint2>
	{
	public:
		explicit RenderVariableUInt2(bool in_cbuff = false)
			: RenderVariableConcrete<uint2>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			uint2 tmp(0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueUInt();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueUInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint2 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			uint2 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<uint2>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt2>(in_cbuff);
		}
	};

	class RenderVariableUInt3 final : public RenderVariableConcrete<uint3>
	{
	public:
		explicit RenderVariableUInt3(bool in_cbuff = false)
			: RenderVariableConcrete<uint3>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			uint3 tmp(0, 0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueUInt();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueUInt();
			}
			if (auto attr = node.Attrib("z"))
			{
				tmp.y() = attr->ValueUInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint3 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			uint3 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<uint3>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt3>(in_cbuff);
		}
	};

	class RenderVariableUInt4 final : public RenderVariableConcrete<uint4>
	{
	public:
		explicit RenderVariableUInt4(bool in_cbuff = false)
			: RenderVariableConcrete<uint4>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			uint4 tmp(0, 0, 0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueUInt();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueUInt();
			}
			if (auto attr = node.Attrib("z"))
			{
				tmp.y() = attr->ValueUInt();
			}
			if (auto attr = node.Attrib("w"))
			{
				tmp.y() = attr->ValueUInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint4 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			uint4 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<uint4>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt4>(in_cbuff);
		}
	};

	class RenderVariableInt2 final : public RenderVariableConcrete<int2>
	{
	public:
		explicit RenderVariableInt2(bool in_cbuff = false)
			: RenderVariableConcrete<int2>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			int2 tmp(0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueInt();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			int2 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			int2 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<int2>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt2>(in_cbuff);
		}
	};

	class RenderVariableInt3 final : public RenderVariableConcrete<int3>
	{
	public:
		explicit RenderVariableInt3(bool in_cbuff = false)
			: RenderVariableConcrete<int3>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			int3 tmp(0, 0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueInt();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueInt();
			}
			if (auto attr = node.Attrib("z"))
			{
				tmp.z() = attr->ValueInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			int3 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			int3 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<int3>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt3>(in_cbuff);
		}
	};

	class RenderVariableInt4 final : public RenderVariableConcrete<int4>
	{
	public:
		explicit RenderVariableInt4(bool in_cbuff = false)
			: RenderVariableConcrete<int4>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			int4 tmp(0, 0, 0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueInt();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueInt();
			}
			if (auto attr = node.Attrib("z"))
			{
				tmp.z() = attr->ValueInt();
			}
			if (auto attr = node.Attrib("w"))
			{
				tmp.z() = attr->ValueInt();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			int4 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			int4 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<int4>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt4>(in_cbuff);
		}
	};

	class RenderVariableFloat2 final : public RenderVariableConcrete<float2>
	{
	public:
		explicit RenderVariableFloat2(bool in_cbuff = false)
			: RenderVariableConcrete<float2>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			float2 tmp(0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueFloat();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueFloat();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			float2 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			float2 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<float2>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat2>(in_cbuff);
		}
	};

	class RenderVariableFloat3 final : public RenderVariableConcrete<float3>
	{
	public:
		explicit RenderVariableFloat3(bool in_cbuff = false)
			: RenderVariableConcrete<float3>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			float3 tmp(0, 0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueFloat();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueFloat();
			}
			if (auto attr = node.Attrib("z"))
			{
				tmp.z() = attr->ValueFloat();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			float3 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			float3 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<float3>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat3>(in_cbuff);
		}
	};

	class RenderVariableFloat4 final : public RenderVariableConcrete<float4>
	{
	public:
		explicit RenderVariableFloat4(bool in_cbuff = false)
			: RenderVariableConcrete<float4>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			float4 tmp(0, 0, 0, 0);
			if (auto attr = node.Attrib("x"))
			{
				tmp.x() = attr->ValueFloat();
			}
			if (auto attr = node.Attrib("y"))
			{
				tmp.y() = attr->ValueFloat();
			}
			if (auto attr = node.Attrib("z"))
			{
				tmp.z() = attr->ValueFloat();
			}
			if (auto attr = node.Attrib("w"))
			{
				tmp.w() = attr->ValueFloat();
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			float4 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			float4 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		using RenderVariableConcrete<float4>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat4>(in_cbuff);
		}
	};

	class RenderVariableFloat4x4 final : public RenderVariableConcrete<float4x4>
	{
	public:
		explicit RenderVariableFloat4x4(bool in_cbuff = false)
			: RenderVariableConcrete<float4x4>(in_cbuff)
		{
		}

		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableFloat4x4>(in_cbuff_);
			if (in_cbuff_)
			{
				ret->data_ = data_;

				float4x4 val;
				RenderVariableConcrete<float4x4>::Value(val);
				RenderVariableConcrete<float4x4>::operator=(val);
			}
			else
			{
				ret->RetriveT() = this->RetriveT();
			}
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			float4x4 tmp(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					auto attr = node.Attrib(std::string("_") + static_cast<char>('0' + y) + static_cast<char>('0' + x));
					if (attr)
					{
						tmp[y * 4 + x] = attr->ValueFloat();
					}
				}
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			float4x4 tmp;
			res.read(&tmp, sizeof(tmp));
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = LE2Native(tmp[i]);
			}
			*this = tmp;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			float4x4 tmp;
			this->Value(tmp);
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				tmp[i] = Native2LE(tmp[i]);
			}
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
#endif

		RenderVariable& operator=(float4x4 const& value) override
		{
			return RenderVariableConcrete<float4x4>::operator=(MathLib::transpose(value));
		}

		void Value(float4x4& val) const override
		{
			RenderVariableConcrete<float4x4>::Value(val);
			val = MathLib::transpose(val);
		}

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat4x4>(in_cbuff);
		}
	};

	class RenderVariableSampler final : public RenderVariableConcrete<SamplerStateObjectPtr>
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			SamplerStateDesc desc;
			for (XMLNodePtr state_node = node.FirstNode("state"); state_node; state_node = state_node->NextSibling("state"))
			{
				std::string_view const name = state_node->Attrib("name")->ValueString();
				size_t const name_hash = HashRange(name.begin(), name.end());

				XMLAttributePtr const value_attr = state_node->Attrib("value");
				std::string_view value_str;
				if (value_attr)
				{
					value_str = value_attr->ValueString();
				}

				if (CT_HASH("filtering") == name_hash)
				{
					desc.filter = TexFilterOpFromName(value_str);
				}
				else if (CT_HASH("address_u") == name_hash)
				{
					desc.addr_mode_u = TexAddressingModeFromName(value_str);
				}
				else if (CT_HASH("address_v") == name_hash)
				{
					desc.addr_mode_v = TexAddressingModeFromName(value_str);
				}
				else if (CT_HASH("address_w") == name_hash)
				{
					desc.addr_mode_w = TexAddressingModeFromName(value_str);
				}
				else if (CT_HASH("max_anisotropy") == name_hash)
				{
					desc.max_anisotropy = static_cast<uint8_t>(value_attr->ValueUInt());
				}
				else if (CT_HASH("min_lod") == name_hash)
				{
					desc.min_lod = value_attr->ValueFloat();
				}
				else if (CT_HASH("max_lod") == name_hash)
				{
					desc.max_lod = value_attr->ValueFloat();
				}
				else if (CT_HASH("mip_map_lod_bias") == name_hash)
				{
					desc.mip_map_lod_bias = value_attr->ValueFloat();
				}
				else if (CT_HASH("cmp_func") == name_hash)
				{
					desc.cmp_func = CompareFunctionFromName(value_str);
				}
				else if (CT_HASH("border_clr") == name_hash)
				{
					if (auto attr = state_node->Attrib("r"))
					{
						desc.border_clr.r() = attr->ValueFloat();
					}
					if (auto attr = state_node->Attrib("g"))
					{
						desc.border_clr.g() = attr->ValueFloat();
					}
					if (auto attr = state_node->Attrib("b"))
					{
						desc.border_clr.b() = attr->ValueFloat();
					}
					if (auto attr = state_node->Attrib("a"))
					{
						desc.border_clr.a() = attr->ValueFloat();
					}
				}
				else
				{
					KFL_UNREACHABLE("Invalid sampler state name");
				}
			}

			*this = Context::Instance().RenderFactoryInstance().MakeSamplerStateObject(desc);
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			SamplerStateDesc desc;
			res.read(&desc, sizeof(desc));
			desc.border_clr[0] = LE2Native(desc.border_clr[0]);
			desc.border_clr[1] = LE2Native(desc.border_clr[1]);
			desc.border_clr[2] = LE2Native(desc.border_clr[2]);
			desc.border_clr[3] = LE2Native(desc.border_clr[3]);
			desc.addr_mode_u = LE2Native(desc.addr_mode_u);
			desc.addr_mode_v = LE2Native(desc.addr_mode_v);
			desc.addr_mode_w = LE2Native(desc.addr_mode_w);
			desc.filter = LE2Native(desc.filter);
			desc.min_lod = LE2Native(desc.min_lod);
			desc.max_lod = LE2Native(desc.max_lod);
			desc.mip_map_lod_bias = LE2Native(desc.mip_map_lod_bias);
			desc.cmp_func = LE2Native(desc.cmp_func);

			*this = Context::Instance().RenderFactoryInstance().MakeSamplerStateObject(desc);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			SamplerStateObjectPtr tmp;
			this->Value(tmp);
			SamplerStateDesc desc = tmp->GetDesc();
			desc.border_clr[0] = Native2LE(desc.border_clr[0]);
			desc.border_clr[1] = Native2LE(desc.border_clr[1]);
			desc.border_clr[2] = Native2LE(desc.border_clr[2]);
			desc.border_clr[3] = Native2LE(desc.border_clr[3]);
			desc.addr_mode_u = Native2LE(desc.addr_mode_u);
			desc.addr_mode_v = Native2LE(desc.addr_mode_v);
			desc.addr_mode_w = Native2LE(desc.addr_mode_w);
			desc.filter = Native2LE(desc.filter);
			desc.min_lod = Native2LE(desc.min_lod);
			desc.max_lod = Native2LE(desc.max_lod);
			desc.mip_map_lod_bias = Native2LE(desc.mip_map_lod_bias);
			desc.cmp_func = Native2LE(desc.cmp_func);
			os.write(reinterpret_cast<char const*>(&desc), sizeof(desc));
		}
#endif

		using RenderVariableConcrete<SamplerStateObjectPtr>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			KFL_UNUSED(in_cbuff);
			return MakeUniquePtr<RenderVariableSampler>();
		}
	};

	class RenderVariableString final : public RenderVariableConcrete<std::string>
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			std::string tmp;
			if (auto attr = node.Attrib("value"))
			{
				tmp = std::string(attr->ValueString());
			}
			*this = tmp;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			*this = ReadShortString(res);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::string tmp;
			this->Value(tmp);
			WriteShortString(os, tmp);
		}
#endif

		using RenderVariableConcrete<std::string>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			KFL_UNUSED(in_cbuff);
			return MakeUniquePtr<RenderVariableString>();
		}
	};

	class RenderVariableShader final : public RenderVariableConcrete<ShaderDesc>
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			ShaderDesc desc;
			desc.profile = RetrieveProfile(node);
			desc.func_name = RetrieveFuncName(node);
			*this = desc;
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			ShaderDesc desc;
			desc.profile = ReadShortString(res);
			desc.func_name = ReadShortString(res);
			*this = desc;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			ShaderDesc tmp;
			this->Value(tmp);
			WriteShortString(os, tmp.profile);
			WriteShortString(os, tmp.func_name);
		}
#endif

		using RenderVariableConcrete<ShaderDesc>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			KFL_UNUSED(in_cbuff);
			return MakeUniquePtr<RenderVariableShader>();
		}
	};

	template <typename T>
	class RenderVariableArray : public RenderVariableConcrete<std::vector<T>>
	{
	public:
		explicit RenderVariableArray(bool in_cbuff = false)
			: RenderVariableConcrete<std::vector<T>>(in_cbuff)
		{
		}

		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = this->MakeInstance(this->in_cbuff_);
			auto& concrete = checked_cast<RenderVariableArray<T>&>(*ret);
			if (this->in_cbuff_)
			{
				concrete.data_ = this->data_;
				concrete.size_ = this->size_;

				auto const& src_cbuff_desc = this->RetriveCBufferDesc();
				uint8_t const* src = this->CBuffer()->template VariableInBuff<uint8_t>(src_cbuff_desc.offset);

				auto const& dst_cbuff_desc = concrete.RetriveCBufferDesc();
				uint8_t* dst = concrete.CBuffer()->template VariableInBuff<uint8_t>(dst_cbuff_desc.offset);

				for (size_t i = 0; i < size_; ++i)
				{
					*reinterpret_cast<T*>(dst) = *reinterpret_cast<T const*>(src);
					src += src_cbuff_desc.stride;
					dst += dst_cbuff_desc.stride;
				}

				concrete.CBuffer()->Dirty(true);
			}
			else
			{
				concrete.RetriveT() = this->RetriveT();
			}
			return ret;
		}

		RenderVariable& operator=(std::vector<T> const& value) override
		{
			return this->operator=(MakeSpan(value));
		}

		RenderVariable& operator=(std::span<T const> value) override
		{
			if (this->in_cbuff_)
			{
				uint8_t const* src = reinterpret_cast<uint8_t const*>(value.data());

				auto& cbuff_desc = this->RetriveCBufferDesc();
				uint8_t* dst = this->CBuffer()->template VariableInBuff<uint8_t>(cbuff_desc.offset);

				size_ = static_cast<uint32_t>(value.size());
				for (int i = 0; i < value.size(); ++i)
				{
					*reinterpret_cast<T*>(dst) = *reinterpret_cast<T const*>(src);
					src += sizeof(T);
					dst += cbuff_desc.stride;
				}

				this->CBuffer()->Dirty(true);
			}
			else
			{
				this->RetriveT() = std::vector<T>(value.begin(), value.end());
			}
			return *this;
		}

		void Value(std::vector<T>& val) const override
		{
			if (this->in_cbuff_)
			{
				auto const& cbuff_desc = this->RetriveCBufferDesc();
				uint8_t const* src = this->CBuffer()->template VariableInBuff<uint8_t>(cbuff_desc.offset);

				val.resize(size_);

				for (size_t i = 0; i < size_; ++i)
				{
					val[i] = *reinterpret_cast<T const*>(src);
					src += cbuff_desc.stride;
				}
			}
			else
			{
				val = this->RetriveT();
			}
		}

	protected:
		uint32_t size_ = 0;
	};

	class RenderVariableBoolArray final : public RenderVariableArray<uint32_t>
	{
	public:
		explicit RenderVariableBoolArray(bool in_cbuff = false)
			: RenderVariableArray<uint32_t>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<bool> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						if (index < strs.size())
						{
							boost::algorithm::trim(strs[index]);
							init_val[index] = BoolFromStr(strs[index]);
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<uint8_t> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));

				std::vector<bool> bool_vec(init_val.size());
				for (size_t i = 0; i < init_val.size(); ++i)
				{
					bool_vec[i] = init_val[i] ? true : false;
				}
				*this = bool_vec;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<bool> init_val;
			this->Value(init_val);

			uint32_t len = static_cast<uint32_t>(init_val.size());
			len = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len), sizeof(len));
			if (len > 0)
			{
				std::vector<uint8_t> const uint8_vec(init_val.begin(), init_val.end());
				os.write(reinterpret_cast<char const*>(uint8_vec.data()), len * sizeof(uint8_vec[0]));
			}
		}
#endif

		RenderVariable& operator=(std::vector<bool> const& value) override
		{
			std::vector<uint8_t> const uint8_vec(value.begin(), value.end());
			return this->operator=(MakeSpan(reinterpret_cast<bool const*>(uint8_vec.data()), uint8_vec.size()));
		}

		RenderVariable& operator=(std::span<bool const> value) override
		{
			if (this->in_cbuff_)
			{
				uint8_t const* src = reinterpret_cast<uint8_t const*>(value.data());

				auto& cbuff_desc = this->RetriveCBufferDesc();
				uint8_t* dst = this->CBuffer()->template VariableInBuff<uint8_t>(cbuff_desc.offset);

				size_ = static_cast<uint32_t>(value.size());
				for (int i = 0; i < value.size(); ++i)
				{
					*reinterpret_cast<uint32_t*>(dst) = *reinterpret_cast<bool const*>(src);
					src += sizeof(bool);
					dst += cbuff_desc.stride;
				}

				this->CBuffer()->Dirty(true);
			}
			else
			{
				this->RetriveT() = std::vector<uint32_t>(value.begin(), value.end());
			}
			return *this;
		}

		void Value(std::vector<bool>& val) const override
		{
			if (this->in_cbuff_)
			{
				auto const& cbuff_desc = this->RetriveCBufferDesc();
				uint8_t const* src = this->CBuffer()->template VariableInBuff<uint8_t>(cbuff_desc.offset);

				val.resize(size_);

				for (size_t i = 0; i < size_; ++i)
				{
					val[i] = *reinterpret_cast<uint32_t const*>(src) ? true : false;
					src += cbuff_desc.stride;
				}
			}
			else
			{
				auto const& uint32_vec = this->RetriveT();
				val.resize(uint32_vec.size());
				for (size_t i = 0; i < uint32_vec.size(); ++i)
				{
					val[i] = uint32_vec[i] ? true : false;
				}
			}
		}

		using RenderVariableArray<uint32_t>::operator=;
		using RenderVariableArray<uint32_t>::Value;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableBoolArray>(in_cbuff);
		}
	};

	class RenderVariableUIntArray final : public RenderVariableArray<uint32_t>
	{
	public:
		explicit RenderVariableUIntArray(bool in_cbuff = false)
			: RenderVariableArray<uint32_t>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<uint32_t> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						if (index < strs.size())
						{
							boost::algorithm::trim(strs[index]);
							init_val[index] = std::stoul(strs[index]);
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<uint32_t> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				for (uint32_t i = 0; i < len; ++i)
				{
					init_val[i] = LE2Native(init_val[i]);
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<uint32_t> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				for (uint32_t i = 0; i < len; ++i)
				{
					init_val[i] = Native2LE(init_val[i]);
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<uint32_t>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUIntArray>(in_cbuff);
		}
	};

	class RenderVariableIntArray final : public RenderVariableArray<int32_t>
	{
	public:
		explicit RenderVariableIntArray(bool in_cbuff = false)
			: RenderVariableArray<int32_t>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<int32_t> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						if (index < strs.size())
						{
							boost::algorithm::trim(strs[index]);
							init_val[index] = std::stol(strs[index]);
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<int32_t> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				for (uint32_t i = 0; i < len; ++i)
				{
					init_val[i] = LE2Native(init_val[i]);
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<int32_t> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				for (uint32_t i = 0; i < len; ++i)
				{
					init_val[i] = Native2LE(init_val[i]);
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<int32_t>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableIntArray>(in_cbuff);
		}
	};

	class RenderVariableFloatArray final : public RenderVariableArray<float>
	{
	public:
		explicit RenderVariableFloatArray(bool in_cbuff = false)
			: RenderVariableArray<float>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<float> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						if (index < strs.size())
						{
							boost::algorithm::trim(strs[index]);
							init_val[index] = std::stof(strs[index]);
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<float> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				for (uint32_t i = 0; i < len; ++i)
				{
					init_val[i] = LE2Native(init_val[i]);
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<float> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				for (uint32_t i = 0; i < len; ++i)
				{
					init_val[i] = Native2LE(init_val[i]);
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<float>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloatArray>(in_cbuff);
		}
	};

	class RenderVariableUInt2Array final : public RenderVariableArray<uint2>
	{
	public:
		explicit RenderVariableUInt2Array(bool in_cbuff = false)
			: RenderVariableArray<uint2>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<uint2> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stoul(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<uint2> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<uint2> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<uint2>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt2Array>(in_cbuff);
		}
	};

	class RenderVariableUInt3Array final : public RenderVariableArray<uint3>
	{
	public:
		explicit RenderVariableUInt3Array(bool in_cbuff = false)
			: RenderVariableArray<uint3>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<uint3> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stoul(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<uint3> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<uint3> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<uint3>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt3Array>(in_cbuff);
		}
	};

	class RenderVariableUInt4Array final : public RenderVariableArray<uint4>
	{
	public:
		explicit RenderVariableUInt4Array(bool in_cbuff = false)
			: RenderVariableArray<uint4>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<uint4> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stoul(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<uint4> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<uint4> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<uint4>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableUInt4Array>(in_cbuff);
		}
	};

	class RenderVariableInt2Array final : public RenderVariableArray<int2>
	{
	public:
		explicit RenderVariableInt2Array(bool in_cbuff = false)
			: RenderVariableArray<int2>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<int2> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stol(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<int2> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<int2> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<int2>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt2Array>(in_cbuff);
		}
	};

	class RenderVariableInt3Array final : public RenderVariableArray<int3>
	{
	public:
		explicit RenderVariableInt3Array(bool in_cbuff = false)
			: RenderVariableArray<int3>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<int3> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stol(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<int3> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<int3> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<int3>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt3Array>(in_cbuff);
		}
	};

	class RenderVariableInt4Array final : public RenderVariableArray<int4>
	{
	public:
		explicit RenderVariableInt4Array(bool in_cbuff = false)
			: RenderVariableArray<int4>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<int4> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stol(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<int4> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<int4> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<int4>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableInt4Array>(in_cbuff);
		}
	};

	class RenderVariableFloat2Array final : public RenderVariableArray<float2>
	{
	public:
		explicit RenderVariableFloat2Array(bool in_cbuff = false)
			: RenderVariableArray<float2>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<float2> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stof(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<float2> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<float2> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<float2>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat2Array>(in_cbuff);
		}
	};

	class RenderVariableFloat3Array final : public RenderVariableArray<float3>
	{
	public:
		explicit RenderVariableFloat3Array(bool in_cbuff = false)
			: RenderVariableArray<float3>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<float3> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stof(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<float3> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<float3> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<float3>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat3Array>(in_cbuff);
		}
	};

	class RenderVariableFloat4Array final : public RenderVariableArray<float4>
	{
	public:
		explicit RenderVariableFloat4Array(bool in_cbuff = false)
			: RenderVariableArray<float4>(in_cbuff)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<float4> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stof(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<float4> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<float4> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		using RenderVariableArray<float4>::operator=;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat4Array>(in_cbuff);
		}
	};

	class RenderVariableFloat4x4Array final : public RenderVariableConcrete<std::vector<float4x4>>
	{
	public:
		explicit RenderVariableFloat4x4Array(bool in_cbuff = false)
			: RenderVariableConcrete<std::vector<float4x4>>(in_cbuff)
		{
		}

		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableFloat4x4Array>(in_cbuff_);
			if (in_cbuff_)
			{
				ret->data_ = data_;
				ret->size_ = size_;

				auto const& src_cbuff_desc = this->RetriveCBufferDesc();
				uint8_t const* src = this->CBuffer()->template VariableInBuff<uint8_t>(src_cbuff_desc.offset);

				auto const& dst_cbuff_desc = ret->RetriveCBufferDesc();
				uint8_t* dst = ret->CBuffer()->template VariableInBuff<uint8_t>(dst_cbuff_desc.offset);

				memcpy(dst, src, size_ * sizeof(float4x4));

				ret->CBuffer()->Dirty(true);
			}
			else
			{
				ret->RetriveT() = this->RetriveT();
			}
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);

			if (XMLNodePtr value_node = node.FirstNode("value"))
			{
				value_node = value_node->FirstNode();
				if (value_node && (XNT_CData == value_node->Type()))
				{
					std::string_view const value_str = value_node->ValueString();
					std::vector<std::string> strs;
					boost::algorithm::split(strs, value_str, boost::is_any_of(","));
					std::vector<float4x4> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 15) / 16)),
						float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
					size_t const dim = init_val[0].size();
					for (size_t index = 0; index < init_val.size(); ++index)
					{
						for (size_t j = 0; j < dim; ++j)
						{
							if (index * dim + j < strs.size())
							{
								boost::algorithm::trim(strs[index * dim + j]);
								init_val[index][j] = std::stof(strs[index * dim + j]);
							}
						}
					}
					*this = init_val;
				}
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			uint32_t len;
			res.read(&len, sizeof(len));
			len = LE2Native(len);
			if (len > 0)
			{
				std::vector<float4x4> init_val(len);
				res.read(&init_val[0], len * sizeof(init_val[0]));
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = LE2Native(init_val[i][j]);
					}
				}
				*this = init_val;
			}
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::vector<float4x4> init_val;
			this->Value(init_val);

			uint32_t const len = static_cast<uint32_t>(init_val.size());
			uint32_t const len_le = Native2LE(len);
			os.write(reinterpret_cast<char const*>(&len_le), sizeof(len_le));
			if (len > 0)
			{
				size_t const dim = init_val[0].size();
				for (uint32_t i = 0; i < len; ++i)
				{
					for (size_t j = 0; j < dim; ++j)
					{
						init_val[i][j] = Native2LE(init_val[i][j]);
					}
				}
				os.write(reinterpret_cast<char const*>(init_val.data()), len * sizeof(init_val[0]));
			}
		}
#endif

		RenderVariable& operator=(std::vector<float4x4> const& value) override
		{
			return this->operator=(MakeSpan(value));
		}

		RenderVariable& operator=(std::span<float4x4 const> value) override
		{
			if (in_cbuff_)
			{
				float4x4 const* src = value.data();

				auto& cbuff_desc = this->RetriveCBufferDesc();
				float4x4* dst = this->CBuffer()->template VariableInBuff<float4x4>(cbuff_desc.offset);

				size_ = static_cast<uint32_t>(value.size());
				for (int i = 0; i < value.size(); ++i)
				{
					*dst = MathLib::transpose(*src);
					++src;
					++dst;
				}

				this->CBuffer()->Dirty(true);
			}
			else
			{
				this->RetriveT() = std::vector<float4x4>(value.begin(), value.end());
			}
			return *this;
		}

		void Value(std::vector<float4x4>& val) const override
		{
			if (in_cbuff_)
			{
				auto const& cbuff_desc = this->RetriveCBufferDesc();
				float4x4 const* src = this->CBuffer()->template VariableInBuff<float4x4>(cbuff_desc.offset);

				val.resize(size_);
				float4x4* dst = val.data();

				for (size_t i = 0; i < size_; ++i)
				{
					*dst = MathLib::transpose(*src);
					++src;
					++dst;
				}
			}
			else
			{
				val = this->RetriveT();
			}
		}

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableFloat4x4Array>(in_cbuff);
		}

	private:
		uint32_t size_ = 0;
	};

	class RenderVariableTexture final : public RenderVariableIOable
	{
	public:
		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableTexture>();
			TexturePtr val;
			this->Value(val);
			*ret = val;
			std::string elem_type;
			this->Value(elem_type);
			*ret = elem_type;
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			*this = TexturePtr();

			std::string elem_type;
			if (auto attr = node.Attrib("elem_type"))
			{
				elem_type = std::string(attr->ValueString());
			}
			else
			{
				elem_type = "float4";
			}

			std::string sample_count;
			if (auto attr = node.Attrib("sample_count"))
			{
				sample_count = std::string(attr->ValueString());
				*this = elem_type + ", " + sample_count;
			}
			else
			{
				*this = elem_type;
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			*this = TexturePtr();
			*this = ReadShortString(res);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::string tmp;
			this->Value(tmp);
			WriteShortString(os, tmp);
		}
#endif

		RenderVariable& operator=(TexturePtr const& value) override
		{
			if (value)
			{
				uint32_t array_size = value->ArraySize();
				uint32_t mipmap = value->NumMipMaps();

				auto& rf = Context::Instance().RenderFactoryInstance();
				val_ = rf.MakeTextureSrv(value, 0, array_size, 0, mipmap);
			}
			else
			{
				val_.reset();
			}
			return *this;
		}

		RenderVariable& operator=(ShaderResourceViewPtr const& value) override
		{
			val_ = value;
			return *this;
		}

		RenderVariable& operator=(std::string const& value) override
		{
			elem_type_ = value;
			return *this;
		}

		void Value(TexturePtr& val) const override
		{
			if (val_)
			{
				val = val_->TextureResource();
			}
			else
			{
				val.reset();
			}
		}

		void Value(ShaderResourceViewPtr& val) const override
		{
			val = val_;
		}

		void Value(std::string& val) const override
		{
			val = elem_type_;
		}

	protected:
		ShaderResourceViewPtr val_;
		std::string elem_type_;
	};

	class RenderVariableRwTexture final : public RenderVariableIOable
	{
	public:
		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableRwTexture>();
			UnorderedAccessViewPtr val;
			this->Value(val);
			*ret = val;
			std::string elem_type;
			this->Value(elem_type);
			*ret = elem_type;
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			*this = UnorderedAccessViewPtr();

			if (auto attr = node.Attrib("elem_type"))
			{
				*this = std::string(attr->ValueString());
			}
			else
			{
				*this = std::string("float4");
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			*this = UnorderedAccessViewPtr();
			*this = ReadShortString(res);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::string tmp;
			this->Value(tmp);
			WriteShortString(os, tmp);
		}
#endif

		RenderVariable& operator=(TexturePtr const& value) override
		{
			auto& rf = Context::Instance().RenderFactoryInstance();
			switch (value->Type())
			{
			case Texture::TT_1D:
				val_ = rf.Make1DUav(value, 0, static_cast<int>(value->ArraySize()), 0);
				break;

			case Texture::TT_2D:
				val_ = rf.Make2DUav(value, 0, static_cast<int>(value->ArraySize()), 0);
				break;

			case Texture::TT_3D:
				val_ = rf.Make3DUav(value, 0, 0, value->Depth(0), 0);
				break;

			case Texture::TT_Cube:
				val_ = rf.MakeCubeUav(value, 0, 0);
				break;
			}
			return *this;
		}

		RenderVariable& operator=(UnorderedAccessViewPtr const& value) override
		{
			val_ = value;
			return *this;
		}

		RenderVariable& operator=(std::string const& value) override
		{
			elem_type_ = value;
			return *this;
		}

		void Value(TexturePtr& val) const override
		{
			val = val_->TextureResource();
		}

		void Value(UnorderedAccessViewPtr& val) const override
		{
			val = val_;
		}

		void Value(std::string& val) const override
		{
			val = elem_type_;
		}

	protected:
		UnorderedAccessViewPtr val_;
		std::string elem_type_;
	};

	class RenderVariableBuffer final : public RenderVariableIOable
	{
	public:
		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableBuffer>();
			ShaderResourceViewPtr val;
			this->Value(val);
			*ret = val;
			std::string elem_type;
			this->Value(elem_type);
			*ret = elem_type;
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			*this = ShaderResourceViewPtr();

			if (auto attr = node.Attrib("elem_type"))
			{
				*this = std::string(attr->ValueString());
			}
			else
			{
				*this = std::string("float4");
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			*this = ShaderResourceViewPtr();
			*this = ReadShortString(res);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::string tmp;
			this->Value(tmp);
			WriteShortString(os, tmp);
		}
#endif

		RenderVariable& operator=(ShaderResourceViewPtr const& value) override
		{
			val_ = value;
			return *this;
		}

		RenderVariable& operator=(std::string const& value) override
		{
			elem_type_ = value;
			return *this;
		}

		void Value(ShaderResourceViewPtr& val) const override
		{
			val = val_;
		}

		void Value(std::string& val) const override
		{
			val = elem_type_;
		}

	protected:
		ShaderResourceViewPtr val_;
		std::string elem_type_;
	};

	class RenderVariableRwBuffer final : public RenderVariableIOable
	{
	public:
		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableRwBuffer>();
			UnorderedAccessViewPtr val;
			this->Value(val);
			*ret = val;
			std::string elem_type;
			this->Value(elem_type);
			*ret = elem_type;
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(array_size);

			*this = UnorderedAccessViewPtr();

			if (auto attr = node.Attrib("elem_type"))
			{
				*this = std::string(attr->ValueString());
			}
			else
			{
				*this = std::string("float4");
			}
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);

			*this = UnorderedAccessViewPtr();
			*this = ReadShortString(res);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			std::string tmp;
			this->Value(tmp);
			WriteShortString(os, tmp);
		}
#endif

		RenderVariable& operator=(UnorderedAccessViewPtr const& value) override
		{
			val_ = value;
			return *this;
		}

		RenderVariable& operator=(std::string const& value) override
		{
			elem_type_ = value;
			return *this;
		}

		void Value(UnorderedAccessViewPtr& val) const override
		{
			val = val_;
		}

		void Value(std::string& val) const override
		{
			val = elem_type_;
		}

	protected:
		UnorderedAccessViewPtr val_;
		std::string elem_type_;
	};

	class RenderVariableByteAddressBuffer final : public RenderVariableIOable
	{
	public:
		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableByteAddressBuffer>();
			ShaderResourceViewPtr val;
			this->Value(val);
			*ret = val;
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(node);
			KFL_UNUSED(array_size);
			*this = ShaderResourceViewPtr();
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(res);
			*this = ShaderResourceViewPtr();
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			KFL_UNUSED(os);
		}
#endif

		RenderVariable& operator=(ShaderResourceViewPtr const& value) override
		{
			val_ = value;
			return *this;
		}

		void Value(ShaderResourceViewPtr& val) const override
		{
			val = val_;
		}

	protected:
		ShaderResourceViewPtr val_;
	};

	class RenderVariableRwByteAddressBuffer final : public RenderVariableIOable
	{
	public:
		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableRwByteAddressBuffer>();
			UnorderedAccessViewPtr val;
			this->Value(val);
			*ret = val;
			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(node);
			KFL_UNUSED(array_size);
			*this = UnorderedAccessViewPtr();
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			KFL_UNUSED(effect);
			KFL_UNUSED(res);
			*this = UnorderedAccessViewPtr();
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			KFL_UNUSED(os);
		}
#endif

		RenderVariable& operator=(UnorderedAccessViewPtr const& value) override
		{
			val_ = value;
			return *this;
		}

		void Value(UnorderedAccessViewPtr& val) const override
		{
			val = val_;
		}

	protected:
		UnorderedAccessViewPtr val_;
	};

	class RenderVariableStruct final : public RenderVariableArray<uint8_t>
	{
	public:
		explicit RenderVariableStruct(bool in_cbuff = false)
			: RenderVariableArray<uint8_t>(in_cbuff)
		{
		}

		RenderEffectStructType* StructType() const override
		{
			return struct_type_;
		}

		std::unique_ptr<RenderVariable> Clone() override
		{
			auto ret = RenderVariableArray<uint8_t>::Clone();

			auto* struct_var = checked_cast<RenderVariableStruct*>(ret.get());
			struct_var->struct_type_ = struct_type_;

			return ret;
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, XMLNode const& node, uint32_t array_size) override
		{
			KFL_UNUSED(array_size);

			struct_type_ = effect.StructTypeByName(node.Attrib("type")->ValueString());
			BOOST_ASSERT(struct_type_);
		}
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res) override
		{
			struct_type_ = effect.StructTypeByName(ReadShortString(res));
			BOOST_ASSERT(struct_type_);
		}

#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const override
		{
			WriteShortString(os, struct_type_->Name());
		}
#endif

		using RenderVariableArray<uint8_t>::operator=;
		using RenderVariableArray<uint8_t>::Value;

	protected:
		std::unique_ptr<RenderVariable> MakeInstance(bool in_cbuff) override
		{
			return MakeUniquePtr<RenderVariableStruct>(in_cbuff);
		}

	private:
		RenderEffectStructType* struct_type_ = nullptr;
	};

	std::unique_ptr<RenderVariableIOable> RenderVariableFactory(RenderEffectDataType type, bool is_array)
	{
		std::unique_ptr<RenderVariableIOable> ret;
		switch (type)
		{
		case REDT_bool:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableBoolArray>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableBool>();
			}
			break;

		case REDT_uint:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableUIntArray>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableUInt>();
			}
			break;

		case REDT_int:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableIntArray>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableInt>();
			}
			break;

		case REDT_string:
			ret = MakeUniquePtr<RenderVariableString>();
			break;

		case REDT_texture1D:
		case REDT_texture2D:
		case REDT_texture2DMS:
		case REDT_texture3D:
		case REDT_textureCUBE:
		case REDT_texture1DArray:
		case REDT_texture2DArray:
		case REDT_texture2DMSArray:
		case REDT_texture3DArray:
		case REDT_textureCUBEArray:
			ret = MakeUniquePtr<RenderVariableTexture>();
			break;

		case REDT_rw_texture1D:
		case REDT_rw_texture2D:
		case REDT_rw_texture3D:
		case REDT_rw_texture1DArray:
		case REDT_rw_texture2DArray:
		case REDT_rasterizer_ordered_texture1D:
		case REDT_rasterizer_ordered_texture1DArray:
		case REDT_rasterizer_ordered_texture2D:
		case REDT_rasterizer_ordered_texture2DArray:
		case REDT_rasterizer_ordered_texture3D:
			ret = MakeUniquePtr<RenderVariableRwTexture>();
			break;

		case REDT_sampler:
			ret = MakeUniquePtr<RenderVariableSampler>();
			break;

		case REDT_shader:
			ret = MakeUniquePtr<RenderVariableShader>();
			break;

		case REDT_float:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableFloatArray>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableFloat>();
			}
			break;

		case REDT_uint2:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableInt2Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableUInt2>();
			}
			break;

		case REDT_uint3:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableInt3Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableUInt3>();
			}
			break;

		case REDT_uint4:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableInt4Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableUInt4>();
			}
			break;

		case REDT_int2:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableInt2Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableInt2>();
			}
			break;

		case REDT_int3:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableInt3Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableInt3>();
			}
			break;

		case REDT_int4:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableInt4Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableInt4>();
			}
			break;

		case REDT_float2:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableFloat2Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableFloat2>();
			}
			break;

		case REDT_float3:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableFloat3Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableFloat3>();
			}
			break;

		case REDT_float4:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableFloat4Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableFloat4>();
			}
			break;

		case REDT_float4x4:
			if (is_array)
			{
				ret = MakeUniquePtr<RenderVariableFloat4x4Array>();
			}
			else
			{
				ret = MakeUniquePtr<RenderVariableFloat4x4>();
			}
			break;

		case REDT_buffer:
		case REDT_structured_buffer:
		case REDT_consume_structured_buffer:
		case REDT_append_structured_buffer:
			ret = MakeUniquePtr<RenderVariableBuffer>();
			break;

		case REDT_rw_buffer:
		case REDT_rw_structured_buffer:
		case REDT_rasterizer_ordered_buffer:
		case REDT_rasterizer_ordered_structured_buffer:
			ret = MakeUniquePtr<RenderVariableRwBuffer>();
			break;

		case REDT_byte_address_buffer:
			ret = MakeUniquePtr<RenderVariableByteAddressBuffer>();
			break;

		case REDT_rw_byte_address_buffer:
		case REDT_rasterizer_ordered_byte_address_buffer:
			ret = MakeUniquePtr<RenderVariableRwByteAddressBuffer>();
			break;

		case REDT_struct:
			ret = MakeUniquePtr<RenderVariableStruct>();
			break;

		default:
			KFL_UNREACHABLE("Invalid type");
		}

		return ret;
	}

#if KLAYGE_IS_DEV_PLATFORM
	std::unique_ptr<RenderVariable> LoadVariable(
		RenderEffect const& effect, XMLNode const& node, RenderEffectDataType type, uint32_t array_size)
	{
		auto ret = RenderVariableFactory(type, array_size != 0);
		ret->Load(effect, node, array_size);
		return ret;
	}
#endif

	std::unique_ptr<RenderVariable> StreamInVariable(
		RenderEffect const& effect, ResIdentifier& res, RenderEffectDataType type, uint32_t array_size)
	{
		auto ret = RenderVariableFactory(type, array_size != 0);
		ret->StreamIn(effect, res);
		return ret;
	}

#if KLAYGE_IS_DEV_PLATFORM
	void StreamOutVariable(std::ostream& os, RenderVariable const& var)
	{
		checked_cast<RenderVariableIOable const&>(var).StreamOut(os);
	}
#endif
}

namespace KlayGE
{
	class EffectLoadingDesc : public ResLoadingDesc
	{
	private:
		struct EffectDesc
		{
			std::vector<std::string> res_name;

			bool cloned = false;
			RenderEffectPtr effect;
		};

	public:
		explicit EffectLoadingDesc(std::span<std::string const> name)
		{
			effect_desc_.res_name = std::vector<std::string>(name.begin(), name.end());
			effect_desc_.effect = MakeSharedPtr<RenderEffect>();
		}

		uint64_t Type() const override
		{
			static uint64_t const type = CT_HASH("EffectLoadingDesc");
			return type;
		}

		bool StateLess() const override
		{
			return false;
		}

		std::shared_ptr<void> CreateResource() override
		{
			effect_desc_.effect->Load(effect_desc_.res_name);
			return effect_desc_.effect;
		}

		void SubThreadStage() override
		{
			std::lock_guard<std::mutex> lock(main_thread_stage_mutex_);

			RenderEffectPtr const& effect = effect_desc_.effect;
			if (effect && effect->HWResourceReady())
			{
				return;
			}

#if KLAYGE_IS_DEV_PLATFORM
			effect->CompileShaders();
#endif

			RenderFactory& rf = Context::Instance().RenderFactoryInstance();
			RenderDeviceCaps const & caps = rf.RenderEngineInstance().DeviceCaps();
			if (caps.multithread_res_creating_support)
			{
				this->MainThreadStageNoLock();
			}
		}

		void MainThreadStage() override
		{
			std::lock_guard<std::mutex> lock(main_thread_stage_mutex_);
			this->MainThreadStageNoLock();
		}

		bool HasSubThreadStage() const override
		{
			return true;
		}

		bool Match(ResLoadingDesc const & rhs) const override
		{
			if (this->Type() == rhs.Type())
			{
				EffectLoadingDesc const & eld = static_cast<EffectLoadingDesc const &>(rhs);
				return (effect_desc_.res_name == eld.effect_desc_.res_name);
			}
			return false;
		}

		void CopyDataFrom(ResLoadingDesc const & rhs) override
		{
			BOOST_ASSERT(this->Type() == rhs.Type());

			EffectLoadingDesc const & eld = static_cast<EffectLoadingDesc const &>(rhs);
			effect_desc_.res_name = eld.effect_desc_.res_name;
			effect_desc_.effect = eld.effect_desc_.effect->Clone();
			effect_desc_.cloned = true;
		}

		std::shared_ptr<void> CloneResourceFrom(std::shared_ptr<void> const & resource) override
		{
			auto rhs_effect = std::static_pointer_cast<RenderEffect>(resource);
			if (effect_desc_.cloned)
			{
				rhs_effect->Reclone(*effect_desc_.effect);
			}
			else
			{
				rhs_effect->CloneInPlace(*effect_desc_.effect);
			}
			return std::static_pointer_cast<void>(effect_desc_.effect);
		}

		std::shared_ptr<void> Resource() const override
		{
			return effect_desc_.effect;
		}

	private:
		void MainThreadStageNoLock()
		{
			RenderEffectPtr const& effect = effect_desc_.effect;
			if (!effect || !effect->HWResourceReady())
			{
				effect->CreateHwShaders();
			}
		}

	private:
		EffectDesc effect_desc_;
		std::mutex main_thread_stage_mutex_;
	};


#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectAnnotation::Load(RenderEffect const& effect, XMLNode const& node)
	{
		type_ = TypeFromName(node.Attrib("type")->ValueString());
		name_ = std::string(node.Attrib("name")->ValueString());
		var_ = LoadVariable(effect, node, type_, 0);
	}
#endif

	void RenderEffectAnnotation::StreamIn(RenderEffect const& effect, ResIdentifier& res)
	{
		res.read(&type_, sizeof(type_));
		type_ = LE2Native(type_);
		name_ = ReadShortString(res);
		var_ = StreamInVariable(effect, res, type_, 0);
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectAnnotation::StreamOut(std::ostream& os) const
	{
		uint32_t t = Native2LE(type_);
		os.write(reinterpret_cast<char const *>(&t), sizeof(t));
		WriteShortString(os, name_);
		StreamOutVariable(os, *var_);
	}
#endif


#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectStructType::Load(RenderEffect const& effect, XMLNode const& node)
	{
		name_ = std::string(node.Attrib("name")->ValueString());
		name_hash_ = HashRange(name_.begin(), name_.end());

		for (XMLNodePtr member_node = node.FirstNode("member"); member_node; member_node = member_node->NextSibling("member"))
		{
			RenderEffectDataType member_type;
			auto member_type_name = member_node->Attrib("type")->ValueString();
			if (effect.StructTypeByName(member_type_name) != nullptr)
			{
				member_type = REDT_struct;
			}
			else
			{
				member_type = TypeFromName(member_type_name);
				member_type_name = "";
			}

			auto member_name = member_node->Attrib("name")->ValueString();

			std::string member_array_size;
			if (auto attr = member_node->Attrib("array_size"))
			{
				member_array_size = std::string(attr->ValueString());
			}
			
			members_.push_back(std::make_tuple(
				member_type, std::string(member_type_name), std::string(member_name), MakeSharedPtr<std::string>(member_array_size)));
		}
	}
#endif

	void RenderEffectStructType::StreamIn(ResIdentifier& res)
	{
		name_ = ReadShortString(res);
		name_hash_ = HashRange(name_.begin(), name_.end());

		uint8_t len;
		res.read(reinterpret_cast<char*>(&len), sizeof(len));

		members_.resize(len);
		for (uint32_t i = 0; i < len; ++i)
		{
			uint8_t type;
			res.read(reinterpret_cast<char*>(&type), sizeof(type));
			std::get<0>(members_[i]) = static_cast<RenderEffectDataType>(type);

			if (type == REDT_struct)
			{
				std::get<1>(members_[i]) = ReadShortString(res);
			}

			std::get<2>(members_[i]) = ReadShortString(res);
			std::get<3>(members_[i]) = MakeSharedPtr<std::string>(ReadShortString(res));
		}
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectStructType::StreamOut(std::ostream& os) const
	{
		WriteShortString(os, name_);

		uint8_t len = static_cast<uint8_t>(members_.size());
		os.write(reinterpret_cast<char const*>(&len), sizeof(len));

		for (uint32_t i = 0; i < len; ++i)
		{
			uint8_t const type = static_cast<uint8_t>(std::get<0>(members_[i]));
			os.write(reinterpret_cast<char const*>(&type), sizeof(type));

			if (type == REDT_struct)
			{
				WriteShortString(os, std::get<1>(members_[i]));
			}

			WriteShortString(os, std::get<2>(members_[i]));
			WriteShortString(os, *std::get<3>(members_[i]));
		}
	}
#endif

	uint32_t RenderEffectStructType::NumMembers() const
	{
		return static_cast<uint8_t>(members_.size());
	}

	RenderEffectDataType RenderEffectStructType::MemberType(uint32_t index) const
	{
		BOOST_ASSERT(index < this->NumMembers());
		return std::get<0>(members_[index]);
	}

	std::string const& RenderEffectStructType::MemberTypeName(uint32_t index) const
	{
		BOOST_ASSERT(index < this->NumMembers());
		return std::get<1>(members_[index]);
	}

	std::string const& RenderEffectStructType::MemberName(uint32_t index) const
	{
		BOOST_ASSERT(index < this->NumMembers());
		return std::get<2>(members_[index]);
	}

	std::shared_ptr<std::string> const& RenderEffectStructType::MemberArraySize(uint32_t index) const
	{
		BOOST_ASSERT(index < this->NumMembers());
		return std::get<3>(members_[index]);
	}


	void RenderEffect::Load(std::span<std::string const> names)
	{
		effect_template_ = MakeSharedPtr<RenderEffectTemplate>();
		effect_template_->Load(names, *this);
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffect::CompileShaders()
	{
		effect_template_->CompileShaders(*this);
	}
#endif

	void RenderEffect::CreateHwShaders()
	{
		effect_template_->CreateHwShaders(*this);
	}

	RenderEffectPtr RenderEffect::Clone()
	{
		RenderEffectPtr ret = MakeSharedPtr<RenderEffect>();
		this->CloneInPlace(*ret);
		return ret;
	}

	void RenderEffect::CloneInPlace(RenderEffect& dst_effect)
	{
		dst_effect.effect_template_ = effect_template_;

		dst_effect.params_.resize(params_.size());
		for (size_t i = 0; i < params_.size(); ++i)
		{
			dst_effect.params_[i] = params_[i]->Clone();
		}

		dst_effect.cbuffers_.resize(cbuffers_.size());
		for (size_t i = 0; i < cbuffers_.size(); ++i)
		{
			dst_effect.cbuffers_[i] = cbuffers_[i]->Clone(dst_effect);
		}

		dst_effect.shader_objs_.resize(shader_objs_.size());
		for (size_t i = 0; i < shader_objs_.size(); ++i)
		{
			if (shader_objs_[i]->HWResourceReady())
			{
				dst_effect.shader_objs_[i] = shader_objs_[i]->Clone(dst_effect);
			}
		}
	}

	void RenderEffect::Reclone(RenderEffect& dst_effect)
	{
		for (size_t i = 0; i < cbuffers_.size(); ++i)
		{
			cbuffers_[i]->Reclone(*dst_effect.cbuffers_[i], dst_effect);
		}

		for (size_t i = 0; i < shader_objs_.size(); ++i)
		{
			if (shader_objs_[i]->HWResourceReady())
			{
				dst_effect.shader_objs_[i] = shader_objs_[i]->Clone(dst_effect);
			}
		}
	}

	bool RenderEffect::HWResourceReady() const
	{
		if (!hw_res_ready_)
		{
			hw_res_ready_ = true;
			for (uint32_t i = 0; i < effect_template_->NumTechniques(); ++i)
			{
				hw_res_ready_ &= effect_template_->TechniqueByIndex(i)->HWResourceReady(*this);
			}
		}

		return hw_res_ready_;
	}

	std::string const & RenderEffect::ResName() const
	{
		return effect_template_->ResName();
	}

	size_t RenderEffect::ResNameHash() const
	{
		return effect_template_->ResNameHash();
	}

	RenderEffectParameter* RenderEffect::ParameterByName(std::string_view name) const
	{
		size_t const name_hash = HashRange(name.begin(), name.end());
		for (auto const & param : params_)
		{
			if (name_hash == param->NameHash())
			{
				return param.get();
			}
		}
		return nullptr;
	}

	RenderEffectParameter* RenderEffect::ParameterBySemantic(std::string_view semantic) const
	{
		size_t const semantic_hash = HashRange(semantic.begin(), semantic.end());
		for (auto const & param : params_)
		{
			if (semantic_hash == param->SemanticHash())
			{
				return param.get();
			}
		}
		return nullptr;
	}

	RenderEffectConstantBuffer* RenderEffect::CBufferByName(std::string_view name) const
	{
		uint32_t index = this->FindCBuffer(name);
		if (index != static_cast<uint32_t>(-1))
		{
			return this->CBufferByIndex(index);
		}
		else
		{
			return nullptr;
		}
	}

	uint32_t RenderEffect::FindCBuffer(std::string_view name) const
	{
		size_t const name_hash = HashRange(name.begin(), name.end());
		for (uint32_t i = 0; i < cbuffers_.size(); ++i)
		{
			if (name_hash == cbuffers_[i]->NameHash())
			{
				return i;
			}
		}
		return static_cast<uint32_t>(-1);
	}

	void RenderEffect::BindCBufferByName(std::string_view name, RenderEffectConstantBufferPtr const& cbuff)
	{
		size_t const name_hash = HashRange(name.begin(), name.end());
		for (auto& cbuffer : cbuffers_)
		{
			if (name_hash == cbuffer->NameHash())
			{
				cbuffer = cbuff;
			}
		}
	}

	uint32_t RenderEffect::NumStructTypes() const
	{
		return effect_template_->NumStructTypes();
	}

	RenderEffectStructType* RenderEffect::StructTypeByName(std::string_view name) const
	{
		return effect_template_->StructTypeByName(name);
	}

	RenderEffectStructType* RenderEffect::StructTypeByIndex(uint32_t index) const
	{
		return effect_template_->StructTypeByIndex(index);
	}

	uint32_t RenderEffect::NumTechniques() const
	{
		return effect_template_->NumTechniques();
	}

	RenderTechnique* RenderEffect::TechniqueByName(std::string_view name) const
	{
		return effect_template_->TechniqueByName(name);
	}

	RenderTechnique* RenderEffect::TechniqueByIndex(uint32_t n) const
	{
		return effect_template_->TechniqueByIndex(n);
	}

	uint32_t RenderEffect::NumShaderFragments() const
	{
		return effect_template_->NumShaderFragments();
	}

	RenderShaderFragment const & RenderEffect::ShaderFragmentByIndex(uint32_t n) const
	{
		return effect_template_->ShaderFragmentByIndex(n);
	}

	uint32_t RenderEffect::AddShaderDesc(ShaderDesc const & sd)
	{
		return effect_template_->AddShaderDesc(sd);
	}

	ShaderDesc& RenderEffect::GetShaderDesc(uint32_t id)
	{
		return effect_template_->GetShaderDesc(id);
	}

	ShaderDesc const & RenderEffect::GetShaderDesc(uint32_t id) const
	{
		return effect_template_->GetShaderDesc(id);
	}

	uint32_t RenderEffect::NumMacros() const
	{
		return effect_template_->NumMacros();
	}

	std::pair<std::string, std::string> const & RenderEffect::MacroByIndex(uint32_t n) const
	{
		return effect_template_->MacroByIndex(n);
	}

	uint32_t RenderEffect::AddShaderObject()
	{
		uint32_t index = static_cast<uint32_t>(shader_objs_.size());
		shader_objs_.push_back(Context::Instance().RenderFactoryInstance().MakeShaderObject());
		return index;
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffect::GenHLSLShaderText()
	{
		effect_template_->GenHLSLShaderText(*this);
	}

	std::string const & RenderEffect::HLSLShaderText() const
	{
		return effect_template_->HLSLShaderText();
	}
#endif


#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectTemplate::PreprocessIncludes(XMLDocument& doc, XMLNode& root, std::vector<std::unique_ptr<XMLDocument>>& include_docs)
	{
		std::vector<std::string> whole_include_names;
		for (XMLNodePtr node = root.FirstNode("include"); node;)
		{
			XMLAttributePtr attr = node->Attrib("name");
			BOOST_ASSERT(attr);

			std::string const include_name = std::string(attr->ValueString());

			include_docs.push_back(MakeUniquePtr<XMLDocument>());
			XMLNodePtr include_root = include_docs.back()->Parse(*ResLoader::Instance().Open(include_name));

			std::vector<std::string> include_names;
			this->RecursiveIncludeNode(*include_root, include_names);

			if (!include_names.empty())
			{
				for (auto iter = include_names.begin(); iter != include_names.end();)
				{
					bool found = false;
					for (auto iter_w = whole_include_names.begin(); iter_w != whole_include_names.end(); ++ iter_w)
					{
						if (*iter == *iter_w)
						{
							found = true;
							break;
						}
					}

					if (found)
					{
						iter = include_names.erase(iter);
					}
					else
					{
						include_docs.push_back(MakeUniquePtr<XMLDocument>());
						XMLNodePtr recursive_include_root = include_docs.back()->Parse(*ResLoader::Instance().Open(*iter));
						this->InsertIncludeNodes(doc, root, *node, *recursive_include_root);

						whole_include_names.push_back(*iter);
						++ iter;
					}
				}
			}

			bool found = false;
			for (auto iter_w = whole_include_names.begin(); iter_w != whole_include_names.end(); ++ iter_w)
			{
				if (include_name == *iter_w)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				this->InsertIncludeNodes(doc, root, *node, *include_root);
				whole_include_names.push_back(include_name);
			}

			XMLNodePtr node_next = node->NextSibling("include");
			root.RemoveNode(*node);
			node = node_next;
		}
	}

	void RenderEffectTemplate::RecursiveIncludeNode(XMLNode const & root, std::vector<std::string>& include_names) const
	{
		for (XMLNodePtr node = root.FirstNode("include"); node; node = node->NextSibling("include"))
		{
			XMLAttributePtr attr = node->Attrib("name");
			BOOST_ASSERT(attr);

			std::string const include_name = std::string(attr->ValueString());

			XMLDocument include_doc;
			XMLNodePtr include_root = include_doc.Parse(*ResLoader::Instance().Open(include_name));
			this->RecursiveIncludeNode(*include_root, include_names);

			bool found = false;
			for (size_t i = 0; i < include_names.size(); ++ i)
			{
				if (include_name == include_names[i])
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				include_names.push_back(include_name);
			}
		}
	}

	void RenderEffectTemplate::InsertIncludeNodes(
		XMLDocument& target_doc, XMLNode& target_root, XMLNode const& target_place, XMLNode const& include_root) const
	{
		for (XMLNodePtr child_node = include_root.FirstNode(); child_node; child_node = child_node->NextSibling())
		{
			if ((XNT_Element == child_node->Type()) && (child_node->Name() != "include"))
			{
				target_root.InsertNode(target_place, target_doc.CloneNode(*child_node));
			}
		}
	}

	XMLNodePtr RenderEffectTemplate::ResolveInheritTechNode(XMLDocument& doc, XMLNode& root, XMLNodePtr const & tech_node)
	{
		auto inherit_attr = tech_node->Attrib("inherit");
		if (!inherit_attr)
		{
			return tech_node;
		}

		auto const tech_name = tech_node->Attrib("name")->ValueString();

		auto const inherit_name = inherit_attr->ValueString();
		BOOST_ASSERT(inherit_name != tech_name);

		XMLNodePtr new_tech_node;
		for (auto node = root.FirstNode("technique"); node; node = node->NextSibling("technique"))
		{
			auto const parent_tech_name = node->Attrib("name")->ValueString();
			if (parent_tech_name == inherit_name)
			{
				auto parent_node = this->ResolveInheritTechNode(doc, root, node);
				new_tech_node = doc.CloneNode(*parent_node);

				for (auto tech_anno_node = tech_node->FirstNode("annotation"); tech_anno_node;
					tech_anno_node = tech_anno_node->NextSibling("annotation"))
				{
					new_tech_node->AppendNode(doc.CloneNode(*tech_anno_node));
				}
				for (auto tech_macro_node = tech_node->FirstNode("macro"); tech_macro_node;
					tech_macro_node = tech_macro_node->NextSibling("macro"))
				{
					new_tech_node->AppendNode(doc.CloneNode(*tech_macro_node));
				}
				for (auto pass_node = tech_node->FirstNode("pass"); pass_node; pass_node = pass_node->NextSibling("pass"))
				{
					auto const pass_name = pass_node->Attrib("name")->ValueString();

					bool found_pass = false;
					for (auto new_pass_node = new_tech_node->FirstNode("pass"); new_pass_node;
						new_pass_node = new_pass_node->NextSibling("pass"))
					{
						auto const parent_pass_name = new_pass_node->Attrib("name")->ValueString();

						if (pass_name == parent_pass_name)
						{
							for (auto pass_anno_node = pass_node->FirstNode("annotation"); pass_anno_node;
								pass_anno_node = pass_anno_node->NextSibling("annotation"))
							{
								new_pass_node->AppendNode(doc.CloneNode(*pass_anno_node));
							}
							for (auto pass_macro_node = pass_node->FirstNode("macro"); pass_macro_node;
								pass_macro_node = pass_macro_node->NextSibling("macro"))
							{
								new_pass_node->AppendNode(doc.CloneNode(*pass_macro_node));
							}
							for (auto pass_state_node = pass_node->FirstNode("state"); pass_state_node;
								pass_state_node = pass_state_node->NextSibling("state"))
							{
								new_pass_node->AppendNode(doc.CloneNode(*pass_state_node));
							}

							found_pass = true;
							break;
						}
					}

					if (!found_pass)
					{
						new_tech_node->AppendNode(doc.CloneNode(*pass_node));
					}
				}

				new_tech_node->RemoveAttrib(*new_tech_node->Attrib("name"));
				new_tech_node->AppendAttrib(doc.AllocAttribString("name", tech_name));

				break;
			}
		}
		BOOST_ASSERT(new_tech_node);

		return new_tech_node;
	}

	void RenderEffectTemplate::ResolveOverrideTechs(XMLDocument& doc, XMLNode& root)
	{
		std::vector<XMLNodePtr> tech_nodes;
		for (XMLNodePtr node = root.FirstNode("technique"); node; node = node->NextSibling("technique"))
		{
			tech_nodes.push_back(node);
		}

		for (auto const & node : tech_nodes)
		{
			auto override_attr = node->Attrib("override");
			if (override_attr)
			{
				auto override_tech_name = override_attr->ValueString();
				for (auto& overrided_node : tech_nodes)
				{
					auto name = overrided_node->Attrib("name")->ValueString();
					if (override_tech_name == name)
					{
						auto new_node = doc.CloneNode(*this->ResolveInheritTechNode(doc, root, node));
						new_node->RemoveAttrib(*new_node->Attrib("name"));
						new_node->AppendAttrib(doc.AllocAttribString("name", name));
						auto attr = new_node->Attrib("override");
						if (attr)
						{
							new_node->RemoveAttrib(*attr);
						}

						root.InsertNode(*overrided_node, new_node);
						root.RemoveNode(*overrided_node);
						overrided_node = new_node;

						break;
					}
				}
			}
		}
	}

	void RenderEffectTemplate::Load(XMLNode const & root, RenderEffect& effect)
	{
		{
			XMLNodePtr macro_node = root.FirstNode("macro");
			for (; macro_node; macro_node = macro_node->NextSibling("macro"))
			{
				macros_.emplace_back(std::make_pair(macro_node->Attrib("name")->ValueString(), macro_node->Attrib("value")->ValueString()), true);
			}
		}

		for (XMLNodePtr node = root.FirstNode("struct"); node; node = node->NextSibling("struct"))
		{
			struct_types_.push_back(MakeUniquePtr<RenderEffectStructType>());
			struct_types_.back()->Load(effect, *node);
		}

		std::vector<XMLNodePtr> parameter_nodes;
		for (XMLNodePtr node = root.FirstNode(); node; node = node->NextSibling())
		{
			if ("parameter" == node->Name())
			{
				parameter_nodes.push_back(node);
			}
			else if ("cbuffer" == node->Name())
			{
				for (XMLNodePtr sub_node = node->FirstNode("parameter"); sub_node; sub_node = sub_node->NextSibling("parameter"))
				{
					parameter_nodes.push_back(sub_node);
				}
			}
		}

		for (uint32_t param_index = 0; param_index < parameter_nodes.size(); ++ param_index)
		{
			XMLNode const& node = *parameter_nodes[param_index];

			RenderEffectDataType type = REDT_count;
			auto type_name = node.Attrib("type")->ValueString();
			for (auto const& struct_type : struct_types_)
			{
				if (type_name == struct_type->Name())
				{
					type = REDT_struct;
					break;
				}
			}
			if (type == REDT_count)
			{
				type = TypeFromName(type_name);
			}
			if ((type != REDT_sampler)
				&& (type != REDT_texture1D) && (type != REDT_texture2D) && (type != REDT_texture2DMS) && (type != REDT_texture3D)
				&& (type != REDT_textureCUBE)
				&& (type != REDT_texture1DArray) && (type != REDT_texture2DArray) && (type != REDT_texture2DMSArray)
				&& (type != REDT_texture3DArray) && (type != REDT_textureCUBEArray)
				&& (type != REDT_buffer) && (type != REDT_structured_buffer)
				&& (type != REDT_byte_address_buffer) && (type != REDT_rw_buffer)
				&& (type != REDT_rw_structured_buffer) && (type != REDT_rw_texture1D)
				&& (type != REDT_rw_texture2D) && (type != REDT_rw_texture3D)
				&& (type != REDT_rw_texture1DArray) && (type != REDT_rw_texture2DArray)
				&& (type != REDT_rw_byte_address_buffer) && (type != REDT_append_structured_buffer)
				&& (type != REDT_consume_structured_buffer)
				&& (type != REDT_rasterizer_ordered_buffer) && (type != REDT_rasterizer_ordered_byte_address_buffer)
				&& (type != REDT_rasterizer_ordered_structured_buffer)
				&& (type != REDT_rasterizer_ordered_texture1D) && (type != REDT_rasterizer_ordered_texture1DArray)
				&& (type != REDT_rasterizer_ordered_texture2D) && (type != REDT_rasterizer_ordered_texture2DArray)
				&& (type != REDT_rasterizer_ordered_texture3D))
			{
				RenderEffectConstantBuffer* cbuff = nullptr;
				XMLNodePtr parent_node = node.Parent();
				std::string const cbuff_name = std::string(parent_node->AttribString("name", "global_cb"));
				size_t const cbuff_name_hash = RT_HASH(cbuff_name.c_str());

				bool found = false;
				for (size_t i = 0; i < effect.cbuffers_.size(); ++ i)
				{
					if (effect.cbuffers_[i]->NameHash() == cbuff_name_hash)
					{
						cbuff = effect.cbuffers_[i].get();
						found = true;
						break;
					}
				}
				if (!found)
				{
					effect.cbuffers_.push_back(MakeSharedPtr<RenderEffectConstantBuffer>(effect));
					cbuff = effect.cbuffers_.back().get();
					cbuff->Load(cbuff_name);
				}
				BOOST_ASSERT(cbuff);

				cbuff->AddParameter(param_index);
			}

			effect.params_.push_back(MakeUniquePtr<RenderEffectParameter>());
			effect.params_.back()->Load(effect, node);
		}

		for (XMLNodePtr shader_graph_nodes_node = root.FirstNode("shader_graph_nodes"); shader_graph_nodes_node;
			shader_graph_nodes_node = shader_graph_nodes_node->NextSibling("shader_graph_nodes"))
		{
			for (XMLNodePtr shader_node = shader_graph_nodes_node->FirstNode("node"); shader_node;
				shader_node = shader_node->NextSibling("node"))
			{
				auto name_attr = shader_node->Attrib("name");
				BOOST_ASSERT(name_attr);

				auto const & node_name = name_attr->ValueString();
				size_t const node_name_hash = HashRange(node_name.begin(), node_name.end());
				bool found = false;
				for (auto& gn : shader_graph_nodes_)
				{
					if (node_name_hash == gn.NameHash())
					{
						gn.Load(*shader_node);
						found = true;
						break;
					}
				}

				if (!found)
				{
					shader_graph_nodes_.push_back(RenderShaderGraphNode());
					shader_graph_nodes_.back().Load(*shader_node);
				}
			}
		}

		for (XMLNodePtr shader_node = root.FirstNode("shader"); shader_node; shader_node = shader_node->NextSibling("shader"))
		{
			shader_frags_.push_back(RenderShaderFragment());
			shader_frags_.back().Load(*shader_node);
		}

		this->GenHLSLShaderText(effect);

		uint32_t index = 0;
		for (XMLNodePtr node = root.FirstNode("technique"); node; node = node->NextSibling("technique"), ++ index)
		{
			techniques_.push_back(MakeUniquePtr<RenderTechnique>());
			techniques_.back()->Load(effect, *node, index);
		}
	}
#endif

	void RenderEffectTemplate::Load(std::span<std::string const> names, RenderEffect& effect)
	{
		std::filesystem::path first_fxml_path(ResLoader::Instance().Locate(*names.begin()));
		std::filesystem::path first_fxml_directory = first_fxml_path.parent_path();

		std::string connected_name;
		for (int i = 0; i < names.size(); ++ i)
		{
			connected_name += std::filesystem::path(names[i]).stem().string();
			if (i != names.size() - 1)
			{
				connected_name += '+';
			}
		}

		std::string kfx_name = ResLoader::Instance().Locate(connected_name + ".kfx");
		if (kfx_name.empty())
		{
			kfx_name = (first_fxml_directory / (connected_name + ".kfx")).string();
		}

		res_name_ = (first_fxml_directory / (connected_name + ".fxml")).string();
		res_name_hash_ = HashRange(res_name_.begin(), res_name_.end());
#if KLAYGE_IS_DEV_PLATFORM
		for (auto const & name : names)
		{
			timestamp_ = 0;

			ResIdentifierPtr source = ResLoader::Instance().Open(name);
			if (source)
			{
				timestamp_ = std::max(timestamp_, source->Timestamp());

				std::unique_ptr<XMLDocument> doc = MakeUniquePtr<XMLDocument>();
				XMLNodePtr root = doc->Parse(*source);

				std::vector<std::string> include_names;
				this->RecursiveIncludeNode(*root, include_names);

				for (auto const & include_name : include_names)
				{
					timestamp_ = std::max(timestamp_, ResLoader::Instance().Timestamp(include_name));
				}
			}
		}
#endif

#if KLAYGE_IS_DEV_PLATFORM
		need_compile_ = false;
#endif
		ResIdentifierPtr kfx_source = ResLoader::Instance().Open(kfx_name);
		if (!kfx_source || !this->StreamIn(*kfx_source, effect))
		{
#if KLAYGE_IS_DEV_PLATFORM
			effect.params_.clear();
			effect.cbuffers_.clear();
			effect.shader_objs_.clear();

			macros_.clear();
			struct_types_.clear();
			shader_frags_.clear();
			hlsl_shader_.clear();
			techniques_.clear();
			shader_graph_nodes_.clear();

			shader_descs_.resize(1);

			std::vector<std::unique_ptr<XMLDocument>> include_docs;
			std::vector<std::unique_ptr<XMLDocument>> frag_docs(names.size());

			ResIdentifierPtr main_source = ResLoader::Instance().Open(names[0]);
			if (main_source)
			{
				frag_docs[0] = MakeUniquePtr<XMLDocument>();
				XMLNodePtr root = frag_docs[0]->Parse(*main_source);
				this->PreprocessIncludes(*frag_docs[0], *root, include_docs);

				for (int i = 1; i < names.size(); ++ i)
				{
					ResIdentifierPtr source = ResLoader::Instance().Open(names[i]);
					if (source)
					{
						frag_docs[i] = MakeUniquePtr<XMLDocument>();
						XMLNodePtr frag_root = frag_docs[i]->Parse(*source);

						this->PreprocessIncludes(*frag_docs[i], *frag_root, include_docs);

						for (auto frag_node = frag_root->FirstNode(); frag_node; frag_node = frag_node->NextSibling())
						{
							root->AppendNode(frag_docs[i]->CloneNode(*frag_node));
						}
					}
				}

				this->ResolveOverrideTechs(*frag_docs[0], *root);

				this->Load(*root, effect);

				kfx_name_ = kfx_name;
				need_compile_ = true;
			}
#endif
		}
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectTemplate::CompileShaders(RenderEffect& effect)
	{
		if (need_compile_)
		{
			uint32_t tech_index = 0;
			for (auto const& tech : techniques_)
			{
				tech->CompileShaders(effect, tech_index);
				++tech_index;
			}

			std::ofstream ofs(kfx_name_.c_str(), std::ios_base::binary | std::ios_base::out);
			this->StreamOut(ofs, effect);
		}
	}
#endif

	void RenderEffectTemplate::CreateHwShaders(RenderEffect& effect)
	{
		uint32_t tech_index = 0;
		for (auto const& tech : techniques_)
		{
			tech->CreateHwShaders(effect, tech_index);
			++tech_index;
		}
	}

	bool RenderEffectTemplate::StreamIn(ResIdentifier& source, RenderEffect& effect)
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();

		bool ret = false;
		
		uint32_t fourcc;
		source.read(&fourcc, sizeof(fourcc));
		fourcc = LE2Native(fourcc);
			
		uint32_t ver;
		source.read(&ver, sizeof(ver));
		ver = LE2Native(ver);
			
		if ((MakeFourCC<'K', 'F', 'X', ' '>::value == fourcc) && (KFX_VERSION == ver))
		{
			uint32_t shader_fourcc;
			source.read(&shader_fourcc, sizeof(shader_fourcc));
			shader_fourcc = LE2Native(shader_fourcc);

			uint32_t shader_ver;
			source.read(&shader_ver, sizeof(shader_ver));
			shader_ver = LE2Native(shader_ver);

			uint8_t shader_platform_name_len;
			source.read(&shader_platform_name_len, sizeof(shader_platform_name_len));
			std::string shader_platform_name(shader_platform_name_len, 0);
			source.read(&shader_platform_name[0], shader_platform_name_len);

			if ((re.NativeShaderFourCC() == shader_fourcc) && (re.NativeShaderVersion() == shader_ver)
				&& (re.NativeShaderPlatformName() == shader_platform_name))
			{
				uint64_t timestamp;
				source.read(&timestamp, sizeof(timestamp));
#if KLAYGE_IS_DEV_PLATFORM
				timestamp = LE2Native(timestamp);
				if (timestamp_ <= timestamp)
#endif
				{
					shader_descs_.resize(1);

					{
						uint16_t num_macros;
						source.read(&num_macros, sizeof(num_macros));
						num_macros = LE2Native(num_macros);

						macros_.resize(num_macros);
						for (uint32_t i = 0; i < num_macros; ++ i)
						{
							std::string name = ReadShortString(source);
							std::string value = ReadShortString(source);
							macros_[i] = std::make_pair(std::make_pair(name, value), true);
						}
					}

					{
						uint16_t num_structs;
						source.read(&num_structs, sizeof(num_structs));
						num_structs = LE2Native(num_structs);

						struct_types_.resize(num_structs);
						for (uint32_t i = 0; i < num_structs; ++i)
						{
							struct_types_[i] = MakeUniquePtr<RenderEffectStructType>();
							struct_types_[i]->StreamIn(source);
						}
					}

					{
						uint16_t num_cbufs;
						source.read(&num_cbufs, sizeof(num_cbufs));
						num_cbufs = LE2Native(num_cbufs);
						effect.cbuffers_.resize(num_cbufs);
						for (uint32_t i = 0; i < num_cbufs; ++ i)
						{
							effect.cbuffers_[i] = MakeSharedPtr<RenderEffectConstantBuffer>(effect);
							effect.cbuffers_[i]->StreamIn(source);
						}
					}

					{
						uint16_t num_params;
						source.read(&num_params, sizeof(num_params));
						num_params = LE2Native(num_params);
						effect.params_.resize(num_params);
						for (uint32_t i = 0; i < num_params; ++ i)
						{
							effect.params_[i] = MakeUniquePtr<RenderEffectParameter>();
							effect.params_[i]->StreamIn(effect, source);
						}
					}

					{
						uint8_t num_shader_graph_nodes;
						source.read(&num_shader_graph_nodes, sizeof(num_shader_graph_nodes));
						shader_graph_nodes_.resize(num_shader_graph_nodes);
						for (uint32_t i = 0; i < num_shader_graph_nodes; ++ i)
						{
							shader_graph_nodes_[i].StreamIn(source);
						}
					}

					{
						uint16_t num_shader_frags;
						source.read(&num_shader_frags, sizeof(num_shader_frags));
						num_shader_frags = LE2Native(num_shader_frags);
						if (num_shader_frags > 0)
						{
							shader_frags_.resize(num_shader_frags);
							for (uint32_t i = 0; i < num_shader_frags; ++ i)
							{
								shader_frags_[i].StreamIn(source);
							}
						}
					}

					{
						uint16_t num_shader_descs;
						source.read(&num_shader_descs, sizeof(num_shader_descs));
						num_shader_descs = LE2Native(num_shader_descs);
						shader_descs_.resize(num_shader_descs + 1);
						for (uint32_t i = 1; i <= num_shader_descs; ++ i)
						{
							shader_descs_[i].profile = ReadShortString(source);
							shader_descs_[i].func_name = ReadShortString(source);
							source.read(&shader_descs_[i].macros_hash, sizeof(shader_descs_[i].macros_hash));

							source.read(&shader_descs_[i].tech_pass_type, sizeof(shader_descs_[i].tech_pass_type));
							shader_descs_[i].tech_pass_type = LE2Native(shader_descs_[i].tech_pass_type);

							uint8_t len;
							source.read(&len, sizeof(len));
							if (len > 0)
							{
								shader_descs_[i].so_decl.resize(len);
								source.read(&shader_descs_[i].so_decl[0], len * sizeof(shader_descs_[i].so_decl[0]));
								for (uint32_t j = 0; j < len; ++ j)
								{
									shader_descs_[i].so_decl[j].usage = LE2Native(shader_descs_[i].so_decl[j].usage);
								}
							}
						}
					}

					ret = true;
					{
						uint16_t num_techs;
						source.read(&num_techs, sizeof(num_techs));
						num_techs = LE2Native(num_techs);
						techniques_.resize(num_techs);
						for (uint32_t i = 0; i < num_techs; ++ i)
						{
							techniques_[i] = MakeUniquePtr<RenderTechnique>();
							ret &= techniques_[i]->StreamIn(effect, source, i);
						}
					}
				}
			}
		}

		return ret;
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectTemplate::StreamOut(std::ostream& os, RenderEffect const & effect) const
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();

		uint32_t fourcc = Native2LE(MakeFourCC<'K', 'F', 'X', ' '>::value);
		os.write(reinterpret_cast<char const *>(&fourcc), sizeof(fourcc));

		uint32_t ver = Native2LE(KFX_VERSION);
		os.write(reinterpret_cast<char const *>(&ver), sizeof(ver));

		uint32_t shader_fourcc = Native2LE(re.NativeShaderFourCC());
		os.write(reinterpret_cast<char const *>(&shader_fourcc), sizeof(shader_fourcc));

		uint32_t shader_ver = Native2LE(re.NativeShaderVersion());
		os.write(reinterpret_cast<char const *>(&shader_ver), sizeof(shader_ver));

		uint8_t shader_platform_name_len = static_cast<uint8_t>(re.NativeShaderPlatformName().size());
		os.write(reinterpret_cast<char const *>(&shader_platform_name_len), sizeof(shader_platform_name_len));
		os.write(&re.NativeShaderPlatformName()[0], shader_platform_name_len);

		uint64_t timestamp = Native2LE(timestamp_);
		os.write(reinterpret_cast<char const *>(&timestamp), sizeof(timestamp));

		{
			uint16_t num_macros = 0;
			for (uint32_t i = 0; i < macros_.size(); ++ i)
			{
				if (macros_[i].second)
				{
					++ num_macros;
				}
			}

			num_macros = Native2LE(num_macros);
			os.write(reinterpret_cast<char const *>(&num_macros), sizeof(num_macros));

			for (uint32_t i = 0; i < macros_.size(); ++ i)
			{
				if (macros_[i].second)
				{
					WriteShortString(os, macros_[i].first.first);
					WriteShortString(os, macros_[i].first.second);
				}
			}
		}

		{
			uint16_t num_structs = Native2LE(static_cast<uint16_t>(struct_types_.size()));
			os.write(reinterpret_cast<char const*>(&num_structs), sizeof(num_structs));
			for (uint32_t i = 0; i < struct_types_.size(); ++i)
			{
				struct_types_[i]->StreamOut(os);
			}
		}

		{
			uint16_t num_cbufs = Native2LE(static_cast<uint16_t>(effect.cbuffers_.size()));
			os.write(reinterpret_cast<char const *>(&num_cbufs), sizeof(num_cbufs));
			for (uint32_t i = 0; i < effect.cbuffers_.size(); ++ i)
			{
				effect.cbuffers_[i]->StreamOut(os);
			}
		}

		{
			uint16_t num_params = Native2LE(static_cast<uint16_t>(effect.params_.size()));
			os.write(reinterpret_cast<char const *>(&num_params), sizeof(num_params));
			for (uint32_t i = 0; i < effect.params_.size(); ++ i)
			{
				effect.params_[i]->StreamOut(os);
			}
		}

		{
			uint8_t num_shader_graph_nodes = static_cast<uint8_t>(shader_graph_nodes_.size());
			os.write(reinterpret_cast<char const *>(&num_shader_graph_nodes), sizeof(num_shader_graph_nodes));
			for (uint32_t i = 0; i < shader_graph_nodes_.size(); ++ i)
			{
				shader_graph_nodes_[i].StreamOut(os);
			}
		}

		{
			uint16_t num_shader_frags = Native2LE(static_cast<uint16_t>(shader_frags_.size()));
			os.write(reinterpret_cast<char const *>(&num_shader_frags), sizeof(num_shader_frags));
			for (uint32_t i = 0; i < shader_frags_.size(); ++ i)
			{
				shader_frags_[i].StreamOut(os);
			}
		}

		{
			uint16_t num_shader_descs = Native2LE(static_cast<uint16_t>(shader_descs_.size() - 1));
			os.write(reinterpret_cast<char const *>(&num_shader_descs), sizeof(num_shader_descs));
			for (uint32_t i = 1; i < shader_descs_.size(); ++i)
			{
				WriteShortString(os, shader_descs_[i].profile);
				WriteShortString(os, shader_descs_[i].func_name);

				uint64_t tmp64 = Native2LE(shader_descs_[i].macros_hash);
				os.write(reinterpret_cast<char const *>(&tmp64), sizeof(tmp64));

				uint32_t tmp32 = Native2LE(shader_descs_[i].tech_pass_type);
				os.write(reinterpret_cast<char const *>(&tmp32), sizeof(tmp32));

				uint8_t len = static_cast<uint8_t>(shader_descs_[i].so_decl.size());
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				for (uint32_t j = 0; j < len; ++ j)
				{
					ShaderDesc::StreamOutputDecl so_decl = shader_descs_[i].so_decl[j];
					so_decl.usage = Native2LE(so_decl.usage);
					os.write(reinterpret_cast<char const *>(&so_decl), sizeof(so_decl));
				}
			}
		}

		{
			uint16_t num_techs = Native2LE(static_cast<uint16_t>(techniques_.size()));
			os.write(reinterpret_cast<char const *>(&num_techs), sizeof(num_techs));
			for (uint32_t i = 0; i < techniques_.size(); ++ i)
			{
				techniques_[i]->StreamOut(effect, os, i);
			}
		}
	}
#endif

	RenderEffectStructType* RenderEffectTemplate::StructTypeByName(std::string_view name) const
	{
		size_t const name_hash = HashRange(name.begin(), name.end());
		for (auto const& struct_type : struct_types_)
		{
			if (name_hash == struct_type->NameHash())
			{
				return struct_type.get();
			}
		}
		return nullptr;
	}

	RenderTechnique* RenderEffectTemplate::TechniqueByName(std::string_view name) const
	{
		size_t const name_hash = HashRange(name.begin(), name.end());
		for (auto const & tech : techniques_)
		{
			if (name_hash == tech->NameHash())
			{
				return tech.get();
			}
		}
		return nullptr;
	}

	uint32_t RenderEffectTemplate::AddShaderDesc(ShaderDesc const & sd)
	{
		for (uint32_t i = 0; i < shader_descs_.size(); ++ i)
		{
			if (shader_descs_[i] == sd)
			{
				return i;
			}
		}

		uint32_t id = static_cast<uint32_t>(shader_descs_.size());
		shader_descs_.push_back(sd);
		return id;
	}

	ShaderDesc& RenderEffectTemplate::GetShaderDesc(uint32_t id)
	{
		BOOST_ASSERT(id < shader_descs_.size());
		return shader_descs_[id];
	}

	ShaderDesc const & RenderEffectTemplate::GetShaderDesc(uint32_t id) const
	{
		BOOST_ASSERT(id < shader_descs_.size());
		return shader_descs_[id];
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectTemplate::GenHLSLShaderText(RenderEffect const & effect)
	{
		std::string& str = hlsl_shader_;

		str += "#define SHADER_MODEL(major, minor) ((major) * 4 + (minor))\n\n";

		for (uint32_t i = 0; i < this->NumMacros(); ++ i)
		{
			std::pair<std::string, std::string> const & name_value = this->MacroByIndex(i);
			str += "#define " + name_value.first + " " + name_value.second + "\n";
		}
		str += '\n';

		for (uint32_t i = 0; i < this->NumStructTypes(); ++i)
		{
			RenderEffectStructType const& struct_type = *this->StructTypeByIndex(i);
			str += "struct " + struct_type.Name() + "\n";
			str += "{\n";
			for (uint32_t j = 0; j < struct_type.NumMembers(); ++j)
			{
				RenderEffectDataType member_type = struct_type.MemberType(j);
				if (member_type == REDT_struct)
				{
					str += struct_type.MemberTypeName(j);
				}
				else
				{
					str += std::string(TypeNameFromCode(member_type));
				}

				str += " " + struct_type.MemberName(j);

				auto array_size_str = struct_type.MemberArraySize(j);
				if (array_size_str && !array_size_str->empty())
				{
					str += "[" + *array_size_str + "]";
				}
				str += ";\n";
			}
			str += "};\n";
		}

		for (uint32_t i = 0; i < effect.NumCBuffers(); ++ i)
		{
			RenderEffectConstantBuffer const & cbuff = *effect.CBufferByIndex(i);
			str += "cbuffer " + cbuff.Name() + "\n";
			str += "{\n";

			for (uint32_t j = 0; j < cbuff.NumParameters(); ++ j)
			{
				RenderEffectParameter const & param = *effect.ParameterByIndex(cbuff.ParameterIndex(j));
				switch (param.Type())
				{
				case REDT_texture1D:
				case REDT_texture2D:
				case REDT_texture2DMS:
				case REDT_texture3D:
				case REDT_textureCUBE:
				case REDT_texture1DArray:
				case REDT_texture2DArray:
				case REDT_texture2DMSArray:
				case REDT_texture3DArray:
				case REDT_textureCUBEArray:
				case REDT_sampler:
				case REDT_buffer:
				case REDT_structured_buffer:
				case REDT_byte_address_buffer:
				case REDT_rw_buffer:
				case REDT_rw_structured_buffer:
				case REDT_rw_texture1D:
				case REDT_rw_texture2D:
				case REDT_rw_texture3D:
				case REDT_rw_texture1DArray:
				case REDT_rw_texture2DArray:
				case REDT_rw_byte_address_buffer:
				case REDT_append_structured_buffer:
				case REDT_consume_structured_buffer:
				case REDT_rasterizer_ordered_buffer:
				case REDT_rasterizer_ordered_byte_address_buffer:
				case REDT_rasterizer_ordered_structured_buffer:
				case REDT_rasterizer_ordered_texture1D:
				case REDT_rasterizer_ordered_texture1DArray:
				case REDT_rasterizer_ordered_texture2D:
				case REDT_rasterizer_ordered_texture2DArray:
				case REDT_rasterizer_ordered_texture3D:
					break;

				default:
					if (param.Type() == REDT_struct)
					{
						str += param.StructType()->Name();
					}
					else
					{
						str += std::string(TypeNameFromCode(param.Type()));
					}

					str += " " + param.Name();

					auto array_size_str = param.ArraySize();
					if (array_size_str && !array_size_str->empty())
					{
						str += "[" + *array_size_str + "]";
					}
					str += ";\n";
					break;
				}
			}

			str += "};\n";
		}

		for (uint32_t i = 0; i < effect.NumParameters(); ++ i)
		{
			RenderEffectParameter& param = *effect.ParameterByIndex(i);

			std::string elem_type;
			switch (param.Type())
			{
			case REDT_texture1D:
			case REDT_texture2D:
			case REDT_texture2DMS:
			case REDT_texture3D:
			case REDT_textureCUBE:
			case REDT_texture1DArray:
			case REDT_texture2DArray:
			case REDT_texture2DMSArray:
			case REDT_textureCUBEArray:
			case REDT_buffer:
			case REDT_structured_buffer:
			case REDT_rw_buffer:
			case REDT_rw_structured_buffer:
			case REDT_rw_texture1D:
			case REDT_rw_texture2D:
			case REDT_rw_texture3D:
			case REDT_rw_texture1DArray:
			case REDT_rw_texture2DArray:
			case REDT_append_structured_buffer:
			case REDT_consume_structured_buffer:
			case REDT_rasterizer_ordered_buffer:
			case REDT_rasterizer_ordered_byte_address_buffer:
			case REDT_rasterizer_ordered_structured_buffer:
			case REDT_rasterizer_ordered_texture1D:
			case REDT_rasterizer_ordered_texture1DArray:
			case REDT_rasterizer_ordered_texture2D:
			case REDT_rasterizer_ordered_texture2DArray:
			case REDT_rasterizer_ordered_texture3D:
				param.Var().Value(elem_type);
				break;

			default:
				break;
			}

			std::string const & param_name = param.Name();
			switch (param.Type())
			{
			case REDT_texture1D:
				str += "Texture1D<" + elem_type + "> " + param_name + ";\n";
				break;

			case REDT_texture2D:
				str += "Texture2D<" + elem_type + "> " + param_name + ";\n";
				break;

			case REDT_texture2DMS:
				str += "#if KLAYGE_EXPLICIT_MULTI_SAMPLE_SUPPORT\n";
				str += "Texture2DMS<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_texture3D:
				str += "#if KLAYGE_MAX_TEX_DEPTH <= 1\n";
				str += "Texture2D<" + elem_type + "> " + param_name + ";\n";
				str += "#else\n";
				str += "Texture3D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_textureCUBE:
				str += "TextureCube<" + elem_type + "> " + param_name + ";\n";
				break;

			case REDT_texture1DArray:
				str += "#if KLAYGE_MAX_TEX_ARRAY_LEN > 1\n";
				str += "Texture1DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_texture2DArray:
				str += "#if KLAYGE_MAX_TEX_ARRAY_LEN > 1\n";
				str += "Texture2DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_texture2DMSArray:
				str += "#if KLAYGE_MAX_TEX_ARRAY_LEN > 1\n";
				str += "#if KLAYGE_EXPLICIT_MULTI_SAMPLE_SUPPORT\n";
				str += "Texture2DMSArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				str += "#endif\n";
				break;

			case REDT_textureCUBEArray:
				str += "#if (KLAYGE_MAX_TEX_ARRAY_LEN > 1) && (KLAYGE_SHADER_MODEL >= SHADER_MODEL(4, 1))\n";
				str += "TextureCubeArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_buffer:
				str += "Buffer<" + elem_type + "> " + param_name + ";\n";
				break;

			case REDT_sampler:
				str += "sampler " + param_name + ";\n";
				break;

			case REDT_structured_buffer:
				str += "StructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				break;

			case REDT_byte_address_buffer:
				str += "ByteAddressBuffer " + param_name + ";\n";
				break;

			case REDT_rw_buffer:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rw_structured_buffer:
				str += "RWStructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				break;

			case REDT_rw_texture1D:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture1D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rw_texture2D:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture2D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rw_texture3D:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture3D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rw_texture1DArray:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture1DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rw_texture2DArray:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture2DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rw_byte_address_buffer:
				str += "RWByteAddressBuffer " + param_name + ";\n";
				break;

			case REDT_append_structured_buffer:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "AppendStructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_consume_structured_buffer:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "ConsumeStructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_buffer:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_byte_address_buffer:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedByteAddressBuffer " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_structured_buffer:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedStructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_texture1D:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedTexture1D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_texture1DArray:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedTexture1DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_texture2D:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedTexture2D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_texture2DArray:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedTexture2DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			case REDT_rasterizer_ordered_texture3D:
				str += "#if KLAYGE_SHADER_MODEL >= SHADER_MODEL(5, 1)\n";
				str += "RasterizerOrderedTexture3D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;

			default:
				break;
			}
		}

		if (this->NumShaderGraphNodes() > 0)
		{
			for (uint32_t i = 0; i < this->NumShaderGraphNodes(); ++ i)
			{
				auto const & node = this->ShaderGraphNodesByIndex(i);
				str += node.GenDeclarationCode();
			}
			str += '\n';
		}

		for (uint32_t i = 0; i < this->NumShaderFragments(); ++ i)
		{
			RenderShaderFragment const & effect_shader_frag = this->ShaderFragmentByIndex(i);
			ShaderStage const shader_stage = effect_shader_frag.Stage();
			switch (shader_stage)
			{
			case ShaderStage::Vertex:
				str += "#if KLAYGE_VERTEX_SHADER\n";
				break;

			case ShaderStage::Pixel:
				str += "#if KLAYGE_PIXEL_SHADER\n";
				break;

			case ShaderStage::Geometry:
				str += "#if KLAYGE_GEOMETRY_SHADER\n";
				break;

			case ShaderStage::Compute:
				str += "#if KLAYGE_COMPUTE_SHADER\n";
				break;

			case ShaderStage::Hull:
				str += "#if KLAYGE_HULL_SHADER\n";
				break;

			case ShaderStage::Domain:
				str += "#if KLAYGE_DOMAIN_SHADER\n";
				break;

			case ShaderStage::NumStages:
				break;

			default:
				KFL_UNREACHABLE("Invalid shader type");
			}
			ShaderModel const & ver = effect_shader_frag.Version();
			if ((ver.major_ver != 0) || (ver.minor_ver != 0))
			{
				str += std::format(
					"#if KLAYGE_SHADER_MODEL >= SHADER_MODEL({}, {})\n", static_cast<int>(ver.major_ver), static_cast<int>(ver.minor_ver));
			}

			str += effect_shader_frag.str() + "\n";

			if ((ver.major_ver != 0) || (ver.minor_ver != 0))
			{
				str += "#endif\n";
			}
			if (shader_stage != ShaderStage::NumStages)
			{
				str += "#endif\n";
			}
		}

		if (this->NumShaderGraphNodes() > 0)
		{
			str += '\n';
			for (uint32_t i = 0; i < this->NumShaderGraphNodes(); ++ i)
			{
				auto const & node = this->ShaderGraphNodesByIndex(i);
				str += node.GenDefinitionCode();
			}
			str += '\n';
		}
	}
#endif


#if KLAYGE_IS_DEV_PLATFORM
	void RenderTechnique::Load(RenderEffect& effect, XMLNode const& node, uint32_t tech_index)
	{
		name_ = std::string(node.Attrib("name")->ValueString());
		name_hash_ = HashRange(name_.begin(), name_.end());

		RenderTechnique* parent_tech = nullptr;
		XMLAttributePtr inherit_attr = node.Attrib("inherit");
		if (inherit_attr)
		{
			std::string_view const inherit = inherit_attr->ValueString();
			BOOST_ASSERT(inherit != name_);

			parent_tech = effect.TechniqueByName(inherit);
			BOOST_ASSERT(parent_tech);
		}

		{
			XMLNodePtr anno_node = node.FirstNode("annotation");
			if (anno_node)
			{
				annotations_ = MakeSharedPtr<std::remove_reference<decltype(*annotations_)>::type>();
				if (parent_tech && parent_tech->annotations_)
				{
					*annotations_ = *parent_tech->annotations_;
				}
				for (; anno_node; anno_node = anno_node->NextSibling("annotation"))
				{
					RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
					annotations_->push_back(annotation);

					annotation->Load(effect, *anno_node);
				}
			}
			else if (parent_tech)
			{
				annotations_ = parent_tech->annotations_;
			}
		}

		{
			XMLNodePtr macro_node = node.FirstNode("macro");
			if (macro_node)
			{
				macros_ = MakeSharedPtr<std::remove_reference<decltype(*macros_)>::type>();
				if (parent_tech && parent_tech->macros_)
				{
					*macros_ = *parent_tech->macros_;
				}
				for (; macro_node; macro_node = macro_node->NextSibling("macro"))
				{
					std::string_view const name = macro_node->Attrib("name")->ValueString();
					std::string_view const value = macro_node->Attrib("value")->ValueString();
					bool found = false;
					for (size_t i = 0; i < macros_->size(); ++ i)
					{
						if ((*macros_)[i].first == name)
						{
							(*macros_)[i].second = std::string(value);
							found = true;
							break;
						}
					}
					if (!found)
					{
						macros_->emplace_back(name, value);
					}
				}
			}
			else if (parent_tech)
			{
				macros_ = parent_tech->macros_;
			}
		}

		if (!node.FirstNode("pass") && parent_tech)
		{
			is_validate_ = parent_tech->is_validate_;
			has_discard_ = parent_tech->has_discard_;
			has_tessellation_ = parent_tech->has_tessellation_;
			transparent_ = parent_tech->transparent_;
			weight_ = parent_tech->weight_;

			if (macros_ == parent_tech->macros_)
			{
				passes_ = parent_tech->passes_;
			}
			else
			{
				for (uint32_t index = 0; index < parent_tech->passes_.size(); ++ index)
				{
					RenderPassPtr pass = MakeSharedPtr<RenderPass>();
					passes_.push_back(pass);

					auto inherit_pass = parent_tech->passes_[index].get();

					pass->Load(effect, tech_index, index, inherit_pass);
					is_validate_ &= pass->Validate();
				}
			}
		}
		else
		{
			is_validate_ = true;

			has_discard_ = false;
			has_tessellation_ = false;
			transparent_ = false;
			if (parent_tech)
			{
				weight_ = parent_tech->Weight();
			}
			else
			{
				weight_ = 1;
			}
		
			uint32_t index = 0;
			for (XMLNodePtr pass_node = node.FirstNode("pass"); pass_node; pass_node = pass_node->NextSibling("pass"), ++ index)
			{
				RenderPassPtr pass = MakeSharedPtr<RenderPass>();
				passes_.push_back(pass);

				RenderPass* inherit_pass = nullptr;
				if (parent_tech && (index < parent_tech->passes_.size()))
				{
					inherit_pass = parent_tech->passes_[index].get();
				}

				pass->Load(effect, *pass_node, tech_index, index, inherit_pass);

				is_validate_ &= pass->Validate();

				for (XMLNodePtr state_node = pass_node->FirstNode("state"); state_node; state_node = state_node->NextSibling("state"))
				{
					++ weight_;

					std::string_view const state_name = state_node->Attrib("name")->ValueString();
					if ("blend_enable" == state_name)
					{
						std::string_view const value_str = state_node->Attrib("value")->ValueString();
						if (BoolFromStr(value_str))
						{
							transparent_ = true;
						}
					}
				}

				auto const* shader_obj = pass->GetShaderObject(effect).get();
				if (auto const* ps_stage = shader_obj->Stage(ShaderStage::Pixel).get())
				{
					has_discard_ |= ps_stage->HasDiscard();
				}
				has_tessellation_ |= !!shader_obj->Stage(ShaderStage::Hull);
			}
			if (transparent_)
			{
				weight_ += 10000;
			}
		}
	}

	void RenderTechnique::CompileShaders(RenderEffect& effect, uint32_t tech_index)
	{
		uint32_t pass_index = 0;
		for (auto& pass : passes_)
		{
			pass->CompileShaders(effect, tech_index, pass_index);
			++pass_index;
		}
	}
#endif

	void RenderTechnique::CreateHwShaders(RenderEffect& effect, uint32_t tech_index)
	{
		is_validate_ = true;

		has_discard_ = false;
		has_tessellation_ = false;

		uint32_t pass_index = 0;
		for (auto& pass : passes_)
		{
			pass->CreateHwShaders(effect, tech_index, pass_index);

			is_validate_ &= pass->Validate();

			auto const* shader_obj = pass->GetShaderObject(effect).get();
			if (auto const* ps_stage = shader_obj->Stage(ShaderStage::Pixel).get())
			{
				has_discard_ |= ps_stage->HasDiscard();
			}
			has_tessellation_ |= !!shader_obj->Stage(ShaderStage::Hull);

			++pass_index;
		}
	}

	bool RenderTechnique::StreamIn(RenderEffect& effect, ResIdentifier& res, uint32_t tech_index)
	{
		name_ = ReadShortString(res);
		name_hash_ = HashRange(name_.begin(), name_.end());

		uint8_t num_anno;
		res.read(&num_anno, sizeof(num_anno));
		if (num_anno > 0)
		{
			annotations_ = MakeSharedPtr<std::remove_reference<decltype(*annotations_)>::type>();
			annotations_->resize(num_anno);
			for (uint32_t i = 0; i < num_anno; ++ i)
			{
				RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
				(*annotations_)[i] = annotation;
				
				annotation->StreamIn(effect, res);
			}
		}

		uint8_t num_macro;
		res.read(&num_macro, sizeof(num_macro));
		if (num_macro > 0)
		{
			macros_ = MakeSharedPtr<std::remove_reference<decltype(*macros_)>::type>();
			macros_->resize(num_macro);
			for (uint32_t i = 0; i < num_macro; ++ i)
			{
				std::string name = ReadShortString(res);
				std::string value = ReadShortString(res);
				(*macros_)[i] = std::make_pair(name, value);
			}
		}

		res.read(&transparent_, sizeof(transparent_));
		res.read(&weight_, sizeof(weight_));
		weight_ = LE2Native(weight_);

		bool ret = true;
		uint8_t num_passes;
		res.read(&num_passes, sizeof(num_passes));
		passes_.resize(num_passes);
		for (uint32_t pass_index = 0; pass_index < num_passes; ++ pass_index)
		{
			RenderPassPtr pass = MakeSharedPtr<RenderPass>();
			passes_[pass_index] = pass;

			ret &= pass->StreamIn(effect, res, tech_index, pass_index);
		}

		return ret;
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderTechnique::StreamOut(RenderEffect const & effect, std::ostream& os, uint32_t tech_index) const
	{
		WriteShortString(os, name_);

		uint8_t num_anno;
		if (annotations_)
		{
			num_anno = static_cast<uint8_t>(annotations_->size());
		}
		else
		{
			num_anno = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_anno), sizeof(num_anno));
		for (uint32_t i = 0; i < num_anno; ++ i)
		{
			RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
			(*annotations_)[i] = annotation;
				
			annotation->StreamOut(os);
		}

		uint8_t num_macro;
		if (macros_)
		{
			num_macro = static_cast<uint8_t>(macros_->size());
		}
		else
		{
			num_macro = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_macro), sizeof(num_macro));
		for (uint32_t i = 0; i < num_macro; ++ i)
		{
			WriteShortString(os, (*macros_)[i].first);
			WriteShortString(os, (*macros_)[i].second);
		}

		os.write(reinterpret_cast<char const *>(&transparent_), sizeof(transparent_));
		float w = Native2LE(weight_);
		os.write(reinterpret_cast<char const *>(&w), sizeof(w));

		uint8_t num_passes = static_cast<uint8_t>(passes_.size());
		os.write(reinterpret_cast<char const *>(&num_passes), sizeof(num_passes));
		for (uint32_t pass_index = 0; pass_index < num_passes; ++ pass_index)
		{
			passes_[pass_index]->StreamOut(effect, os, tech_index, pass_index);
		}
	}
#endif

	bool RenderTechnique::HWResourceReady(RenderEffect const& effect) const
	{
		bool hw_res_ready = true;
		for (auto const& pass : passes_)
		{
			auto const* shader_obj = pass->GetShaderObject(effect).get();
			if (shader_obj)
			{
				hw_res_ready &= shader_obj->HWResourceReady();
			}
			else
			{
				hw_res_ready = false;
				break;
			}
		}

		return hw_res_ready;
	}


#if KLAYGE_IS_DEV_PLATFORM
	void RenderPass::Load(
		RenderEffect& effect, XMLNode const& node, uint32_t tech_index, uint32_t pass_index, RenderPass const* inherit_pass)
	{
		name_ = std::string(node.Attrib("name")->ValueString());
		name_hash_ = HashRange(name_.begin(), name_.end());

		{
			XMLNodePtr anno_node = node.FirstNode("annotation");
			if (anno_node)
			{
				annotations_ = MakeSharedPtr<std::remove_reference<decltype(*annotations_)>::type>();
				if (inherit_pass && inherit_pass->annotations_)
				{
					*annotations_ = *inherit_pass->annotations_;
				}
				for (; anno_node; anno_node = anno_node->NextSibling("annotation"))
				{
					RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
					annotations_->push_back(annotation);

					annotation->Load(effect, *anno_node);
				}
			}
			else if (inherit_pass)
			{
				annotations_ = inherit_pass->annotations_;
			}
		}

		{
			XMLNodePtr macro_node = node.FirstNode("macro");
			if (macro_node)
			{
				macros_ = MakeSharedPtr<std::remove_reference<decltype(*macros_)>::type>();
				if (inherit_pass && inherit_pass->macros_)
				{
					*macros_ = *inherit_pass->macros_;
				}
				for (; macro_node; macro_node = macro_node->NextSibling("macro"))
				{
					std::string_view const name = macro_node->Attrib("name")->ValueString();
					std::string_view const value = macro_node->Attrib("value")->ValueString();
					bool found = false;
					for (size_t i = 0; i < macros_->size(); ++ i)
					{
						if ((*macros_)[i].first == name)
						{
							(*macros_)[i].second = std::string(value);
							found = true;
							break;
						}
					}
					if (!found)
					{
						macros_->emplace_back(name, value);
					}
				}
			}
			else if (inherit_pass)
			{
				macros_ = inherit_pass->macros_;
			}
		}

		uint64_t macros_hash;
		{
			RenderTechnique* tech = effect.TechniqueByIndex(tech_index);

			size_t hash_val = 0;
			for (uint32_t i = 0; i < tech->NumMacros(); ++ i)
			{
				std::pair<std::string, std::string> const & name_value = tech->MacroByIndex(i);
				HashRange(hash_val, name_value.first.begin(), name_value.first.end());
				HashRange(hash_val, name_value.second.begin(), name_value.second.end());
			}
			for (uint32_t i = 0; i < this->NumMacros(); ++ i)
			{
				std::pair<std::string, std::string> const & name_value = this->MacroByIndex(i);
				HashRange(hash_val, name_value.first.begin(), name_value.first.end());
				HashRange(hash_val, name_value.second.begin(), name_value.second.end());
			}
			macros_hash = static_cast<uint64_t>(hash_val);
		}

		RasterizerStateDesc rs_desc;
		DepthStencilStateDesc dss_desc;
		BlendStateDesc bs_desc;
		shader_obj_index_ = effect.AddShaderObject();

		shader_desc_ids_.fill(0);

		if (inherit_pass)
		{
			rs_desc = inherit_pass->render_state_obj_->GetRasterizerStateDesc();
			dss_desc = inherit_pass->render_state_obj_->GetDepthStencilStateDesc();
			bs_desc = inherit_pass->render_state_obj_->GetBlendStateDesc();
			shader_desc_ids_ = inherit_pass->shader_desc_ids_;
		}

		for (XMLNodePtr state_node = node.FirstNode("state"); state_node; state_node = state_node->NextSibling("state"))
		{
			std::string_view const name = state_node->Attrib("name")->ValueString();
			size_t const state_name_hash = HashRange(name.begin(), name.end());

			XMLAttributePtr const value_attr = state_node->Attrib("value");
			std::string_view value_str;
			if (value_attr)
			{
				value_str = value_attr->ValueString();
			}

			if (CT_HASH("polygon_mode") == state_name_hash)
			{
				rs_desc.polygon_mode = PolygonModeFromName(value_str);
			}
			else if (CT_HASH("shade_mode") == state_name_hash)
			{
				rs_desc.shade_mode = ShadeModeFromName(value_str);
			}
			else if (CT_HASH("cull_mode") == state_name_hash)
			{
				rs_desc.cull_mode = CullModeFromName(value_str);
			}
			else if (CT_HASH("front_face_ccw") == state_name_hash)
			{
				rs_desc.front_face_ccw = BoolFromStr(value_str);
			}
			else if (CT_HASH("polygon_offset_factor") == state_name_hash)
			{
				rs_desc.polygon_offset_factor = value_attr->ValueFloat();
			}
			else if (CT_HASH("polygon_offset_units") == state_name_hash)
			{
				rs_desc.polygon_offset_units = value_attr->ValueFloat();
			}
			else if (CT_HASH("depth_clip_enable") == state_name_hash)
			{
				rs_desc.depth_clip_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("scissor_enable") == state_name_hash)
			{
				rs_desc.scissor_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("multisample_enable") == state_name_hash)
			{
				rs_desc.multisample_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("alpha_to_coverage_enable") == state_name_hash)
			{
				bs_desc.alpha_to_coverage_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("independent_blend_enable") == state_name_hash)
			{
				bs_desc.independent_blend_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("blend_enable") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.blend_enable[index] = BoolFromStr(value_str);
			}
			else if (CT_HASH("logic_op_enable") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.logic_op_enable[index] = BoolFromStr(value_str);
			}
			else if (CT_HASH("blend_op") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.blend_op[index] = BlendOperationFromName(value_str);
			}
			else if (CT_HASH("src_blend") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.src_blend[index] = AlphaBlendFactorFromName(value_str);
			}
			else if (CT_HASH("dest_blend") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.dest_blend[index] = AlphaBlendFactorFromName(value_str);
			}
			else if (CT_HASH("blend_op_alpha") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.blend_op_alpha[index] = BlendOperationFromName(value_str);
			}
			else if (CT_HASH("src_blend_alpha") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.src_blend_alpha[index] = AlphaBlendFactorFromName(value_str);
			}
			else if (CT_HASH("dest_blend_alpha") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.dest_blend_alpha[index] = AlphaBlendFactorFromName(value_str);
			}
			else if (CT_HASH("logic_op") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.logic_op[index] = LogicOperationFromName(value_str);
			}
			else if (CT_HASH("color_write_mask") == state_name_hash)
			{
				int index = RetrieveIndex(*state_node);
				bs_desc.color_write_mask[index] = static_cast<uint8_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("blend_factor") == state_name_hash)
			{
				XMLAttributePtr attr = state_node->Attrib("r");
				if (attr)
				{
					bs_desc.blend_factor.r() = attr->ValueFloat();
				}
				attr = state_node->Attrib("g");
				if (attr)
				{
					bs_desc.blend_factor.g() = attr->ValueFloat();
				}
				attr = state_node->Attrib("b");
				if (attr)
				{
					bs_desc.blend_factor.b() = attr->ValueFloat();
				}
				attr = state_node->Attrib("a");
				if (attr)
				{
					bs_desc.blend_factor.a() = attr->ValueFloat();
				}
			}
			else if (CT_HASH("sample_mask") == state_name_hash)
			{
				bs_desc.sample_mask = value_attr->ValueUInt();
			}
			else if (CT_HASH("depth_enable") == state_name_hash)
			{
				dss_desc.depth_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("depth_write_mask") == state_name_hash)
			{
				dss_desc.depth_write_mask = BoolFromStr(value_str);
			}
			else if (CT_HASH("depth_func") == state_name_hash)
			{
				dss_desc.depth_func = CompareFunctionFromName(value_str);
			}
			else if (CT_HASH("front_stencil_enable") == state_name_hash)
			{
				dss_desc.front_stencil_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("front_stencil_func") == state_name_hash)
			{
				dss_desc.front_stencil_func = CompareFunctionFromName(value_str);
			}
			else if (CT_HASH("front_stencil_ref") == state_name_hash)
			{
				dss_desc.front_stencil_ref = static_cast<uint16_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("front_stencil_read_mask") == state_name_hash)
			{
				dss_desc.front_stencil_read_mask = static_cast<uint16_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("front_stencil_write_mask") == state_name_hash)
			{
				dss_desc.front_stencil_write_mask = static_cast<uint16_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("front_stencil_fail") == state_name_hash)
			{
				dss_desc.front_stencil_fail = StencilOperationFromName(value_str);
			}
			else if (CT_HASH("front_stencil_depth_fail") == state_name_hash)
			{
				dss_desc.front_stencil_depth_fail = StencilOperationFromName(value_str);
			}
			else if (CT_HASH("front_stencil_pass") == state_name_hash)
			{
				dss_desc.front_stencil_pass = StencilOperationFromName(value_str);
			}
			else if (CT_HASH("back_stencil_enable") == state_name_hash)
			{
				dss_desc.back_stencil_enable = BoolFromStr(value_str);
			}
			else if (CT_HASH("back_stencil_func") == state_name_hash)
			{
				dss_desc.back_stencil_func = CompareFunctionFromName(value_str);
			}
			else if (CT_HASH("back_stencil_ref") == state_name_hash)
			{
				dss_desc.back_stencil_ref = static_cast<uint16_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("back_stencil_read_mask") == state_name_hash)
			{
				dss_desc.back_stencil_read_mask = static_cast<uint16_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("back_stencil_write_mask") == state_name_hash)
			{
				dss_desc.back_stencil_write_mask = static_cast<uint16_t>(value_attr->ValueUInt());
			}
			else if (CT_HASH("back_stencil_fail") == state_name_hash)
			{
				dss_desc.back_stencil_fail = StencilOperationFromName(value_str);
			}
			else if (CT_HASH("back_stencil_depth_fail") == state_name_hash)
			{
				dss_desc.back_stencil_depth_fail = StencilOperationFromName(value_str);
			}
			else if (CT_HASH("back_stencil_pass") == state_name_hash)
			{
				dss_desc.back_stencil_pass = StencilOperationFromName(value_str);
			}
			else if ((CT_HASH("vertex_shader") == state_name_hash) || (CT_HASH("pixel_shader") == state_name_hash)
				|| (CT_HASH("geometry_shader") == state_name_hash) || (CT_HASH("compute_shader") == state_name_hash)
				|| (CT_HASH("hull_shader") == state_name_hash) || (CT_HASH("domain_shader") == state_name_hash))
			{
				ShaderStage stage;
				if (CT_HASH("vertex_shader") == state_name_hash)
				{
					stage = ShaderStage::Vertex;
				}
				else if (CT_HASH("pixel_shader") == state_name_hash)
				{
					stage = ShaderStage::Pixel;
				}
				else if (CT_HASH("geometry_shader") == state_name_hash)
				{
					stage = ShaderStage::Geometry;
				}
				else if (CT_HASH("compute_shader") == state_name_hash)
				{
					stage = ShaderStage::Compute;
				}
				else if (CT_HASH("hull_shader") == state_name_hash)
				{
					stage = ShaderStage::Hull;
				}
				else
				{
					BOOST_ASSERT(CT_HASH("domain_shader") == state_name_hash);
					stage = ShaderStage::Domain;
				}

				ShaderDesc sd;
				sd.profile = RetrieveProfile(*state_node);
				sd.func_name = RetrieveFuncName(*state_node);
				sd.macros_hash = macros_hash;

				if ((ShaderStage::Vertex == stage) || (ShaderStage::Geometry == stage))
				{
					XMLNodePtr so_node = state_node->FirstNode("stream_output");
					if (so_node)
					{
						for (XMLNodePtr entry_node = so_node->FirstNode("entry"); entry_node; entry_node = entry_node->NextSibling("entry"))
						{
							ShaderDesc::StreamOutputDecl decl;

							std::string_view const usage_str = entry_node->Attrib("usage")->ValueString();
							size_t const usage_str_hash = HashRange(usage_str.begin(), usage_str.end());
							XMLAttributePtr attr = entry_node->Attrib("usage_index");
							if (attr)
							{
								decl.usage_index = static_cast<uint8_t>(attr->ValueInt());
							}
							else
							{
								decl.usage_index = 0;
							}

							if ((CT_HASH("POSITION") == usage_str_hash) || (CT_HASH("SV_Position") == usage_str_hash))
							{
								decl.usage = VEU_Position;
							}
							else if (CT_HASH("NORMAL") == usage_str_hash)
							{
								decl.usage = VEU_Normal;
							}
							else if (CT_HASH("COLOR") == usage_str_hash)
							{
								if (0 == decl.usage_index)
								{
									decl.usage = VEU_Diffuse;
								}
								else
								{
									decl.usage = VEU_Specular;
								}
							}
							else if (CT_HASH("BLENDWEIGHT") == usage_str_hash)
							{
								decl.usage = VEU_BlendWeight;
							}
							else if (CT_HASH("BLENDINDICES") == usage_str_hash)
							{
								decl.usage = VEU_BlendIndex;
							}
							else if (CT_HASH("TEXCOORD") == usage_str_hash)
							{
								decl.usage = VEU_TextureCoord;
							}
							else if (CT_HASH("TANGENT") == usage_str_hash)
							{
								decl.usage = VEU_Tangent;
							}
							else if (CT_HASH("BINORMAL") == usage_str_hash)
							{
								decl.usage = VEU_Binormal;
							}
							else
							{
								KFL_UNREACHABLE("Invalid usage");
							}

							attr = entry_node->Attrib("component");
							std::string component_str;
							if (attr)
							{
								component_str = std::string(attr->ValueString());
							}
							else
							{
								component_str = "xyzw";
							}
							decl.start_component = static_cast<uint8_t>(component_str[0] - 'x');
							decl.component_count = static_cast<uint8_t>(std::min(static_cast<size_t>(4), component_str.size()));

							attr = entry_node->Attrib("slot");
							if (attr)
							{
								decl.slot = static_cast<uint8_t>(attr->ValueInt());
							}
							else
							{
								decl.slot = 0;
							}

							sd.so_decl.push_back(decl);
						}
					}
				}

				shader_desc_ids_[static_cast<uint32_t>(stage)] = effect.AddShaderDesc(sd);
			}
			else
			{
				KFL_UNREACHABLE("Invalid state name");
			}
		}

		auto& rf = Context::Instance().RenderFactoryInstance();
		render_state_obj_ = rf.MakeRenderStateObject(rs_desc, dss_desc, bs_desc);

		auto const & shader_obj = this->GetShaderObject(effect);

		for (uint32_t stage_index = 0; stage_index < NumShaderStages; ++stage_index)
		{
			ShaderDesc& sd = effect.GetShaderDesc(shader_desc_ids_[stage_index]);
			if (!sd.func_name.empty())
			{
				ShaderStage const stage = static_cast<ShaderStage>(stage_index);
				ShaderStageObjectPtr shader_stage;
				if (sd.tech_pass_type == 0xFFFFFFFF)
				{
					shader_stage = rf.MakeShaderStageObject(stage);
					sd.tech_pass_type = (tech_index << 16) + (pass_index << 8) + stage_index;
				}
				else
				{
					auto const& tech = *effect.TechniqueByIndex(sd.tech_pass_type >> 16);
					auto const& pass = tech.Pass((sd.tech_pass_type >> 8) & 0xFF);
					shader_stage = pass.GetShaderObject(effect)->Stage(stage);
				}

				shader_obj->AttachStage(stage, shader_stage);
			}
		}
	}

	void RenderPass::Load(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index, RenderPass const * inherit_pass)
	{
		BOOST_ASSERT(inherit_pass);

		name_ = inherit_pass->name_;
		annotations_ = inherit_pass->annotations_;
		macros_ = inherit_pass->macros_;

		uint64_t macros_hash;
		{
			auto const & tech = *effect.TechniqueByIndex(tech_index);

			size_t hash_val = 0;
			for (uint32_t i = 0; i < tech.NumMacros(); ++ i)
			{
				std::pair<std::string, std::string> const & name_value = tech.MacroByIndex(i);
				HashRange(hash_val, name_value.first.begin(), name_value.first.end());
				HashRange(hash_val, name_value.second.begin(), name_value.second.end());
			}
			for (uint32_t i = 0; i < this->NumMacros(); ++ i)
			{
				std::pair<std::string, std::string> const & name_value = this->MacroByIndex(i);
				HashRange(hash_val, name_value.first.begin(), name_value.first.end());
				HashRange(hash_val, name_value.second.begin(), name_value.second.end());
			}
			macros_hash = static_cast<uint64_t>(hash_val);
		}

		shader_obj_index_ = effect.AddShaderObject();
		auto const & shader_obj = this->GetShaderObject(effect);

		shader_desc_ids_.fill(0);

		render_state_obj_ = inherit_pass->render_state_obj_;

		for (uint32_t stage_index = 0; stage_index < NumShaderStages; ++stage_index)
		{
			ShaderDesc sd = effect.GetShaderDesc(inherit_pass->shader_desc_ids_[stage_index]);
			if (!sd.func_name.empty())
			{
				sd.macros_hash = macros_hash;
				sd.tech_pass_type = (tech_index << 16) + (pass_index << 8) + stage_index;
				shader_desc_ids_[stage_index] = effect.AddShaderDesc(sd);
			}
		}

		for (uint32_t stage_index = 0; stage_index < NumShaderStages; ++stage_index)
		{
			ShaderDesc const& sd = effect.GetShaderDesc(shader_desc_ids_[stage_index]);
			if (!sd.func_name.empty())
			{
				ShaderStage const stage = static_cast<ShaderStage>(stage_index);
				ShaderStageObjectPtr shader_stage;
				if (sd.tech_pass_type == (tech_index << 16) + (pass_index << 8) + stage_index)
				{
					auto& rf = Context::Instance().RenderFactoryInstance();
					shader_stage = rf.MakeShaderStageObject(stage);
				}
				else
				{
					auto const& tech = *effect.TechniqueByIndex(sd.tech_pass_type >> 16);
					auto const& pass = tech.Pass((sd.tech_pass_type >> 8) & 0xFF);
					shader_stage = pass.GetShaderObject(effect)->Stage(stage);
				}

				shader_obj->AttachStage(stage, shader_stage);
			}
		}
	}

	void RenderPass::CompileShaders(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index)
	{
		auto const & shader_obj = this->GetShaderObject(effect);
		for (uint32_t stage_index = 0; stage_index < NumShaderStages; ++stage_index)
		{
			ShaderDesc const& sd = effect.GetShaderDesc(shader_desc_ids_[stage_index]);
			if (!sd.func_name.empty())
			{
				ShaderStage const stage = static_cast<ShaderStage>(stage_index);
				if (sd.tech_pass_type == (tech_index << 16) + (pass_index << 8) + stage_index)
				{
					auto const & tech = *effect.TechniqueByIndex(tech_index);
					shader_obj->Stage(stage)->CompileShader(effect, tech, *this, shader_desc_ids_);
				}
			}
		}
	}
#endif

	void RenderPass::CreateHwShaders(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index)
	{
		auto const & shader_obj = this->GetShaderObject(effect);
		for (uint32_t stage_index = 0; stage_index < NumShaderStages; ++stage_index)
		{
			ShaderDesc const& sd = effect.GetShaderDesc(shader_desc_ids_[stage_index]);
			if (!sd.func_name.empty())
			{
				ShaderStage const stage = static_cast<ShaderStage>(stage_index);
				if (sd.tech_pass_type == (tech_index << 16) + (pass_index << 8) + stage_index)
				{
					shader_obj->Stage(stage)->CreateHwShader(effect, shader_desc_ids_);
				}
			}
		}

		shader_obj->LinkShaders(effect);

		is_validate_ = shader_obj->Validate();
	}

	bool RenderPass::StreamIn(RenderEffect& effect, ResIdentifier& res, uint32_t tech_index, uint32_t pass_index)
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();

		name_ = ReadShortString(res);
		name_hash_ = HashRange(name_.begin(), name_.end());

		uint8_t num_anno;
		res.read(&num_anno, sizeof(num_anno));
		if (num_anno > 0)
		{
			annotations_ = MakeSharedPtr<std::remove_reference<decltype(*annotations_)>::type>();
			annotations_->resize(num_anno);
			for (uint32_t i = 0; i < num_anno; ++ i)
			{
				RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
				(*annotations_)[i] = annotation;
				annotation->StreamIn(effect, res);
			}
		}

		uint8_t num_macro;
		res.read(&num_macro, sizeof(num_macro));
		if (num_macro > 0)
		{
			macros_ = MakeSharedPtr<std::remove_reference<decltype(*macros_)>::type>();
			macros_->resize(num_macro);
			for (uint32_t i = 0; i < num_macro; ++ i)
			{
				std::string name = ReadShortString(res);
				std::string value = ReadShortString(res);
				(*macros_)[i] = std::make_pair(name, value);
			}
		}

		RasterizerStateDesc rs_desc;
		DepthStencilStateDesc dss_desc;
		BlendStateDesc bs_desc;

		res.read(&rs_desc, sizeof(rs_desc));
		rs_desc.polygon_mode = LE2Native(rs_desc.polygon_mode);
		rs_desc.shade_mode = LE2Native(rs_desc.shade_mode);
		rs_desc.cull_mode = LE2Native(rs_desc.cull_mode);
		rs_desc.polygon_offset_factor = LE2Native(rs_desc.polygon_offset_factor);
		rs_desc.polygon_offset_units = LE2Native(rs_desc.polygon_offset_units);
		
		res.read(&dss_desc, sizeof(dss_desc));
		dss_desc.depth_func = LE2Native(dss_desc.depth_func);
		dss_desc.front_stencil_func = LE2Native(dss_desc.front_stencil_func);
		dss_desc.front_stencil_ref = LE2Native(dss_desc.front_stencil_ref);
		dss_desc.front_stencil_read_mask = LE2Native(dss_desc.front_stencil_read_mask);
		dss_desc.front_stencil_write_mask = LE2Native(dss_desc.front_stencil_write_mask);
		dss_desc.front_stencil_fail = LE2Native(dss_desc.front_stencil_fail);
		dss_desc.front_stencil_depth_fail = LE2Native(dss_desc.front_stencil_depth_fail);
		dss_desc.front_stencil_pass = LE2Native(dss_desc.front_stencil_pass);
		dss_desc.back_stencil_func = LE2Native(dss_desc.back_stencil_func);
		dss_desc.back_stencil_ref = LE2Native(dss_desc.back_stencil_ref);
		dss_desc.back_stencil_read_mask = LE2Native(dss_desc.back_stencil_read_mask);
		dss_desc.back_stencil_write_mask = LE2Native(dss_desc.back_stencil_write_mask);
		dss_desc.back_stencil_fail = LE2Native(dss_desc.back_stencil_fail);
		dss_desc.back_stencil_depth_fail = LE2Native(dss_desc.back_stencil_depth_fail);
		dss_desc.back_stencil_pass = LE2Native(dss_desc.back_stencil_pass);

		res.read(&bs_desc, sizeof(bs_desc));
		for (size_t i = 0; i < 4; ++ i)
		{
			bs_desc.blend_factor[i] = LE2Native(bs_desc.blend_factor[i]);
		}
		bs_desc.sample_mask = LE2Native(bs_desc.sample_mask);
		for (size_t i = 0; i < bs_desc.blend_op.size(); ++ i)
		{
			bs_desc.blend_op[i] = LE2Native(bs_desc.blend_op[i]);
			bs_desc.src_blend[i] = LE2Native(bs_desc.src_blend[i]);
			bs_desc.dest_blend[i] = LE2Native(bs_desc.dest_blend[i]);
			bs_desc.blend_op_alpha[i] = LE2Native(bs_desc.blend_op_alpha[i]);
			bs_desc.src_blend_alpha[i] = LE2Native(bs_desc.src_blend_alpha[i]);
			bs_desc.dest_blend_alpha[i] = LE2Native(bs_desc.dest_blend_alpha[i]);
		}
		
		render_state_obj_ = rf.MakeRenderStateObject(rs_desc, dss_desc, bs_desc);

		res.read(&shader_desc_ids_[0], shader_desc_ids_.size() * sizeof(shader_desc_ids_[0]));
		for (uint32_t stage = 0; stage < NumShaderStages; ++stage)
		{
			shader_desc_ids_[stage] = LE2Native(shader_desc_ids_[stage]);
		}

		shader_obj_index_ = effect.AddShaderObject();
		auto const& shader_obj = this->GetShaderObject(effect);

		bool native_accepted = true;
		for (uint32_t stage_index = 0; stage_index < NumShaderStages; ++stage_index)
		{
			ShaderDesc const& sd = effect.GetShaderDesc(shader_desc_ids_[stage_index]);
			if (!sd.func_name.empty())
			{
				ShaderStage const stage = static_cast<ShaderStage>(stage_index);

				ShaderStageObjectPtr shader_stage;
				if (sd.tech_pass_type == (tech_index << 16) + (pass_index << 8) + stage_index)
				{
					shader_stage = rf.MakeShaderStageObject(stage);
					shader_stage->StreamIn(effect, shader_desc_ids_, res);
				}
				else
				{
					auto const& tech = *effect.TechniqueByIndex(sd.tech_pass_type >> 16);
					auto const& pass = tech.Pass((sd.tech_pass_type >> 8) & 0xFF);
					shader_stage = pass.GetShaderObject(effect)->Stage(stage);
				}

				shader_obj->AttachStage(stage, shader_stage);

				native_accepted &= shader_stage->Validate();
			}
		}

		return native_accepted;
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderPass::StreamOut(RenderEffect const & effect, std::ostream& os, uint32_t tech_index, uint32_t pass_index) const
	{
		WriteShortString(os, name_);

		uint8_t num_anno;
		if (annotations_)
		{
			num_anno = static_cast<uint8_t>(annotations_->size());
		}
		else
		{
			num_anno = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_anno), sizeof(num_anno));
		for (uint32_t i = 0; i < num_anno; ++ i)
		{
			RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
			(*annotations_)[i] = annotation;
				
			annotation->StreamOut(os);
		}

		uint8_t num_macro;
		if (macros_)
		{
			num_macro = static_cast<uint8_t>(macros_->size());
		}
		else
		{
			num_macro = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_macro), sizeof(num_macro));
		for (uint32_t i = 0; i < num_macro; ++ i)
		{
			WriteShortString(os, (*macros_)[i].first);
			WriteShortString(os, (*macros_)[i].second);
		}

		RasterizerStateDesc rs_desc = render_state_obj_->GetRasterizerStateDesc();
		DepthStencilStateDesc dss_desc = render_state_obj_->GetDepthStencilStateDesc();
		BlendStateDesc bs_desc = render_state_obj_->GetBlendStateDesc();

		rs_desc.polygon_mode = Native2LE(rs_desc.polygon_mode);
		rs_desc.shade_mode = Native2LE(rs_desc.shade_mode);
		rs_desc.cull_mode = Native2LE(rs_desc.cull_mode);
		rs_desc.polygon_offset_factor = Native2LE(rs_desc.polygon_offset_factor);
		rs_desc.polygon_offset_units = Native2LE(rs_desc.polygon_offset_units);
		os.write(reinterpret_cast<char const *>(&rs_desc), sizeof(rs_desc));
		
		dss_desc.depth_func = Native2LE(dss_desc.depth_func);
		dss_desc.front_stencil_func = Native2LE(dss_desc.front_stencil_func);
		dss_desc.front_stencil_ref = Native2LE(dss_desc.front_stencil_ref);
		dss_desc.front_stencil_read_mask = Native2LE(dss_desc.front_stencil_read_mask);
		dss_desc.front_stencil_write_mask = Native2LE(dss_desc.front_stencil_write_mask);
		dss_desc.front_stencil_fail = Native2LE(dss_desc.front_stencil_fail);
		dss_desc.front_stencil_depth_fail = Native2LE(dss_desc.front_stencil_depth_fail);
		dss_desc.front_stencil_pass = Native2LE(dss_desc.front_stencil_pass);
		dss_desc.back_stencil_func = Native2LE(dss_desc.back_stencil_func);
		dss_desc.back_stencil_ref = Native2LE(dss_desc.back_stencil_ref);
		dss_desc.back_stencil_read_mask = Native2LE(dss_desc.back_stencil_read_mask);
		dss_desc.back_stencil_write_mask = Native2LE(dss_desc.back_stencil_write_mask);
		dss_desc.back_stencil_fail = Native2LE(dss_desc.back_stencil_fail);
		dss_desc.back_stencil_depth_fail = Native2LE(dss_desc.back_stencil_depth_fail);
		dss_desc.back_stencil_pass = Native2LE(dss_desc.back_stencil_pass);
		os.write(reinterpret_cast<char const *>(&dss_desc), sizeof(dss_desc));

		for (size_t i = 0; i < 4; ++ i)
		{
			bs_desc.blend_factor[i] = Native2LE(bs_desc.blend_factor[i]);
		}
		bs_desc.sample_mask = Native2LE(bs_desc.sample_mask);
		for (size_t i = 0; i < bs_desc.blend_op.size(); ++ i)
		{
			bs_desc.blend_op[i] = Native2LE(bs_desc.blend_op[i]);
			bs_desc.src_blend[i] = Native2LE(bs_desc.src_blend[i]);
			bs_desc.dest_blend[i] = Native2LE(bs_desc.dest_blend[i]);
			bs_desc.blend_op_alpha[i] = Native2LE(bs_desc.blend_op_alpha[i]);
			bs_desc.src_blend_alpha[i] = Native2LE(bs_desc.src_blend_alpha[i]);
			bs_desc.dest_blend_alpha[i] = Native2LE(bs_desc.dest_blend_alpha[i]);
		}		
		os.write(reinterpret_cast<char const *>(&bs_desc), sizeof(bs_desc));

		for (uint32_t i = 0; i < shader_desc_ids_.size(); ++ i)
		{
			uint32_t tmp = Native2LE(shader_desc_ids_[i]);
			os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		}

		for (uint32_t stage = 0; stage < NumShaderStages; ++stage)
		{
			ShaderDesc const& sd = effect.GetShaderDesc(shader_desc_ids_[stage]);
			if (!sd.func_name.empty())
			{
				if (sd.tech_pass_type == (tech_index << 16) + (pass_index << 8) + stage)
				{
					this->GetShaderObject(effect)->Stage(static_cast<ShaderStage>(stage))->StreamOut(os);
				}
			}
		}
	}
#endif

	void RenderPass::Bind(RenderEffect const & effect) const
	{
		RenderEngine& render_eng = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		render_eng.SetStateObject(render_state_obj_);

		this->GetShaderObject(effect)->Bind(effect);
	}

	void RenderPass::Unbind(RenderEffect const & effect) const
	{
		this->GetShaderObject(effect)->Unbind();
	}


#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectConstantBuffer::Load(std::string const & name)
	{
		name_ = MakeSharedPtr<std::remove_reference<decltype(*name_)>::type>();
		name_->first = name;
		name_->second = HashRange(name_->first.begin(), name_->first.end());
		param_indices_ = MakeSharedPtr<std::remove_reference<decltype(*param_indices_)>::type>();
	}
#endif

	void RenderEffectConstantBuffer::StreamIn(ResIdentifier& res)
	{
		name_ = MakeSharedPtr<std::remove_reference<decltype(*name_)>::type>();
		name_->first = ReadShortString(res);
		name_->second = HashRange(name_->first.begin(), name_->first.end());
		param_indices_ = MakeSharedPtr<std::remove_reference<decltype(*param_indices_)>::type>();

		uint16_t len;
		res.read(&len, sizeof(len));
		len = LE2Native(len);
		param_indices_->resize(len);
		res.read(&(*param_indices_)[0], len * sizeof((*param_indices_)[0]));
		for (uint32_t i = 0; i < len; ++ i)
		{
			(*param_indices_)[i] = LE2Native((*param_indices_)[i]);
		}
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectConstantBuffer::StreamOut(std::ostream& os) const
	{
		WriteShortString(os, name_->first);

		uint16_t len = Native2LE(static_cast<uint16_t>(param_indices_->size()));
		os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		for (size_t i = 0; i < param_indices_->size(); ++ i)
		{
			uint32_t tmp = Native2LE((*param_indices_)[i]);
			os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		}
	}
#endif

	RenderEffectConstantBufferPtr RenderEffectConstantBuffer::Clone(RenderEffect const& dst_effect)
	{
		auto ret = MakeSharedPtr<RenderEffectConstantBuffer>(dst_effect);
		this->Reclone(*ret, dst_effect);
		return ret;
	}

	void RenderEffectConstantBuffer::Reclone(RenderEffectConstantBuffer& dst_cbuffer, RenderEffect const& dst_effect)
	{
		dst_cbuffer.name_ = name_;
		if (effect_->ResNameHash() == dst_effect.ResNameHash())
		{
			dst_cbuffer.param_indices_ = param_indices_;
		}
		else
		{
			dst_cbuffer.param_indices_ = MakeSharedPtr<std::vector<uint32_t>>(param_indices_->size());
		}
		dst_cbuffer.buff_ = buff_;
		dst_cbuffer.Resize(static_cast<uint32_t>(buff_.size()));

		this->RebindParameters(dst_cbuffer, dst_effect);
	}

	void RenderEffectConstantBuffer::RebindParameters(RenderEffectConstantBuffer& dst_cbuffer, RenderEffect const& dst_effect)
	{
		if (effect_ != &dst_effect)
		{
			for (uint32_t i = 0; i < param_indices_->size(); ++i)
			{
				uint32_t param_index = (*param_indices_)[i];
				RenderEffectParameter* src_param = effect_->ParameterByIndex(param_index);
				if (src_param->InCBuffer())
				{
					if (effect_->ResNameHash() != dst_effect.ResNameHash())
					{
						for (uint32_t j = 0; j < dst_effect.NumParameters(); ++j)
						{
							if (dst_effect.ParameterByIndex(j)->NameHash() == src_param->NameHash())
							{
								param_index = j;
								break;
							}
						}
					}

					RenderEffectParameter* dst_param = dst_effect.ParameterByIndex(param_index);
					if (dst_param->InCBuffer())
					{
						dst_param->RebindToCBuffer(dst_effect, src_param->CBufferIndex());
					}
					else
					{
						dst_param->BindToCBuffer(dst_effect, src_param->CBufferIndex(), src_param->CBufferOffset(), src_param->Stride());
					}

					(*dst_cbuffer.param_indices_)[i] = param_index;
				}
			}
		}
	}

	void RenderEffectConstantBuffer::AddParameter(uint32_t index)
	{
		param_indices_->push_back(index);
	}

	void RenderEffectConstantBuffer::Resize(uint32_t size)
	{
		buff_.resize(size);
		if (size > 0)
		{
			if (!hw_buff_ || (size > hw_buff_->Size()))
			{
				RenderFactory& rf = Context::Instance().RenderFactoryInstance();
				hw_buff_ = rf.MakeConstantBuffer(BU_Dynamic, 0, size, nullptr);
			}
		}

		dirty_ = true;
	}

	void RenderEffectConstantBuffer::Update()
	{
		if (dirty_)
		{
			hw_buff_->UpdateSubresource(0, static_cast<uint32_t>(buff_.size()), &buff_[0]);

			dirty_ = false;
		}
	}

	void RenderEffectConstantBuffer::BindHWBuff(GraphicsBufferPtr const & buff)
	{
		hw_buff_ = buff;
		buff_.resize(buff->Size());
	}


#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectParameter::Load(RenderEffect const& effect, XMLNode const& node)
	{
		auto type_name = node.Attrib("type")->ValueString();
		auto* struct_type = effect.StructTypeByName(type_name);
		if (struct_type)
		{
			type_ = REDT_struct;
		}
		else
		{
			type_ = TypeFromName(type_name);
		}

		name_ = MakeSharedPtr<std::remove_reference<decltype(*name_)>::type>();
		name_->first = std::string(node.Attrib("name")->ValueString());
		name_->second = HashRange(name_->first.begin(), name_->first.end());

		XMLAttributePtr attr = node.Attrib("semantic");
		if (attr)
		{
			semantic_ = MakeSharedPtr<std::remove_reference<decltype(*semantic_)>::type>();
			semantic_->first = std::string(attr->ValueString());
			semantic_->second = HashRange(semantic_->first.begin(), semantic_->first.end());
		}

		uint32_t as;
		attr = node.Attrib("array_size");
		if (attr)
		{
			array_size_ = MakeSharedPtr<std::string>(attr->ValueString());

			if (!attr->TryConvert(as))
			{
				as = 1;  // dummy array size
			}
		}
		else
		{
			as = 0;
		}
		var_ = LoadVariable(effect, node, type_, as);

		{
			XMLNodePtr anno_node = node.FirstNode("annotation");
			if (anno_node)
			{
				annotations_ = MakeSharedPtr<std::remove_reference<decltype(*annotations_)>::type>();
				for (; anno_node; anno_node = anno_node->NextSibling("annotation"))
				{
					annotations_->push_back(MakeUniquePtr<RenderEffectAnnotation>());
					annotations_->back()->Load(effect, *anno_node);
				}
			}
		}

		if (annotations_ && ((REDT_texture1D == type_) || (REDT_texture2D == type_) || (REDT_texture2DMS == type_)
			|| (REDT_texture3D == type_) || (REDT_textureCUBE == type_)
			|| (REDT_texture1DArray == type_) || (REDT_texture2DArray == type_) || (REDT_texture2DMSArray == type_)
			|| (REDT_texture3DArray == type_) || (REDT_textureCUBEArray == type_)))
		{
			for (size_t i = 0; i < annotations_->size(); ++ i)
			{
				if (REDT_string == (*annotations_)[i]->Type())
				{
					if ("SasResourceAddress" == (*annotations_)[i]->Name())
					{
						std::string val;
						(*annotations_)[i]->Value(val);

						if (ResLoader::Instance().Locate(val).empty())
						{
							LogError() << val << " NOT found" << std::endl;
						}
						else
						{
							*var_ = SyncLoadTexture(val, EAH_GPU_Read | EAH_Immutable);
						}
					}
				}
			}
		}
	}
#endif

	void RenderEffectParameter::StreamIn(RenderEffect const& effect, ResIdentifier& res)
	{
		res.read(&type_, sizeof(type_));
		type_ = LE2Native(type_);
		name_ = MakeSharedPtr<std::remove_reference<decltype(*name_)>::type>();
		name_->first = ReadShortString(res);
		name_->second = HashRange(name_->first.begin(), name_->first.end());

		std::string sem = ReadShortString(res);
		if (!sem.empty())
		{
			semantic_ = MakeSharedPtr<std::remove_reference<decltype(*semantic_)>::type>();
			semantic_->first = sem;
			semantic_->second = HashRange(sem.begin(), sem.end());
		}

		uint32_t as;
		std::string as_str = ReadShortString(res);
		if (as_str.empty())
		{
			as = 0;
		}
		else
		{
			array_size_ = MakeSharedPtr<std::string>(as_str);

#ifdef KLAYGE_CXX17_LIBRARY_CHARCONV_SUPPORT
			char const* str = as_str.c_str();
			std::from_chars_result result = std::from_chars(str, str + as_str.size(), as);
			if (result.ec != std::errc())
			{
				as = 1;  // dummy array size
			}
#else
			try
			{
				as = std::stoul(as_str);
			}
			catch (...)
			{
				as = 1;  // dummy array size
			}
#endif
		}
		var_ = StreamInVariable(effect, res, type_, as);

		uint8_t num_anno;
		res.read(&num_anno, sizeof(num_anno));
		if (num_anno > 0)
		{
			annotations_ = MakeSharedPtr<std::remove_reference<decltype(*annotations_)>::type>();
			annotations_->resize(num_anno);
			for (uint32_t i = 0; i < num_anno; ++ i)
			{
				(*annotations_)[i] = MakeUniquePtr<RenderEffectAnnotation>();
				(*annotations_)[i]->StreamIn(effect, res);
			}
		}

		if (annotations_ && ((REDT_texture1D == type_) || (REDT_texture2D == type_) || (REDT_texture2DMS == type_)
			|| (REDT_texture3D == type_) || (REDT_textureCUBE == type_)
			|| (REDT_texture1DArray == type_) || (REDT_texture2DArray == type_) || (REDT_texture2DMSArray == type_)
			|| (REDT_texture3DArray == type_) || (REDT_textureCUBEArray == type_)))
		{
			for (size_t i = 0; i < annotations_->size(); ++ i)
			{
				if (REDT_string == (*annotations_)[i]->Type())
				{
					if ("SasResourceAddress" == (*annotations_)[i]->Name())
					{
						std::string val;
						(*annotations_)[i]->Value(val);

						if (ResLoader::Instance().Locate(val).empty())
						{
							LogError() << val << " NOT found" << std::endl;
						}
						else
						{
							*var_ = SyncLoadTexture(val, EAH_GPU_Read | EAH_Immutable);
						}
					}
				}
			}
		}
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderEffectParameter::StreamOut(std::ostream& os) const
	{
		uint32_t t = Native2LE(type_);
		os.write(reinterpret_cast<char const *>(&t), sizeof(t));
		WriteShortString(os, name_->first);
		if (semantic_)
		{
			WriteShortString(os, semantic_->first);
		}
		else
		{
			uint8_t len = 0;
			os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		}

		if (array_size_)
		{
			WriteShortString(os, *array_size_);
		}
		else
		{
			uint8_t len = 0;
			os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		}
		StreamOutVariable(os, *var_);

		uint8_t num_anno;
		if (annotations_)
		{
			num_anno = static_cast<uint8_t>(annotations_->size());
		}
		else
		{
			num_anno = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_anno), sizeof(num_anno));
		for (uint32_t i = 0; i < num_anno; ++ i)
		{
			(*annotations_)[i]->StreamOut(os);
		}
	}
#endif

	std::unique_ptr<RenderEffectParameter> RenderEffectParameter::Clone()
	{
		std::unique_ptr<RenderEffectParameter> ret = MakeUniquePtr<RenderEffectParameter>();

		ret->name_ = name_;
		ret->semantic_ = semantic_;

		ret->type_ = type_;
		ret->var_ = var_->Clone();
		ret->array_size_ = array_size_;

		ret->annotations_ = annotations_;

		return ret;
	}

	std::string const & RenderEffectParameter::Semantic() const
	{
		if (this->HasSemantic())
		{
			return semantic_->first;
		}
		else
		{
			static std::string empty("");
			return empty;
		}
	}

	size_t RenderEffectParameter::SemanticHash() const
	{
		return this->HasSemantic() ? semantic_->second : 0;
	}

	void RenderEffectParameter::BindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index, uint32_t offset, uint32_t stride)
	{
		var_->BindToCBuffer(effect, cbuff_index, offset, stride);
	}

	void RenderEffectParameter::RebindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index)
	{
		var_->RebindToCBuffer(effect, cbuff_index);
	}


#if KLAYGE_IS_DEV_PLATFORM
	void RenderShaderFragment::Load(XMLNode const& node)
	{
		stage_ = ShaderStage::NumStages;
		XMLAttributePtr attr = node.Attrib("type");
		if (attr)
		{
			std::string_view const type_str = attr->ValueString();
			size_t const type_str_hash = HashRange(type_str.begin(), type_str.end());
			if (CT_HASH("vertex_shader") == type_str_hash)
			{
				stage_ = ShaderStage::Vertex;
			}
			else if (CT_HASH("pixel_shader") == type_str_hash)
			{
				stage_ = ShaderStage::Pixel;
			}
			else if (CT_HASH("geometry_shader") == type_str_hash)
			{
				stage_ = ShaderStage::Geometry;
			}
			else if (CT_HASH("compute_shader") == type_str_hash)
			{
				stage_ = ShaderStage::Compute;
			}
			else if (CT_HASH("hull_shader") == type_str_hash)
			{
				stage_ = ShaderStage::Hull;
			}
			else
			{
				BOOST_ASSERT(CT_HASH("domain_shader") == type_str_hash);
				stage_ = ShaderStage::Domain;
			}
		}
		
		ver_ = ShaderModel(0, 0);
		attr = node.Attrib("major_version");
		if (attr)
		{
			uint8_t minor_ver = 0;
			XMLAttributePtr minor_attr = node.Attrib("minor_version");
			if (minor_attr)
			{
				minor_ver = static_cast<uint8_t>(minor_attr->ValueInt());
			}
			ver_ = ShaderModel(static_cast<uint8_t>(attr->ValueInt()), minor_ver);
		}
		else
		{
			attr = node.Attrib("version");
			if (attr)
			{
				ver_ = ShaderModel(static_cast<uint8_t>(attr->ValueInt()), 0);
			}
		}

		for (XMLNodePtr shader_text_node = node.FirstNode(); shader_text_node; shader_text_node = shader_text_node->NextSibling())
		{
			if ((XNT_Comment == shader_text_node->Type()) || (XNT_CData == shader_text_node->Type()))
			{
				str_ += std::string(shader_text_node->ValueString());
			}
		}
	}
#endif

	void RenderShaderFragment::StreamIn(ResIdentifier& res)
	{
		uint32_t tmp;
		res.read(&tmp, sizeof(tmp));
		stage_ = static_cast<ShaderStage>(LE2Native(tmp));
		res.read(&ver_, sizeof(ver_));

		uint32_t len;
		res.read(&len, sizeof(len));
		len = LE2Native(len);
		str_.resize(len);
		res.read(&str_[0], len * sizeof(str_[0]));
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderShaderFragment::StreamOut(std::ostream& os) const
	{
		uint32_t tmp;
		tmp = Native2LE(static_cast<uint32_t>(stage_));
		os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		os.write(reinterpret_cast<char const *>(&ver_), sizeof(ver_));

		uint32_t len = static_cast<uint32_t>(str_.size());
		tmp = Native2LE(len);
		os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		os.write(&str_[0], len * sizeof(str_[0]));
	}
#endif


#if KLAYGE_IS_DEV_PLATFORM
	void RenderShaderGraphNode::Load(XMLNode const& node)
	{
		XMLAttributePtr attr = node.Attrib("name");
		BOOST_ASSERT(attr);

		if (!name_.empty())
		{
			BOOST_ASSERT(name_ == std::string(attr->ValueString()));
		}
		else
		{
			name_ = std::string(attr->ValueString());
			name_hash_ = HashRange(name_.begin(), name_.end());

			attr = node.Attrib("return");
			if (attr)
			{
				return_type_ = std::string(attr->ValueString());
			}
			else
			{
				return_type_ = "void";
			}

			for (XMLNodePtr param_node = node.FirstNode(); param_node; param_node = param_node->NextSibling())
			{
				XMLAttributePtr type_attr = param_node->Attrib("type");
				XMLAttributePtr name_attr = param_node->Attrib("name");
				BOOST_ASSERT(type_attr);
				BOOST_ASSERT(name_attr);

				params_.emplace_back(type_attr->ValueString(), name_attr->ValueString());
			}
		}

		attr = node.Attrib("impl");
		if (attr)
		{
			impl_ = std::string(attr->ValueString());
		}
	}
#endif

	void RenderShaderGraphNode::StreamIn(ResIdentifier& res)
	{
		name_ = ReadShortString(res);
		name_hash_ = HashRange(name_.begin(), name_.end());

		return_type_ = ReadShortString(res);
		impl_ = ReadShortString(res);

		uint8_t len;
		res.read(&len, sizeof(len));
		params_.resize(len);
		for (uint32_t i = 0; i < len; ++ i)
		{
			params_.emplace_back(ReadShortString(res), ReadShortString(res));
		}
	}

#if KLAYGE_IS_DEV_PLATFORM
	void RenderShaderGraphNode::StreamOut(std::ostream& os) const
	{
		WriteShortString(os, name_);
		WriteShortString(os, return_type_);
		WriteShortString(os, impl_);

		uint8_t len = static_cast<uint8_t>(params_.size());
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		for (uint32_t i = 0; i < len; ++ i)
		{
			WriteShortString(os, params_[i].first);
			WriteShortString(os, params_[i].second);
		}
	}
#endif

#if KLAYGE_IS_DEV_PLATFORM
	std::string RenderShaderGraphNode::GenDeclarationCode() const
	{
		std::string ret;

		ret += return_type_;
		ret += ' ';
		ret += name_;
		ret += '(';
		for (size_t i = 0; i < params_.size(); ++ i)
		{
			auto const & param = params_[i];

			ret += param.first;
			ret += ' ';
			ret += param.second;

			if (i != params_.size() - 1)
			{
				ret += ", ";
			}
		}
		ret += ");\n";

		return ret;
	}

	std::string RenderShaderGraphNode::GenDefinitionCode() const
	{
		std::string ret;

		ret += return_type_;
		ret += ' ';
		ret += name_;
		ret += '(';
		for (size_t i = 0; i < params_.size(); ++ i)
		{
			auto const & param = params_[i];

			ret += param.first;
			ret += ' ';
			ret += param.second;

			if (i != params_.size() - 1)
			{
				ret += ", ";
			}
		}
		ret += ")\n";
		ret += "{\n";
		ret += "\t";
		if (return_type_ != "void")
		{
			ret += "return ";
		}
		ret += impl_;
		ret += '(';
		for (size_t i = 0; i < params_.size(); ++ i)
		{
			auto const & param = params_[i];

			ret += param.second;

			if (i != params_.size() - 1)
			{
				ret += ", ";
			}
		}
		ret += ");\n";
		ret += "}\n\n";

		return ret;
	}
#endif


	RenderVariable::RenderVariable()
	{
	}

	RenderVariable::~RenderVariable()
	{
	}

	RenderVariable& RenderVariable::operator=(bool const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint32_t const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int32_t const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint2 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint3 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint4 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int2 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int3 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int4 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float2 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float3 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float4 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float4x4 const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(TexturePtr const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(ShaderResourceViewPtr const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(UnorderedAccessViewPtr const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(SamplerStateObjectPtr const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::string const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(ShaderDesc const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<bool> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint32_t> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int32_t> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint2> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint3> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint4> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int2> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int3> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int4> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float2> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float3> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float4> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float4x4> const & /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<bool const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<uint32_t const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<int32_t const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<float const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<uint2 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<uint3 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<uint4 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<int2 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<int3 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<int4 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<float2 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<float3 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<float4 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<float4x4 const> /*value*/)
	{
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint8_t> const& value)
	{
		KFL_UNUSED(value);
		KFL_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::span<uint8_t const> value)
	{
		KFL_UNUSED(value);
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(bool& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(uint32_t& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(int32_t& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(float& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(uint2& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(uint3& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(uint4& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(int2& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(int3& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(int4& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(float2& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(float3& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(float4& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(float4x4& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(TexturePtr& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(ShaderResourceViewPtr& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(UnorderedAccessViewPtr& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(SamplerStateObjectPtr& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::string& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(ShaderDesc& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<bool>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<uint32_t>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<int32_t>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<float>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<uint2>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<uint3>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<uint4>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<int2>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<int3>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<int4>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<float2>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<float3>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<float4>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<float4x4>& /*value*/) const
	{
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::Value(std::vector<uint8_t>& value) const
	{
		KFL_UNUSED(value);
		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::BindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index, uint32_t offset, uint32_t stride)
	{
		KFL_UNUSED(effect);
		KFL_UNUSED(cbuff_index);
		KFL_UNUSED(offset);
		KFL_UNUSED(stride);

		KFL_UNREACHABLE("Can't be called");
	}

	void RenderVariable::RebindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index)
	{
		KFL_UNUSED(effect);
		KFL_UNUSED(cbuff_index);
		
		KFL_UNREACHABLE("Can't be called");
	}


	RenderEffectPtr SyncLoadRenderEffect(std::string_view effect_name)
	{
		return ResLoader::Instance().SyncQueryT<RenderEffect>(MakeSharedPtr<EffectLoadingDesc>(MakeSpan<1>(std::string(effect_name))));
	}

	RenderEffectPtr SyncLoadRenderEffects(std::span<std::string const> effect_names)
	{
		return ResLoader::Instance().SyncQueryT<RenderEffect>(MakeSharedPtr<EffectLoadingDesc>(effect_names));
	}

	RenderEffectPtr ASyncLoadRenderEffect(std::string_view effect_name)
	{
		return ResLoader::Instance().ASyncQueryT<RenderEffect>(MakeSharedPtr<EffectLoadingDesc>(MakeSpan<1>(std::string(effect_name))));
	}

	RenderEffectPtr ASyncLoadRenderEffects(std::span<std::string const> effect_names)
	{
		return ResLoader::Instance().ASyncQueryT<RenderEffect>(MakeSharedPtr<EffectLoadingDesc>(effect_names));
	}
}
