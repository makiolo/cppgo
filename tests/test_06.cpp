#include <iostream>
#include <assert.h>
#include <tuple>
#include <dp14/factory.h>
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/import.hpp>
#include <boost/python/exec.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/extract.hpp>
#include <gtest/gtest.h>

using namespace boost::python;
namespace py = boost::python;

class Base
{
public:
	using factory = dp14::factory<Base, std::string, int>;

	explicit Base(std::string name, int q)
		: _name(name)
		, _q(q)
	{
		std::cout << "constructor " << _name << " - " << _q << std::endl;
	}
	virtual ~Base() { std::cout << "destruction" << std::endl; }
	virtual std::string name() const { return "Base"; }

protected:
	std::string _name;
	int _q;
};

struct BaseWrap : Base, py::wrapper<Base>
{
	explicit BaseWrap(std::string name, int q) : Base(name, q) { ; }
	virtual ~BaseWrap() = default;
	
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

class A : public Base
{
public:
	DEFINE_KEY(A)
	explicit A(std::string name, int q) : Base(name, q) { ; }
	virtual ~A() = default;
	virtual std::string name() const { return "A"; }
};

class B : public Base
{
public:
	DEFINE_KEY(B)
	explicit B(std::string name, int q) : Base(name, q) { ; }
	virtual ~B() = default;
	virtual std::string name() const { return "B"; }
};

namespace regA
{
	Base::factory::registrator<A> reg;
}
namespace regB
{
	Base::factory::registrator<B> reg;
}

// http://codereview.stackexchange.com/questions/9202/boost-python-converter-for-stdtuple
//
// template<int ...> struct sequ{};
// template<int N, int ...S> struct gener : gener<N-1, N-1, S...>{};
// template<int ...S> struct gener<0, S...> {typedef sequ<S...> type;};
//
// template <typename ...Args>
// struct cpptuple2pytuple_wrapper
// {
//   std::tuple<Args...> params;
//   cpptuple2pytuple_wrapper(const std::tuple<Args...>& _params):params(_params){}
//
//   boost::python::tuple delayed_dispatch()
//   {
//     return callFunc(typename gener<sizeof...(Args)>::type());
//   }
//
//   template<int ...S>
//   boost::python::tuple callFunc(sequ<S...>)
//   {
//     return boost::python::make_tuple(std::get<S>(params) ...);
//   }
// };
//
// template <typename ...Args>
// struct pytuple2cpptuple_wrapper
// {
//   boost::python::tuple params;
//   pytuple2cpptuple_wrapper(const boost::python::tuple& _params):params(_params){}
//
//   std::tuple<Args...> delayed_dispatch()
//   {
//     return callFunc(typename gener<sizeof...(Args)>::type());
//   }
//
//   template<int ...S>
//   std::tuple<Args...> callFunc(sequ<S...>)
//   {
//     return std::make_tuple((static_cast<Args>(extract<Args>(params[S])))...);
//   }
// };
//
// // Convert (C++) tuple to (Python) tuple as PyObject*.
// template<typename ... Args> PyObject* cpptuple2pytuple(const std::tuple<Args...>& t)
// {
//   cpptuple2pytuple_wrapper<Args...> wrapper(t);
//   boost::python::tuple bpt = wrapper.delayed_dispatch();
//   return boost::python::incref(boost::python::object(bpt).ptr());
// }
//
// // Convert (Python) tuple to (C++) tuple.
// template<typename ... Args> std::tuple<Args...> pytuple2cpptuple(PyObject* obj)
// {
//   boost::python::tuple tup(boost::python::borrowed(obj));
//   pytuple2cpptuple_wrapper<Args...> wrapper(tup);
//   std::tuple<Args...> bpt = wrapper.delayed_dispatch();
//   return bpt;
// }
//
// template<typename ... Args>
// struct cpptuple_to_python_tuple
// {
//   static PyObject* convert(const std::tuple<Args...>& t)
//     {
//       return cpptuple2pytuple<Args...>(t);
//     }
// };
//
// template<typename ... Args>
// struct cpptuple_from_python_tuple
// {
//   cpptuple_from_python_tuple()
//   {
//     boost::python::converter::registry::push_back(
//                           &convertible,
//                           &construct,
//                           boost::python::type_id<std::tuple<Args...> >());
//   }
//
//   static void* convertible(PyObject* obj_ptr)
//   {
//     if (!PyTuple_CheckExact(obj_ptr)) return 0;
//     return obj_ptr;
//   }
//
//   static void construct(
//             PyObject* obj_ptr,
//             boost::python::converter::rvalue_from_python_stage1_data* data)
//   {
//     void* storage = (
//              (boost::python::converter::rvalue_from_python_storage<std::tuple<Args...> >*)
//              data)->storage.bytes;
//     new (storage) std::tuple<Args...>(pytuple2cpptuple<Args...>(obj_ptr));
//     data->convertible = storage;
//   }
// };
//
// template<typename ...Args> void create_tuple_converter()
// {
//   boost::python::to_python_converter<std::tuple<Args...>, cpptuple_to_python_tuple<Args...> >();
//   cpptuple_from_python_tuple<Args...>();
// }
//
// void export_cpptuple_conv()
// {
// 	create_tuple_converter<py::str, py::str, int>();
// }
//
// void create(std::tuple<py::str, py::str, int> t)
// {
// 	std::cout << "tuple: str str int" << std::endl;
// }

template <typename T, std::size_t... Indices>
auto vectorToTupleHelper(const std::vector<T>& v, std::index_sequence<Indices...>) {
	return std::make_tuple(v[Indices]...);
}

template <std::size_t N, typename T>
auto vectorToTuple(const std::vector<T>& v) {
	assert(v.size() >= N);
	return vectorToTupleHelper(v, std::make_index_sequence<N>());
}

std::ostream& operator<<(std::ostream& os, const py::str& o)
{
    return os << py::extract<std::string>(py::str(o))();
}

object create(tuple args, dict kwargs)
{
	for(int i = 0; i < len(args); ++i)
	{
		std::cout << "cout: " << py::str(args[i]) << std::endl;
	}
	
