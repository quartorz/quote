#pragma once

#include <functional>

namespace quote{ namespace tmp{

	template <class Func>
	class on_exit_class{
		Func f;
	public:
		on_exit_class(Func f):f(f)
		{
		}
		~on_exit_class()
		{
			f();
		}
	};

	template <class Func>
	on_exit_class<Func> on_exit(Func f)
	{
		return on_exit_class<Func>(f);
	}

#define QUOTE_ON_EXIT_I_I(line) ::quote::tmp::on_exit_class<std::function<void(void)>> quote_on_exit_class_instance_ ## line = [&]()
#define QUOTE_ON_EXIT_I(line) QUOTE_ON_EXIT_I_I(line)
#define QUOTE_ON_EXIT QUOTE_ON_EXIT_I(__LINE__)

} }
