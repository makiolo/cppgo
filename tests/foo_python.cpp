#include "foo_python.h"
//
#include <boost/python.hpp>
#include <tuple>
#include <type_traits>

namespace regBasePythonic {
	foo::Base::factory::registrator<BasePythonImpl> reg(	foo::Base::get_factory() );
}

void init_impl()
{
	init_factory();
}

template <typename ... Args, size_t... Is>
constexpr auto my_extract_impl(const py::tuple& t, std::index_sequence<Is...>) {
	return std::make_tuple(py::extract< typename std::tuple_element<Is, std::tuple<Args...> >::type >(t[Is])()...);
}

template <typename ... Args>
constexpr auto my_extract(const py::tuple& t) {
    return my_extract_impl<Args...>(t, std::make_index_sequence<sizeof...(Args)>{});
}

template <class F, size_t... Is>
constexpr auto index_apply_impl(F&& f, std::index_sequence<Is...>) {
    return f(std::integral_constant<size_t, Is> {}...);
}

template <size_t N, class F>
constexpr auto index_apply(F&& f) {
    return index_apply_impl(std::forward<F>(f), std::make_index_sequence<N>{});
}

template <typename Function, typename Tuple>
constexpr auto apply(Function&& f, Tuple&& t) {
    return index_apply<std::tuple_size<Tuple>{}>(
        [&](auto... Is) {
            return std::forward<Function>(f)( std::get<Is>(std::forward<Tuple>(t))... );
        });
}

template <typename T, typename ... Args>
object _create_ref(Args&& ... args)
{
	return object( T::get_factory().create(std::forward<Args>(args)...) );
}

template <typename T, typename ... Args>
object create_ref(py::tuple args, py::dict kwargs)
{
	return apply( _create_ref<T, Args...>, my_extract<Args...>(args) );
}

template <typename T, typename ... Args>
object create_copy(Args ... args)
{
	return object( T::get_factory().create(std::move(args)...) );
}

BOOST_PYTHON_MODULE(foo_python)
{
	class_<BasePythonInterface,boost::noncopyable >("Base", init<std::string, int>())
		.def("name", &BasePythonInterface::getKEY, &BasePythonInterface::getKEY_default, py::return_value_policy<py::copy_const_reference>())
	;
	class_<foo::Base, std::shared_ptr<foo::Base> >("__Base__", init<std::string, int>())
		.def("name", &foo::Base::getKEY, py::return_value_policy<py::copy_const_reference>())
	;
   	def("create", raw_function(create_ref<foo::Base, std::string, std::string, int>, 3));
   	def("create2", create_copy<foo::Base, std::string, std::string, int>);
	def("init", init_impl);
}

