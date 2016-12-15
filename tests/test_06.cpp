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

/*
template <typename T, std::size_t... Indices>
auto vectorToTupleHelper(const std::vector<T>& v, std::index_sequence<Indices...>) {
	return std::make_tuple(v[Indices]...);
}

template <std::size_t N, typename T>
auto vectorToTuple(const std::vector<T>& v) {
	assert(v.size() >= N);
	return vectorToTupleHelper(v, std::make_index_sequence<N>());
}
*/

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
