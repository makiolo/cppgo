#include "foo_python.h"
//
#include <boost/python.hpp>
#include <tuple>
#include <type_traits>

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

#if 1

template<typename ... Args>
class get_type
{
    template <std::size_t N>
    using type = typename std::tuple_element<N, std::tuple<Args...> >::type;
};

template <class F, size_t... Is>
constexpr auto index_apply_impl(F&& f, std::index_sequence<Is...>) {
    return f(std::integral_constant<size_t, Is> {}...);
}

template <size_t N, class F>
constexpr auto index_apply(F&& f) {
    return index_apply_impl(std::forward<F>(f), std::make_index_sequence<N>{});
}

template <class Args...>
constexpr auto expand(const py::tuple& t) {
    return index_apply<sizeof...(Args)>(
        [&](auto... Is) {
        	return make_tuple(
			py::extract< get_type<Args...>::type<Is> >(t[Is])()...
		);
        });
}

template <typename Function, typename Tuple>
constexpr auto apply(Function&& f, Tuple&& t) {
    return index_apply<std::tuple_size<Tuple>{}>(
        [&](auto... Is) {
            return std::forward<Function>(f)( std::get<Is>(std::forward<Tuple>(t))... );
        });
}

#endif

object create(py::tuple args, py::dict kwargs)
{
	auto new_args = expand<std::string, std::string, int>(args);
	
	std::cout << "--------------------" << std::endl;
	std::cout << std::get<0>(new_args) << std::endl;
	std::cout << std::get<1>(new_args) << std::endl;
	std::cout << std::get<2>(new_args) << std::endl;
	std::cout << "--------------------" << std::endl;

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
