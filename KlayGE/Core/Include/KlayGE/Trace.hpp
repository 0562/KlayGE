// Trace.hpp
// KlayGE ������ ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ���ν��� (2003.11.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TRACE_HPP
#define _TRACE_HPP

#pragma KLAYGE_ONCE

#include <iostream>

namespace KlayGE
{
	// ���Ը��ٺ���ִ�е���
	// �����ϴ����ֻ�ڵ��Ե�ʱ����һ��
	class Trace
	{
	public:
		Trace(char* const func, int line = 0, char* const file = NULL)
			: func_(func), line_(line), file_(file)
		{
#ifdef KLAYGE_DEBUG
			std::clog << "Calling " << func_ << " in file " << file_ << " on line " << line_ << std::endl;
#endif
		}

		~Trace()
		{
#ifdef KLAYGE_DEBUG
			std::clog << "Leaving " << func_ << " in file " << file_ << " on line " << line_ << std::endl;
#endif
		}

	private:
		char* const func_;
		int line_;
		char* const file_;
	};
}

#endif		// _TRACE_HPP
