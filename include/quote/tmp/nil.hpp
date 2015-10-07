#pragma once

#include <type_traits>

namespace quote{ namespace tmp{

	class nil{
	public:
		template <class T>
		void operator=(const T&)
		{
		}
		template <class T>
		operator T()
		{
			return T();
		}
	};

} }
