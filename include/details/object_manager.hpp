#pragma once

#include "../object_manager.hpp"

#include <algorithm>
#include <utility>
#include <stdexcept>

namespace quote{

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	object_manager<Derived, Traits, CheckDuplicate, MultiThread>::~object_manager()
	{
		destroy_resource();
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	bool object_manager<Derived, Traits, CheckDuplicate, MultiThread>::create_resource(const typename Traits::creation_params &cp)
	{
		created = true;
		return std::all_of(
			objects.begin(), objects.end(),
			std::bind(
				&Traits::object::create_resource,
				std::placeholders::_1,
				std::ref(cp)));
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::destroy_resource()
	{
		created = false;
		std::for_each(objects.begin(), objects.end(), std::mem_fn(&Traits::object::destroy_resource));
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::draw(const typename Traits::paint_params &pp)
	{
		std::for_each(
			objects.begin(), objects.end(),
			std::bind(
				&Traits::object::draw,
				std::placeholders::_1,
				std::ref(pp)));
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::reorder_object(typename Traits::object *move, typename Traits::object *base, bool behind /* =false */)
	{
		auto it = std::find(objects.begin(), objects.end(), move);
		if(it != objects.end()){
			if(base == nullptr){
				if(it != objects.begin()){
					std::copy_backward(objects.begin(), it - 1, it + 1);
					objects[0] = *it;
				}
			}else{
				auto base_it = std::find(objects.begin(), objects.end(), base);
				if(base_it == objects.end())
					return;
				if(it < base_it){
					std::rotate(it, it + 1, base_it); 
					if(behind)
						std::iter_swap(base_it, base_it - 1);
				}else if(it > base_it){
					std::rotate(base_it + 1, it, it + 1);
					if(!behind)
						std::iter_swap(base_it, base_it + 1);
				}
			}
		}
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::register_object(typename Traits::object *o)
	{
		if(o == nullptr)
			return;
		if(CheckDuplicate && std::find(objects.begin(), objects.end(), o) != objects.end())
			return;

		if(created)
			o->create_resource(static_cast<Derived*>(this)->creation_params());
		objects.push_back(o);
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::unregister_object(typename Traits::object *o)
	{
		if(o == nullptr)
			return;

		auto it = std::find(objects.begin(), objects.end(), o);
		if(it != objects.end()){
			o->destroy_resource();
			objects.erase(it);
			return;
		}

		// throw std::runtime_error("non-registered object");
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::delete_object(typename Traits::object *o)
	{
		if(o == nullptr)
			return;

		unregister_object(o);
		o->destroy_resource();
		delete o;
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	template <class Object, class... Args>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::init_object(Object *&o, Args&&... args)
	{
		o = new Object(std::forward(args)...);
		register_object(o);
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	template <class Object, class... Args>
	Object *object_manager<Derived, Traits, CheckDuplicate, MultiThread>::new_object(Args&&... args)
	{
		Object *o;
		init_object(o, std::forward(args)...);
		return o;
	}

	template <class Derived, class Traits, bool CheckDuplicate, bool MultiThread>
	template <class Functor>
	void object_manager<Derived, Traits, CheckDuplicate, MultiThread>::iterate_objects(Functor func)
	{
		std::any_of(objects.rbegin(), objects.rend(), [&func](typename Traits::object *o){return func(*o);});
	}

}