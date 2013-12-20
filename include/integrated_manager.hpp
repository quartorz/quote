#pragma once

#include "object_manager.hpp"
#include "resource_manager.hpp"

namespace quote{

	template <class Derived, class Traits, bool CheckDuplicate=false, bool MultiThread=false>
	class integrated_manager:
		public object_manager<Derived, Traits, CheckDuplicate, MultiThread>,
		public resource_manager<Derived, Traits, CheckDuplicate, MultiThread>
	{
		using object_manager = object_manager<Derived, Traits, CheckDuplicate, MultiThread>;
		using resource_manager = resource_manager<Derived, Traits, CheckDuplicate, MultiThread>;

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
