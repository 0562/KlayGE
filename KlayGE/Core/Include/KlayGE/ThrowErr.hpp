// ThrowErr.hpp
// KlayGE �׳����� ͷ�ļ�
// Ver 1.3.8.1
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 1.3.8.1
// ���ν���
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _THROWERR_HPP
#define _THROWERR_HPP

#include <string>
#include <exception>

#define NOMINMAX
#include <windows.h>

namespace KlayGE
{
	class Exception : public std::exception
	{
	public:
		Exception(const std::string& errFile, U32 errLine, U32 errCode, const std::string& msg) throw()
			: errFile_(errFile), errLine_(errLine),
				errCode_(errCode), msg_(msg)
			{ }

		const std::string& ErrorFile() const throw()
			{ return errFile_; }
		U32 ErrorLine() const throw()
			{ return errLine_; }

		U32 ErrorCode() const throw()
			{ return errCode_; }
		const char* what() const throw()
			{ return msg_.c_str(); }

	public:
		std::string errFile_;
		U32 errLine_;

		U32 errCode_;
		std::string msg_;
	};
}

#ifndef IID_PPV_ARG
#define IID_PPV_ARG(Type, Expr)		IID_##Type,	reinterpret_cast<void**>(static_cast<Type**>(&Expr))
#endif

#ifdef NDEBUG

// �׳�HRESULT
inline void
THR(HRESULT hr)
	{ throw hr; }

// �׳�HRESULT
inline void
TIF(HRESULT hr)
{
	if (FAILED(hr))
	{
		THR(hr);
	}
}

#else

#define THR(x)			{ throw KlayGE::Exception(__FILE__, __LINE__, x, #x); }

// ������󣬾��׳��������
#define TIF(x)			{ HRESULT _hr = x; if (FAILED(_hr)) { THR(x); } }

#endif

// ����
inline void
Verify(bool x)
{
	if (!x)
	{
		THR(E_FAIL);
	}
}

#endif		// _THROWERR_HPP
