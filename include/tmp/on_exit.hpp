#pragma once

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

} }
