#include "fooB.h"

namespace reg_fooB {

	foo::Base::factory::registrator<fooB::B> reg(
			foo::Base::get_factory()
		);

}

