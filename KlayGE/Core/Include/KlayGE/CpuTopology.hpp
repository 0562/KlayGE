// CpuTopology.hpp
// KlayGE CPU����� ʵ���ļ�
// Ver 3.7.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.7.0
// ���ν��� (2008.2.16)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _CPU_TOPOLOGY_HPP
#define _CPU_TOPOLOGY_HPP

namespace KlayGE
{
	class CpuTopology
	{
	public:
		explicit CpuTopology(bool force_cpuid = false);

		int NumHWThreads() const;
		int NumCores() const;
	};
}

#endif  // _CPU_TOPOLOGY_HPP
