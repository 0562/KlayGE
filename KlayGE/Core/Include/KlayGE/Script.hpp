// Script.hpp
// KlayGE �ű������� ͷ�ļ�
// Ver 1.2.8.11
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
//
// �޸ļ�¼
////////////////////////////////////////////////////////////////////////////

#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP

#include <python.h>
#include <vector>
#include <string>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

#pragma comment(lib, "KlayGE_Core.lib")

namespace KlayGE
{
	void PyObjDeleter(PyObject* p)
	{
		if (p != 0)
		{
			Py_DECREF(p);
		}
	}

	// PyObjectָ��
	/////////////////////////////////////////////////////////////////////////////////
	boost::shared_ptr<PyObject> MakePyObjectPtr(PyObject* p)
	{
		return boost::shared_ptr<PyObject>(p, PyObjDeleter);
	};

	// ��һ��.py����ģ��
	/////////////////////////////////////////////////////////////////////////////////
	class ScriptModule
	{
	public:
		ScriptModule(std::string const & name)
		{
			module_	= MakePyObjectPtr(PyImport_Import(PyString_FromString(name.c_str())));
			dict_	= MakePyObjectPtr(PyModule_GetDict(module_.get()));
		}

		boost::shared_ptr<PyObject> Value(std::string const & name)
		{
			return MakePyObjectPtr(PyDict_GetItemString(dict_.get(), name.c_str()));
		}

		template <typename ForwardIterator>
		boost::shared_ptr<PyObject> Call(std::string const & funcName, ForwardIterator first, ForwardIterator last)
		{
			boost::shared_ptr<PyObject> func(MakePyObjectPtr(this->Value(funcName)));
			boost::shared_ptr<PyObject> args(MakePyObjectPtr(PyTuple_New(last - first)));

			for (ForwardIterator iter = first; iter != last; ++ iter)
			{
				PyTuple_SetItem(args.get(), iter - first, iter->get());
			}

			return MakePyObjectPtr(PyObject_CallObject(func.get(), args.get()));
		}

	private:
		boost::shared_ptr<PyObject> module_;
		boost::shared_ptr<PyObject> dict_;
	};

	#define BEGIN_REG() 	methods.clear() 
	#define ADD(x, y) 		AddMethod(x,y) 
	#define END_REG() \
				do\
				{\
					PyMethodDef __tmp = {NULL, NULL, 0, NULL};\
					methods.push_back(__tmp);\
				} while(0)

	// ע��һ��������Python�е��õ�ģ��
	/////////////////////////////////////////////////////////////////////////////////
	class RegisterModule
	{
	public:
		typedef PyObject *(*PyCallback)(PyObject*, PyObject*);

		RegisterModule(std::string const & name)
			: moduleName_(name)
			{ }

		std::string const & Name() const
			{ return moduleName_;}

		void AddMethod(std::string const & methodName, PyCallback method);
		void Regiter();

	protected:
		std::string moduleName_;

		std::vector<PyMethodDef>	methods_;
		std::vector<std::string>	methodNames_;
	};


	// ʵ�ֽű�����Ĺ���
	/////////////////////////////////////////////////////////////////////////////////
	class ScriptEngine : boost::noncopyable
	{
	public:
		ScriptEngine();
		~ScriptEngine();

		// ���ַ������нű�
		void ExecString(std::string const & script);
	};
}

#endif		// _SCRIPT_HPP
