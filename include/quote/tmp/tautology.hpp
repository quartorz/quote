#pragma once

namespace quote{ namespace tmp{

	template <class Type>
	class tautological_pred{
	public:
		static const bool value = true;
	};

	template <std::size_t Value>
	class tautological_filter{
	public:
		static const bool value = true;
	};

} }
