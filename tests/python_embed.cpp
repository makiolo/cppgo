#include <iostream>
#include <assert.h>
#include <tuple>
#include <teelogging/teelogging.h>
#include <boost/python.hpp>
#include <gtest/gtest.h>
#include <dp14/factory.h>
#include "foo.h"
#include "foo_python.h"

using namespace boost::python;
namespace py = boost::python;

class PythonTest : testing::Test { };

// https://wiki.python.org/moin/boost.python/EmbeddingPython
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
	// reg c++ implementations
	init_factory();

	PythonEmbed python;
	try
	{
		// register python implementations
		py::object mainmodule = py::import("__main__");
		py::object globals = mainmodule.attr("__dict__");
		py::exec_file("fooA_python.py", globals, globals);

		auto base1 = foo::Base::get_factory().create("A", "create c++ from factory c++", 111);
		std::cout << "base1 = " << base1->getKEY() << " end" << std::endl;

		auto base2 = foo::Base::get_factory().create("B", "create c++ from factory c++", 222);
		std::cout << "base2 = " << base2->getKEY() << " end" << std::endl;

		try
		{
		std::cout << "------------------------" << std::endl;
		auto base3 = foo::Base::get_factory().create("PythonDerived1", "create python from factory c++", 333);
		std::cout << "base3 = " << base3->getKEY() << " end" <<  std::endl;
		std::cout << "------------------------" << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		// auto base4 = foo::Base::get_factory().create("PythonDerived2", "create python from factory c++", 999);
		// std::cout << "base4 = " << base4->getKEY() << " end" <<  std::endl;
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