	return object( Base::factory::instance().create( 	std::string( py::extract<const char*>(py::str(args[0]))() ), 
								std::string( py::extract<const char*>(py::str(args[1]))() ), 
								py::extract<int>(args[2])() 
		      					));
}

object register_impl(tuple args, dict kwargs)
{
	std::cout << "register: " << std::string( py::extract<const char*>(py::str(args[0]))() ) << std::endl;
	return object();
}

BOOST_PYTHON_MODULE(factory)
{
	class_<Base, std::shared_ptr<Base> >("__Base__", init<std::string, int>())
	;
	
	class_<BaseWrap, boost::noncopyable >("Base", init<std::string, int>())
		.def("name", &Base::name, &BaseWrap::default_name)
	;
	
   	def("create", raw_function(create));
	def("register", raw_function(register_impl));
}

class PythonTest : testing::Test { };

/*
template <typename ... Args>
void call_python(const std::string& name_function, const Args& ... data)
{
	try
	{
		// http://www.boost.org/doc/libs/1_57_0/libs/python/doc/v2/callbacks.html#argument_handling
		//output[name_function](std::forward<Args>(data)...);
		output[name_function](data...);
		//output.attr(name_function.c_str())(data...);
	}
	catch (const boost::python::error_already_set& e)
	{
		(void)e;

		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		boost::python::handle<> hType(ptype);
		boost::python::object extype(hType);
		std::string strErrorMessage = boost::python::extract<std::string>(pvalue);
		if (ptraceback)
		{
			boost::python::handle<> hTraceback(ptraceback);
			boost::python::object traceback(hTraceback);

			long lineno = boost::python::extract<long>(traceback.attr("tb_lineno"));
			std::string filename = boost::python::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
			std::string funcname = boost::python::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
			LOGE("[Python] %s:%ld: <%s> %s", FileSystem::basename(filename).c_str(), lineno, funcname.c_str(), strErrorMessage.c_str());
		}
		else
		{
			LOGE("[Python] %s", strErrorMessage.c_str());
		}
	}
}
*/

TEST(PythonTest, Test1)
{
	// char* python_home = const_cast<char*>(ss2.str().c_str());
	// Py_SetPythonHome(python_home);
	Py_NoSiteFlag=1;
	// Py_DebugFlag=1;
	//Py_SetProgramName(const_cast<wchar*>("test_06"));
	Py_SetProgramName(L"test_06");
	// PyImport_AppendInittab("Engine", initEngine);
	Py_InitializeEx(0);
	
	try
	{
		py::object mainmodule = py::import("__main__");
		py::object globals = mainmodule.attr("__dict__");
		py::object result = py::exec_file("hello.py", globals, globals);
		output = py::extract<py::dict>(globals);
	}
	catch (const py::error_already_set& /*e*/)
	{
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		py::handle<> hType(ptype);
		py::object extype(hType);
		std::string strErrorMessage = py::extract<std::string>(pvalue);
		if (ptraceback)
		{
			py::handle<> hTraceback(ptraceback);
			py::object traceback(hTraceback);

			long lineno = py::extract<long>(traceback.attr("tb_lineno"));
			std::string filename = py::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
			std::string funcname = py::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
			printf("[Python] hello.py:%ld: <%s> %s", lineno, funcname.c_str(), strErrorMessage.c_str());
		}
		else
		{
			printf("[Python] %s", strErrorMessage.c_str());
		}
	}
}
