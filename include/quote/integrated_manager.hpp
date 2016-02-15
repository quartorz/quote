#pragma once

#include <quote/object_manager.hpp>
#include <quote/resource_manager.hpp>

namespace quote{

	template <class Derived, class Traits>
	class integrated_manager:
		public object_manager<Derived, Traits>,
		public resource_manager<Derived, Traits>
	{
		using object_manager = object_manager<Derived, Traits>;
		using resource_manager = resource_manager<Derived, Traits>;

	public:
		bool create_resource(const typename Traits::creation_params &cp)
		{
			return this->object_manager::create_resource(cp)
				&& this->resource_manager::create_resource(cp);
		}
		void destroy_resource()
		{
			object_manager::destroy_resource();
			resource_manager::destroy_resource();
		}
		void draw(const typename Traits::paint_params &pp)
		{
			object_manager::draw(pp);
		}
	};

}
