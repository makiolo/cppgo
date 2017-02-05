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

struct foo_python_API BasePythonic : foo::Base
{
public:
	explicit BasePythonic(const std::string& name, int q)
		: foo::Base(name, q)
	{
		// init
		py::object mainmodule = py::import("__main__");
		py::object globals = mainmodule.attr("__dict__");
		// instance
		py::object class_derived = globals[BasePythonic::KEY()];
		_instance = class_derived(name, q);
	}
	virtual ~BasePythonic() = default;

	constexpr static char const* KEY() { return "PythonDerived1"; }
	virtual const std::string& getKEY() const
	{
		// if (override n = this->get_override("name"))
		//     return n();
		auto result = _instance.attr("name")();
		static std::string name = py::extract<std::string>(py::str(result))();
		return name;
	}
protected:
	py::object _instance;
};

void init_factory()
{
	load_library fooA("fooA");
	load_library fooB("fooB");

	// register python implementations
	py::object mainmodule = py::import("__main__");
	py::object globals = mainmodule.attr("__dict__");
	py::exec_file("fooA_python.py", globals, globals);

}


#endif

