#include <iostream>
#include <assert.h>
#include <tuple>
#include <dp14/factory.h>

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

protected:
	std::string _name;
	int _q;
};

class A : public Base
{
public:
	DEFINE_KEY(A)
	explicit A(std::string name, int q) : Base(name, q) { ; }
	virtual ~A() = default;
};

class B : public Base
{
public:
	explicit B(std::string name, int q) : Base(name, q) { ; }
	virtual ~B() = default;
};

// no-macros option
namespace std {
	template <>
	struct hash<B>
	{
		size_t operator()() const
		{
			return std::hash<std::string>()("B");
		}
	};
}

// 	Base::factory f;
// 	Base::factory::registrator<A> reg1(f);
// 	Base::factory::registrator<B> reg2(f);
// 		// equivalent ways of create A
// 		std::shared_ptr<Base> a1 = f.create<A>("first parameter", 2);
// 		std::shared_ptr<A> a2 = f.create<A>("first parameter", 2);
// 		std::shared_ptr<Base> a3 = f.create("A", "first parameter", 2);
// 		// equivalent ways of create B
// 		std::shared_ptr<Base> b1 = f.create<B>("first parameter", 2);
// 		std::shared_ptr<B> b2 = f.create<B>("first parameter", 2);
// 		std::shared_ptr<Base> b3 = f.create("B", "first parameter", 2);

#include <boost/python.hpp>
using namespace boost::python;
namespace py = boost::python;


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

std::ostream& operator<<(std::ostream& os, const py::str& o)
{
    return os << py::extract<std::string>(py::str(o))();
}

object create(tuple args, dict kwargs)
{
	std::vector< std::string > parms;
    for(int i = 1; i < len(args); ++i) {
		// std::stringstream ss;
		// ss << py::str( args[i] );
		// parms.emplace_back(ss.str());
		std::cout << "cout: " << py::str(args[i]) << std::endl;
    }

	for(auto p : parms)
	{
		std::cout << "stringstream: " << p << std::endl;
	}

	return object();
}

BOOST_PYTHON_MODULE(factory)
{
    class_<Base, std::shared_ptr<Base> >("Base", init<std::string, int>())
   	;

    class_<A, bases<Base> >("A", init<std::string, int>())
	;

    class_<B, bases<Base> >("B", init<std::string, int>())
	;

    class_<Base::factory >("factory")
	;
	
    class_<Base::factory::registrator<A> >("regA")
	;
	
    class_<Base::factory::registrator<B> >("regB")
	;
	
	def("create", raw_function(create, 1));
}

