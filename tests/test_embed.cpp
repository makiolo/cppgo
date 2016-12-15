#include <iostream>
#include <assert.h>
#include <tuple>
#include <teelogging/teelogging.h>
#include <boost/python.hpp>
#include <gtest/gtest.h>

using namespace boost::python;
namespace py = boost::python;

// https://wiki.python.org/moin/boost.python/EmbeddingPython

class PythonTest : testing::Test { };

struct PythonEmbed
{
	PythonEmbed()
	{
		LOGI("creating python ...");
		program = Py_DecodeLocale("python_embed", NULL);
		if (program == NULL) {
			fprintf(stderr, "Fatal error: cannot decode\n");
			exit(1);
		}
		Py_SetProgramName(program);
		Py_Initialize();
	}

	~PythonEmbed()
	{
		LOGI("destroying python ...");
		Py_Finalize();
		PyMem_RawFree(program);
	}

	wchar_t *program;
};

TEST(PythonTest, Test1)
{
	PythonEmbed python;
	try
	{
		py::object mainmodule = py::import("__main__");
		py::object globals = mainmodule.attr("__dict__");
		py::exec_file("hello.py", globals, globals);

		// instance class and call method
		LOGI("call PythonDerived.name() class in hello.py");
		py::object class_derived = globals["PythonDerived"];
		py::object instance_derived = class_derived("from c++", 9876);
		py::object result = instance_derived.attr("name")();
		std::cout << "PythonDerived.name() = " << py::extract<std::string>(py::str(result))() << std::endl;

		// call static method
		LOGI("call func1() in hello.py");
		globals["func1"](1, 2, 3, 5.0f, "hiiii from c++");
	}
	catch (const py::error_already_set& /*e*/)
	{
		LOGE("exception in boost::python ...");
		PyErr_Print();
	}
}

