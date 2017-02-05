#include "foo.h"
#include <iostream>

foo::Base::factory& foo::Base::get_factory()
{
	return foo::Base::factory::instance();
}
