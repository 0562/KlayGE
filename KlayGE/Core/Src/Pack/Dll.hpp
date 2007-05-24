// Dll.hpp
// KlayGE ���ϵͳDLL������ ͷ�ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ���ν��� (2007.5.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DLL_HPP
#define _DLL_HPP

#include <string>

namespace KlayGE
{
	class DllLoader
	{
	public:
		DllLoader();
		~DllLoader();

		void Load(std::string const & dll_name);
		void Free();

		void* GetProcAddress(std::string const & proc_name);

	private:
		void* dll_handle_;
	};
}

#endif		// _DLL_HPP
