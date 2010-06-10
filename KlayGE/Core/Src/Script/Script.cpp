// Script.cpp
// KlayGE �ű������� ʵ���ļ�
// Ver 1.2.8.11
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.klayge.org
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

#include <boost/assert.hpp>

#include <KlayGE/Script.hpp>

#ifdef KLAYGE_COMPILER_MSVC
#ifdef KLAYGE_DEBUG
	#if PY_MAJOR_VERSION >= 3
		#pragma comment(lib, "python31_d.lib")
	#else
		#pragma comment(lib, "python26_d.lib")
	#endif
#else
	#if PY_MAJOR_VERSION >= 3
		#pragma comment(lib, "python31.lib")
	#else
		#pragma comment(lib, "python26.lib")
	#endif
#endif
#endif

namespace KlayGE
{
	ScriptModule::ScriptModule(std::string const & name)
	{
		module_	= MakePyObjectPtr(PyImport_ImportModule(name.c_str()));
		dict_	= MakePyObjectPtr(PyModule_GetDict(module_.get()));
		Py_INCREF(dict_.get());
	}

	ScriptModule::~ScriptModule()
	{
		dict_.reset();
		module_.reset();
	}

	PyObjectPtr ScriptModule::Value(std::string const & name)
	{
		PyObject* p = PyDict_GetItemString(dict_.get(), name.c_str());
		Py_INCREF(p);
		return MakePyObjectPtr(p);
	}

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
#if PY_MAJOR_VERSION >= 3
		static struct PyModuleDef module =
		{
			PyModuleDef_HEAD_INIT,
			const_cast<char*>(moduleName_.c_str()),
			NULL,
			-1,
			&methods_[0]
		};

		PyModule_Create(&module);
#else
		Py_InitModule(const_cast<char*>(moduleName_.c_str()), &methods_[0]);
#endif
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
