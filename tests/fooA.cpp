#include "fooA.h"

namespace reg_fooA {

	foo::Base::factory::registrator<fooA::A> reg(
			foo::Base::get_factory()
		);

}

