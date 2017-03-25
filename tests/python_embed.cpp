#include <iostream>
#include <assert.h>
#include <tuple>
#include <teelogging/teelogging.h>
#include <boost/python.hpp>
#include <gtest/gtest.h>
#include <dp14/factory.h>
#include "foo.h"
// #include "foo_python.h"

using namespace boost::python;
namespace py = boost::python;

class PythonTest : testing::Test { };

// https://wiki.python.org/moin/boost.python/EmbeddingPython
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

bool init_factory()
{
	// register c++ implementations
	static load_library fooA("fooA");
	static load_library fooB("fooB");

	// register python implementations
	py::object mainmodule = py::import("__main__");
	py::object globals = mainmodule.attr("__dict__");
	py::exec_file("fooA_python.py", globals, globals);
	py::exec_file("fooB_python.py", globals, globals);
	return true;
}


TEST(PythonTest, Test1)
{
	PythonEmbed python;
	try
	{
		// reg c++ implementations
		init_factory();

		try
		{
			auto base1 = foo::Base::get_factory().create("A", "create c++ from factory c++", 1);
			LOGI("%s", base1->getKEY().c_str());

			auto base2 = foo::Base::get_factory().create("B", "create c++ from factory c++", 2);
			LOGI("%s", base2->getKEY().c_str());

			auto base3 = foo::Base::get_factory().create("Python", "PythonDerived1", 3);
			LOGI("%s", base3->getKEY().c_str());

			auto base4 = foo::Base::get_factory().create("Python", "PythonDerived2", 4);
			LOGI("%s", base4->getKEY().c_str());
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

	}
	catch (const py::error_already_set& /*e*/)
	{
		LOGE("exception in boost::python ...");
		PyErr_Print();
	}
}

/*
LOGI("call func1() in XX.py");
globals["func1"](1, 2, 3, 5.0f, "hiiii from c++");
*/

