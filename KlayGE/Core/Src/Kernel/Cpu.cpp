// CPU.cpp
// KlayGE CPU��� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.28)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/CommFuncs.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/Engine.hpp>

#include <sstream>

#include <KlayGE/Cpu.hpp>

namespace
{
	using namespace KlayGE;

	// ��ȡCPUID��Ϣ
	/////////////////////////////////////////////////////////////////////////////////
	void CpuidInfo(U32 idLevels, U32& regEAX, U32& regEBX, U32& regECX, U32& regEDX)
	{
		U32 a, b, c, d;

		_asm
		{
			mov eax, idLevels
			cpuid
			mov a, eax
			mov b, ebx
			mov c, ecx
			mov d, edx
		}

		regEAX = a;
		regEBX = b;
		regECX = c;
		regEDX = d;
	}

	// ���CPUIDָ���Ƿ����
	/////////////////////////////////////////////////////////////////////////////////
	bool ChkCpuidEnable()
	{
		_asm
		{
			pushfd							// ��ȡԭ����EFLAGS
			pop		ebx
			mov		ecx, ebx
			xor		ebx, 0x200000			// ��ת EFLAGS ��� ID λ
			push	ebx						// �����µ� EFLAGS

			popfd							// �������ڵ� EFLAGS ֵ
			pushfd							// ��ȡ�µ� EFLAGS
			pop		ebx						// ���µ� EFLAGS ���� ebx
			xor		ebx, ecx				// �Ƚ� ID λ
			jnz		SUPPORT

			xor		eax, eax				// ���� false

SUPPORT:
			mov		eax, 1
		}
	}
}

namespace KlayGE
{
	// ���캯��
	CPUInfo::CPUInfo()
	{
		memset(vendor_, 0, sizeof(vendor_));
		memset(vendorEx_, 0, sizeof(vendorEx_));
		memset(serialNumber_, 0, sizeof(serialNumber_));

		level_ = levelEx_ = 0;
		cpuID_ = cpuIDEx_ = 0;
		feature_ = featureEx_ = 0;
		extendedLevels_ = false;
	}

	// ���CPU
	/////////////////////////////////////////////////////////////////////////////////
	void CPUInfo::CheckCpu() 
	{
		U32 regEAX, regEBX, regECX, regEDX;
		U32 vendorTemp[4];

		if (ChkCpuidEnable())
		{
			CpuidInfo(0x80000000, regEAX, regEBX, regECX, regEDX);

			if (regEAX & 0x80000000)
			{
				// ֧����չ����
				extendedLevels_ = true;

				levelEx_ = regEAX;

				CpuidInfo(0x80000001, regEAX, regEBX, regECX, regEDX);
				cpuIDEx_		= regEAX;
				featureEx_	= regEDX;

				// ȡ����������չ�ַ���
				CpuidInfo(0x80000002, regEAX, regEBX, regECX, regEDX);
				vendorTemp[0] = regEAX;
				vendorTemp[1] = regEBX;
				vendorTemp[2] = regECX;
				vendorTemp[3] = regEDX;
				memcpy(vendorEx_, vendorTemp, 16);

				CpuidInfo(0x80000003, regEAX, regEBX, regECX, regEDX);
				vendorTemp[0] = regEAX;
				vendorTemp[1] = regEBX;
				vendorTemp[2] = regECX;
				vendorTemp[3] = regEDX;
				memcpy(&vendorEx_[16], vendorTemp, 16);

				CpuidInfo(0x80000004, regEAX, regEBX, regECX, regEDX);
				vendorTemp[0] = regEAX;
				vendorTemp[1] = regEBX;
				vendorTemp[2] = regECX;
				vendorTemp[3] = regEDX;
				memcpy(&vendorEx_[32], vendorTemp, 16);
			}

			CpuidInfo(0, regEAX, regEBX, regECX, regEDX);
			level_ = regEAX;

			// ȡ���������ַ���
			vendorTemp[0] = regEBX;
			vendorTemp[1] = regEDX;
			vendorTemp[2] = regECX;
			memcpy(vendor_, vendorTemp, 12);

			CpuidInfo(1, regEAX, regEBX, regECX, regEDX);
			cpuID_		= regEAX;
			feature_	= regEDX;

			if (feature_ & CPUID_STD_PSN)
			{
				std::basic_ostringstream<char, std::char_traits<char>, alloc<char> > sstream;

				// ��ȡPIII����֧�ֵ����к�
				CpuidInfo(3, regEAX, regEBX, regECX, regEDX);
				sstream.width(4);
				sstream << std::ios_base::hex << cpuID_ << regEDX << regECX;

				sstream.str().copy(serialNumber_, sizeof(serialNumber_));
			}
		}
	}

	// ����CPUƵ��
	/////////////////////////////////////////////////////////////////////////////////
	U32 CPUInfo::Frequency() const
	{
		if (this->Feature() & CPUID_STD_TSC)
		{
			U32 regEAX, regEDX;

			const float lastTime(Timer::Instance().AppTime());
			_asm
			{
				rdtsc					// ��ȡϵͳ���������ڵ�ʱ������
				mov		regEAX, eax		// ��32λ������eax��
				mov		regEDX, edx		// ��32λ������edx��
			}
			Sleep(50);
			_asm
			{
				rdtsc
				sub		eax, regEAX
				sbb		edx, regEDX
				mov		regEAX, eax
				mov		regEDX, edx
			}

			const float elapsedTime(Timer::Instance().AppTime() - lastTime);
			return static_cast<U32>(static_cast<float>((regEDX << 32) + regEAX) / elapsedTime);
		}

		return 0;
	}
}
