#include <iostream>
#include <assert.h>
#include <tuple>
#include <teelogging/teelogging.h>
#include <boost/python.hpp>
#include <gtest/gtest.h>
#include <dp14/factory.h>

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

class BaseExample
{
public:
	using factory = dp14::factory<BaseExample, std::string, int>;

	explicit BaseExample(std::string name, int q)
		: _name(name)
		, _q(q)
	{
		std::cout << "constructor " << _name << " - " << _q << std::endl;
	}
	virtual ~BaseExample() { std::cout << "destruction" << std::endl; }
	virtual std::string name() const { return "Base"; }

protected:
	std::string _name;
	int _q;
};

class A_Example : public BaseExample
{
public:
	DEFINE_KEY(A_Example)
	explicit A_Example(std::string name, int q) : BaseExample(name, q) { ; }
	virtual ~A_Example() = default;
	virtual std::string name() const { return "A_Example"; }
};

class B_Example : public BaseExample
{
public:
	DEFINE_KEY(B_Example)
	explicit B_Example(std::string name, int q) : BaseExample(name, q) { ; }
	virtual ~B_Example() = default;
	virtual std::string name() const { return "B_Example"; }
};

class PythonDerived : public BaseExample
{
public:
	DEFINE_KEY(PythonDerived)
	explicit PythonDerived(std::string name, int q) : BaseExample(name, q)
	{ 
		// init
		py::object mainmodule = py::import("__main__");
		py::object globals = mainmodule.attr("__dict__");
		py::exec_file("hello.py", globals, globals);
		// instance
		py::object class_derived = globals["PythonDerived"];
		_instance = class_derived(name, q);
		/*
		// call static method
		LOGI("call func1() in hello.py");
		globals["func1"](1, 2, 3, 5.0f, "hiiii from c++");
		*/
	}
	virtual ~PythonDerived() = default;
	virtual std::string name() const
	{
		auto result = _instance.attr("name")();
		return py::extract<std::string>(py::str(result))();
	}
protected:
	py::object _instance;
};

namespace regA {
	BaseExample::factory::registrator<A_Example> reg;
}
namespace regB {
	BaseExample::factory::registrator<B_Example> reg;
}
namespace regPD {
	BaseExample::factory::registrator<PythonDerived> reg;
}

TEST(PythonTest, Test1)
{
	PythonEmbed python;
	try
	{		
		auto base1 = Base::factory::instance().create("PythonDerived", "from factory c++", 1234567);
		std::cout << "base1 = " << base1->name() << std::endl;
		
		auto base2 = Base::factory::instance().create("A_Example", "from factory c++", 1234567);
		std::cout << "base2 = " << base2->name() << std::endl;
		
		auto base3 = Base::factory::instance().create("B_Example", "from factory c++", 1234567);
		std::cout << "base3 = " << base3->name() << std::endl;
	}
	catch (const py::error_already_set& /*e*/)
	{
		LOGE("exception in boost::python ...");
		PyErr_Print();
	}
}
