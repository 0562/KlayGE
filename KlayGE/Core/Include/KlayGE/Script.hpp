// Script.hpp
// KlayGE �ű������� ͷ�ļ�
// Ver 2.1.3
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.klayge.org
//
// 2.1.3
// ��������tupleΪ������Call (2004.9.15)
//
// �޸ļ�¼
////////////////////////////////////////////////////////////////////////////

#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <Python.h>
#include <vector>
#include <string>

#include <boost/noncopyable.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 6011)
#endif
#include <boost/shared_ptr.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <boost/tuple/tuple.hpp>

namespace KlayGE
{
	typedef boost::shared_ptr<PyObject> PyObjectPtr;

	// PyObjectָ��
	/////////////////////////////////////////////////////////////////////////////////
	KLAYGE_CORE_API PyObjectPtr MakePyObjectPtr(PyObject* p);

	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(std::string const & t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(char* t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(wchar_t* t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(int8_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(int16_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(int32_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(int64_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(uint8_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(uint16_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(uint32_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(uint64_t t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(double t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(float t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(PyObject* t);
	KLAYGE_CORE_API PyObjectPtr CppType2PyObjectPtr(PyObjectPtr const & t);

	// ��һ��.py����ģ��
	/////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API ScriptModule
	{
	private:
		template <typename TupleType>
		std::vector<PyObjectPtr> Tuple2Vector(TupleType const & t)
		{
			std::vector<PyObjectPtr> ret;
			ret.push_back(CppType2PyObjectPtr(boost::tuples::get<0>(t)));

			std::vector<PyObjectPtr> tail(Tuple2Vector(t.get_tail()));
			ret.insert(ret.end(), tail.begin(), tail.end());

			return ret;
		}

	public:
		ScriptModule();
		explicit ScriptModule(std::string const & name);
		~ScriptModule();

		PyObjectPtr Value(std::string const & name);

		template <typename TupleType>
		PyObjectPtr Call(std::string const & func_name, TupleType const & t)
		{
			std::vector<PyObjectPtr> v(Tuple2Vector(t));
			return this->Call(func_name, &v.front(), &v.back() + 1);
		}

		PyObjectPtr Call(std::string const & func_name, PyObjectPtr* first, PyObjectPtr* last);

		PyObjectPtr RunString(std::string const & script);

	private:
		PyObjectPtr module_;
		PyObjectPtr dict_;
	};

	template <>
	inline std::vector<PyObjectPtr>
	ScriptModule::Tuple2Vector<boost::tuples::null_type>(boost::tuples::null_type const & /*t*/)
	{
		return std::vector<PyObjectPtr>();
	}

	// ע��һ��������Python�е��õ�ģ��
	/////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API RegisterModule
	{
	public:
		typedef PyObject *(*PyCallback)(PyObject*, PyObject*);

		RegisterModule(std::string const & name)
			: module_name_(name)
		{
		}

		std::string const & Name() const
		{
			return module_name_;
		}

		void AddMethod(std::string const & method_name, PyCallback method);
		void Regiter();

	protected:
		std::string module_name_;

		std::vector<PyMethodDef>	methods_;
		std::vector<std::string>	method_names_;
	};


	// ʵ�ֽű�����Ĺ���
	/////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API ScriptEngine : boost::noncopyable
	{
	public:
		ScriptEngine();
		~ScriptEngine();

		// ���ַ������нű�
		void RunString(std::string const & script);
	};
}

#endif		// _SCRIPT_HPP
