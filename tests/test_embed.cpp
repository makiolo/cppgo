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

// python to c++
struct BaseWrapExample : BaseExample, py::wrapper<BaseExample>
{
	explicit BaseWrapExample(std::string name, int q) : BaseExample(name, q) { ; }
	virtual ~BaseWrapExample() = default;
	
	virtual std::string name() const
	{
		if (override n = this->get_override("name"))
		    return n();
		return Base::name();
	}
	
	std::string default_name() const
	{
		return this->Base::name();
	}
};

// c++ to python
class PythonDerived : public BaseExample
{
public:
	DEFINE_KEY(PythonDerived1)
	explicit PythonDerived(std::string name, int q) : BaseExample(name, q)
	{ 
		// init
		py::object mainmodule = py::import("__main__");
		py::object globals = mainmodule.attr("__dict__");
		py::exec_file("hello.py", globals, globals);
		// instance
		py::object class_derived = globals["PythonDerived1"];
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
		auto base2 = BaseExample::factory::instance().create("A_Example", "from factory c++", 1234567);
		std::cout << "base2 = " << base2->name() << std::endl;
		
		auto base3 = BaseExample::factory::instance().create("B_Example", "from factory c++", 1234567);
		std::cout << "base3 = " << base3->name() << std::endl;
		
		auto base1 = BaseExample::factory::instance().create("PythonDerived1", "from factory c++", 1234567);
		std::cout << "base1 = " << base1->name() << std::endl;
		
		// auto base1 = BaseExample::factory::instance().create("PythonDerived2", "from factory c++", 1234567);
		// std::cout << "base1 = " << base1->name() << std::endl;
	}
	catch (const py::error_already_set& /*e*/)
	{
		LOGE("exception in boost::python ...");
		PyErr_Print();
	}
}

object create(tuple args, dict kwargs)
{
	return object( BaseExample::factory::instance().create( 	
								std::string( py::extract<const char*>(py::str(args[0]))() ), 
								std::string( py::extract<const char*>(py::str(args[1]))() ), 
								py::extract<int>(args[2])() 
		      					));
}

object register_impl(tuple args, dict kwargs)
{
	std::cout << "register: " << std::string( py::extract<const char*>(py::str(args[0]))() ) << std::endl;
	return object();
}

BOOST_PYTHON_MODULE(test_embed)
{
	class_<BaseExample, std::shared_ptr<BaseExample> >("__BaseExample__", init<std::string, int>())
	;
	
	class_<BaseWrapExample, boost::noncopyable >("BaseExample", init<std::string, int>())
		.def("name", &BaseWrapExample::name, &BaseWrapExample::default_name)
	;
	
   	def("create", raw_function(create));
	def("register", raw_function(register_impl));
}
