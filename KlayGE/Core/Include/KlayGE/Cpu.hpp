// CPU.hpp
// KlayGE CPU��� ͷ�ļ�
// Ver 1.3.8.1
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.8
// ��inline���������� (2002.9.28)
//
// 1.2.8.9
// ������GetSerialNumber���޸���GetVendor��GetVendorEx�ķ������� (2002.10.23)
// �޸����ַ�����Ĵ�С (2002.10.23)
//
// 1.2.8.10
// ��string�����ַ���ָ�� (2002.10.27)
//
// 1.3.8.1
// �����˶Գ��̵߳�ʶ�� (2002.12.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _CPU_HPP
#define _CPU_HPP

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	// ������Ϣ����Intel��AMD

	enum CpuIDStd
	{
		CPUID_STD_FPU			= 1UL << 0,		// ���㴦���� 0
		CPUID_STD_VME			= 1UL << 1,		// �����ģʽ 1
		CPUID_STD_DE			= 1UL << 2,		// ������ 2
		CPUID_STD_PSE			= 1UL << 3,		// ҳ��С��չ 3
		CPUID_STD_TSC			= 1UL << 4,		// ʱ�����ڼ����� 4
		CPUID_STD_MSR			= 1UL << 5,		// RDMSR / WRMSR ָ�� 5
		CPUID_STD_PAE			= 1UL << 6,		// �����ַ��չ 6
		CPUID_STD_MCE			= 1UL << 7,		// Machine Check Exception 7
		CPUID_STD_CX8			= 1UL << 8,		// CMPXCHG8B ָ�� 8
		CPUID_STD_APIC			= 1UL << 9,		// �ں� APIC Ӳ�� 9
		CPUID_STD_SEP			= 1UL << 11,	// SYSENTER / SYSEXIT ָ�� 11
		CPUID_STD_MTRR			= 1UL << 12,	// Memory Type Range Registers 12
		CPUID_STD_PGF			= 1UL << 13,	// Page Global Enable 13
		CPUID_STD_MCA			= 1UL << 14,	// Machine Check Architecture 14
		CPUID_STD_CMOV			= 1UL << 15,	// �����ƶ�ָ�� CMOV 15
		CPUID_STD_PAT			= 1UL << 16,	// ҳ���Ա� 16
		CPUID_STD_PSE36			= 1UL << 17,	// 36λҳ��С��չ 17
		CPUID_STD_PSN			= 1UL << 18,	// ���к� 18
		CPUID_STD_CLFSH			= 1UL << 19,	// CLFLUSH ָ�� 19
		CPUID_STD_DTS			= 1UL << 21,	// Debug Trace Store 21
		CPUID_STD_ACPI			= 1UL << 22,	// ACPI ֧�� 22
		CPUID_STD_MMX			= 1UL << 23,	// MMX ָ� 23
		CPUID_STD_FXSR			= 1UL << 24,	// ���ٸ��㱣��ָ� FXSAVE / FXRSTOR 24
		CPUID_STD_SSE			= 1UL << 25,	// Streaming SIMD Extention 25
		CPUID_STD_SSE2			= 1UL << 26,	// Streaming SIMD Extention2 26
		CPUID_STD_SSP			= 1UL << 27,	// Self-Snoop 27
		CPUID_STD_HTT			= 1UL << 28,	// ���̼߳��� 28
		CPUID_STD_TM			= 1UL << 29,	// Thermal Monitor Support 29
		CPUID_STD_IA64			= 1UL << 30,	// IA-64 30
		CPUID_STD_SBF			= 1UL << 31,	// Signal Break on FERR
	};

	enum CpuIDExt
	{
		CPUID_EXT_FPU			= 1UL << 0,		// ���㴦���� 0
		CPUID_EXT_VME			= 1UL << 1,		// �����ģʽ 1
		CPUID_EXT_DE			= 1UL << 2,		// ������ 2
		CPUID_EXT_PSE			= 1UL << 3,		// ҳ��С��չ 3
		CPUID_EXT_TSC			= 1UL << 4,		// ʱ�����ڼ����� 4
		CPUID_EXT_MSR			= 1UL << 5,		// RDMSR / WRMSR ָ�� 5
		CPUID_EXT_PAE			= 1UL << 6,		// �����ַ��չ 6
		CPUID_EXT_MCE			= 1UL << 7,		// Machine Check Exception 7
		CPUID_EXT_CX8			= 1UL << 8,		// CMPXCHG8B ָ�� 8
		CPUID_EXT_APIC			= 1UL << 9,		// �ں� APIC Ӳ�� 9
		CPUID_EXT_SEP			= 1UL << 11,	// SYSENTER / SYSEXIT ָ�� 11
		CPUID_EXT_MTRR			= 1UL << 12,	// Memory Type Range Registers 12
		CPUID_EXT_PGF			= 1UL << 13,	// Page Global Enable 13
		CPUID_EXT_MCA			= 1UL << 14,	// Machine Check Architecture 14
		CPUID_EXT_CMOV			= 1UL << 15,	// �����ƶ�ָ�� CMOV 15
		CPUID_EXT_PAT			= 1UL << 16,	// ҳ���Ա� 16
		CPUID_EXT_PSE36			= 1UL << 17,	// 36λҳ��С��չ 17
		CPUID_EXT_MP			= 1UL << 19,	// �ദ����֧�� 19
		CPUID_EXT_MMXEX			= 1UL << 22,	// MMX ��չ 22
		CPUID_EXT_MMX			= 1UL << 23,	// MMX ָ� 23
		CPUID_EXT_FXSR			= 1UL << 24,	// ���ٸ��㱣��ָ�ָ�� FXSAVE / FXRSTOR 24
		CPUID_EXT_LM			= 1UL << 29,	// x86-64 ��ģʽ 29
		CPUID_EXT_3DNOWEX		= 1UL << 30,	// 3DNow! ��չ 30
		CPUID_EXT_3DNOW			= 1UL << 31,	// 3DNow! ָ� 31
	};

	class CPUInfo
	{
	public:
		void CheckCpu();
		U32 Frequency() const;

		const char* Vendor() const
			{ return this->vendor_; }
		U32 CpuID() const
			{ return this->cpuID_; }
		U32 Type() const
			{ return (this->CpuID() >> 12) & 0xF; }
		U32 Family() const
			{ return (this->CpuID() >> 8) & 0xF; }
		U32 Model() const
			{ return (this->CpuID() >> 4) & 0xF; }
		U32 Stepping() const
			{ return this->CpuID() & 0xF; }

		const char* VendorEx() const
			{ return this->vendorEx_; }
		U32 CpuIDEx() const
			{ return this->cpuIDEx_; }
		U32 TypeEx() const
			{ return (this->CpuIDEx() >> 12) & 0xF; }
		U32 FamilyEx() const
			{ return (this->CpuIDEx() >> 8) & 0xF; }
		U32 ModelEx() const
			{ return (this->CpuIDEx() >> 4) & 0xF; }
		U32 SteppingEx() const
			{ return this->CpuIDEx() & 0xF; }

		const char* SerialNumber() const
			{ return this->serialNumber_; }

		U32 Feature() const
			{ return this->feature_; }
		U32 FeatureEx() const
			{ return this->featureEx_; }

		bool IsMMXSupport() const
			{ return (this->Feature() & CPUID_STD_MMX) != 0; }
		bool IsSSESupport() const
			{ return (this->Feature() & CPUID_STD_SSE) != 0; }
		bool IsSSE2Support() const
			{ return (this->Feature() & CPUID_STD_SSE2) != 0; }
		bool IsMMXEXSupport() const
			{ return (this->FeatureEx() & CPUID_EXT_MMXEX) != 0; }
		bool Is3DNowSupport() const
			{ return (this->FeatureEx() & CPUID_EXT_3DNOW) != 0; }
		bool Is3DNowEXSupport() const
			{ return (this->FeatureEx() & CPUID_EXT_3DNOWEX) != 0; }

		CPUInfo();

	private:
		char vendor_[17];
		char vendorEx_[49];
		char serialNumber_[14];

		U32 level_;
		U32 levelEx_;
		U32 cpuID_;
		U32 cpuIDEx_;
		U32 feature_;
		U32 featureEx_;

		bool extendedLevels_;
	};
}

#endif		// _CPU_HPP

