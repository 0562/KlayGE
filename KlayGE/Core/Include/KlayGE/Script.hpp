// Script.hpp
// KlayGE �ű������� ͷ�ļ�
// Ver 2.1.3
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 2.1.3
// ��������tupleΪ������Call (2004.9.15)
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
#include <boost/tuple/tuple.hpp>

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

	typedef boost::shared_ptr<PyObject> PyObjectPtr;

	// PyObjectָ��
	/////////////////////////////////////////////////////////////////////////////////
	PyObjectPtr MakePyObjectPtr(PyObject* p)
	{
		return PyObjectPtr(p, PyObjDeleter);
	};

	template <typename TupleType>
	std::vector<PyObjectPtr> Tuple2Vector(const TupleType& t)
	{
		std::vector<PyObjectPtr> ret;
		ret.push_back(boost::tuples::get<0>(t));

		std::vector<PyObjectPtr> tail(Tuple2Vector(t.get_tail()));
		ret.insert(ret.end(), tail.begin(), tail.end());

		return ret;
	}

	template <>
	std::vector<PyObjectPtr> Tuple2Vector<boost::tuples::null_type>(const boost::tuples::null_type& t)
	{
		return std::vector<PyObjectPtr>();
	}

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

		template <typename TupleType>
		PyObjectPtr Call(std::string const & funcName, const TupleType& t)
		{
			std::vector<PyObjectPtr> v(Tuple2Vector(t));
			return this->Call(funcName, v.begin(), v.end());
		}

		template <typename ForwardIterator>
		PyObjectPtr Call(std::string const & funcName, ForwardIterator first, ForwardIterator last)
		{
			PyObjectPtr func(MakePyObjectPtr(this->Value(funcName)));
			PyObjectPtr args(MakePyObjectPtr(PyTuple_New(last - first)));

			for (ForwardIterator iter = first; iter != last; ++ iter)
			{
				PyTuple_SetItem(args.get(), iter - first, iter->get());
			}

			return MakePyObjectPtr(PyObject_CallObject(func.get(), args.get()));
		}

	private:
		PyObjectPtr module_;
		PyObjectPtr dict_;
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
