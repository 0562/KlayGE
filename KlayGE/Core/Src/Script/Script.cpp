// Script.cpp
// KlayGE �ű������� ʵ���ļ�
// Ver 1.2.8.11
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.9
// �����˵�������Close���������� (2002.10.11)
//
// 1.2.8.10
// ��string�������ַ���ָ�� (2002.10.27)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>

#include <cassert>

#include <KlayGE/Script.hpp>

#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "python24d.lib")
#else
	#pragma comment(lib, "python24.lib")
#endif

namespace KlayGE
{
	// ��ģ�����������һ������
	/////////////////////////////////////////////////////////////////////////////////
	void RegisterModule::AddMethod(std::string const & methodName, PyCallback method)
	{
		methodNames_.push_back(methodName);

		PyMethodDef def = { const_cast<char*>(methodName.c_str()), method, METH_VARARGS, NULL };
		methods_.push_back(def);
	}

	// ��ģ��ע���Python
	/////////////////////////////////////////////////////////////////////////////////
	void RegisterModule::Regiter()
	{
		Py_InitModule(const_cast<char*>(moduleName_.c_str()), &methods_[0]);
	}


	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	ScriptEngine::ScriptEngine()
	{
		Py_Initialize();
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	ScriptEngine::~ScriptEngine()
	{
		Py_Finalize();
	}

	// ���ַ������нű�
	void ScriptEngine::ExecString(std::string const & script)
	{
		PyRun_SimpleString(script.c_str());
	}
}
