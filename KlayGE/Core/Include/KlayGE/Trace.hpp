// Trace.hpp
// KlayGE ������ ͷ�ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://www.klayge.org
//
// 2.2.0
// ���ν��� (2003.11.9)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TRACE_HPP
#define _TRACE_HPP

#pragma once

#include <KlayGE/Log.hpp>

namespace KlayGE
{
	// ���Ը��ٺ���ִ�е���
	// �����ϴ����ֻ�ڵ��Ե�ʱ����һ��
	class Trace
	{
	public:
		Trace(char const * func, int line = 0, char const * file = nullptr)
			: func_(func), line_(line), file_(file)
		{
#ifdef KLAYGE_DEBUG
			LOG_Info("Enter %s in file %s (line %d)", func_, file_ != nullptr ? file_ : "", line_);
#endif
		}

		~Trace()
		{
#ifdef KLAYGE_DEBUG
			LOG_Info("Leave %s in file %s (line %d)", func_, file_ != nullptr ? file_ : "", line_);
#endif
		}

	private:
		char const * func_;
		int line_;
		char const * file_;
	};
}

#endif		// _TRACE_HPP
