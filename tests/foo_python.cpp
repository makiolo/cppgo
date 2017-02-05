#include "foo_python.h"
//
#include <boost/python.hpp>

namespace regBasePythonic {
	foo::Base::factory::registrator<BasePythonic> reg(	foo::Base::get_factory() );
}

std::ostream& operator<<(std::ostream& os, const py::str& o)
{
    return os << py::extract<std::string>(py::str(o))();
}


object init_impl(tuple args, dict kwargs)
{
	init_factory();

	return object();
}

object create(tuple args, dict kwargs)
{
	// for(int i = 0; i < len(args); ++i)
	// {
	// 	std::cout << "arg " << i << ": " << py::str(args[i]) << std::endl;
	// }

	return object( foo::Base::get_factory().create(
								std::string( py::extract<const char*>(py::str(args[0]))() ),
								std::string( py::extract<const char*>(py::str(args[1]))() ),
								py::extract<int>(args[2])()
		      				));
}

BOOST_PYTHON_MODULE(foo_python)
{
	class_<foo::Base, std::shared_ptr<foo::Base> >("Base", init<std::string, int>())
		.def("name", &foo::Base::getKEY, py::return_value_policy<py::copy_const_reference>())
	;

   	def("create", raw_function(create, 3));
	def("init", raw_function(init_impl));
}

