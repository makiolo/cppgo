#include <iostream>
#include <assert.h>
#include <tuple>
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
		py::object result = py::exec_file("hello.py", globals, globals);
		py::dict output = py::extract<py::dict>(globals);
		output["func1"](1, 2, 3, 5.0f, "hiiii from c++");
	}
	catch (const py::error_already_set& /*e*/)
	{
		PyErr_Print();
	}
}

