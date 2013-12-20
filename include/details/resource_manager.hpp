#pragma once

#include "../resource_manager.hpp"

#include <algorithm>
#include <functional>
#include <stdexcept>

namespace quote{

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::~resource_manager()
	{
		destroy_resource();
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	bool resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::create_resource(const typename Traits::creation_params &cp)
	{
		created = true;
		return std::all_of(
			resources.begin(), resources.end(),
			std::bind(
				&resource::create_resource,
				std::placeholders::_1,
				std::ref(cp)));
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::destroy_resource()
	{
		created = false;
		for(auto &res: resources){
			res->destroy_resource();
		}
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::register_resource(typename Traits::resource *r)
	{
		if(r == nullptr)
			return;
		if(CheckDuplicate && resources.find(r) != resources.end()){
			// std::runtime_error("this resource is already registered");
			return;
		}

		if(created)
			r->create_resource(static_cast<Derived*>(this)->creation_params());
		resources.insert(r);
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::unregister_resource(typename Traits::resource *r)
	{
		if(r == nullptr)
			return;

		auto it = resources.find(r);
		if(it != resources.end()){
			r->destroy_resource();
			resources.erase(it);
		}else{
			// std::runtime_error("non-registered resource");
		}
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::delete_resource(typename Traits::resource *r)
	{
		if(r == nullptr)
			return;
		unregister_resource(r);
		delete r;
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	template <class Resource, class... Args>
	void resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::init_resource(Resource *&r, Args&&... args)
	{
		r = new Resource(std::forward(args)...);
		register_resource(r);
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	template <class Resource, class... Args>
	Resource *resource_manager<Derived, Traits, CheckDuplicate, MultiThread>::new_resource(Args&&... args)
	{
		Resource r;
		init_resource(r, std::forward(args)...);
		return r;
	}

}
