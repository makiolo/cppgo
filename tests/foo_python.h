#ifndef _FOO_PYTHON_H_
#define _FOO_PYTHON_H_

#include <iostream>
#include <assert.h>
#include <tuple>
#include <teelogging/teelogging.h>
#include <boost/python.hpp>
#include <gtest/gtest.h>
#include <dp14/factory.h>
#include "foo.h"

using namespace boost::python;
namespace py = boost::python;

#ifdef _WIN32
    #ifdef foo_python_EXPORTS
        #define foo_python_API __declspec(dllexport)
    #else
		#define foo_python_API
    #endif
#else
    #ifdef foo_python_EXPORTS
		#define foo_python_API __attribute__((visibility("default")))
    #else
        #define foo_python_API
    #endif
#endif

struct foo_python_API BasePythonInterface : foo::Base, py::wrapper<foo::Base>
{
	explicit BasePythonInterface(const std::string& name, int q) : foo::Base(name, q)
	{
		std::cout << "contructor BasePythonInterface" << std::endl;
	}

	virtual const std::string& getKEY() const
	{
		if(override n = this->get_override("name"))
			return n();
		return this->foo::Base::getKEY();
	}

	virtual const std::string& getKEY_default() const
	{
		return this->foo::Base::getKEY();
	}
};

struct foo_python_API BasePythonImpl : foo::Base, py::object
{
public:
	explicit BasePythonImpl(const std::string& name, int q) 
		: foo::Base(name, q)
		, py::object( py::import("__main__").attr("__dict__")[name](name, q) )
	{
		;
	}
	virtual ~BasePythonImpl() = default;

	constexpr static char const* KEY() { return "Python"; }
	virtual const std::string& getKEY() const
	{
		auto it = _cache_strs.find("name");
		if(it == _cache_strs.end())
			_cache_strs["name"] = py::extract<std::string>( attr("name")() )( );
		return _cache_strs["name"];
	}
protected:
	mutable std::map<std::string, std::string> _cache_strs;
};

bool init_factory()
{
	// register c++ implementations
	load_library fooA("fooA");
	load_library fooB("fooB");

	// register python implementations
	py::object mainmodule = py::import("__main__");
	py::object globals = mainmodule.attr("__dict__");
	py::exec_file("fooA_python.py", globals, globals);
	py::exec_file("fooB_python.py", globals, globals);
	return true;
}


#endif

