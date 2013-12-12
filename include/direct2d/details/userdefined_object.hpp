#pragma once

#include "../userdefined_object.hpp"

#include <utility>

namespace quote{
 namespace direct2d{

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	std::vector<object*>::reverse_iterator find_object(const point &ap)
	{
		Point p = ap - this->GetPosition();
		return std::find_if(
			objects.rbegin(), objects.rend(),
			[&p](Object *o){return o->IsColliding(p); });
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	userdefined_object<Derived, CheckDuplicate, MultiThread>::userdefined_object():
		hover(nullptr),
		focus(nullptr),
		pushing(false),
		creation(),
		created(false)
	{
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	userdefined_object<Derived, CheckDuplicate, MultiThread>::~userdefined_object()
	{
		destroy_resource();
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	bool userdefined_object<Derived, CheckDuplicate, MultiThread>::is_colliding(const point &ap)
	{
		if(!ap.is_inside(get_rect()))
			return false;
		return find_object(ap) != objects.end();
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::on_left_press(const point &ap, hittest &ht)
	{
		pushing = true;

		auto it = find_object(ap);
		if(it != objects.rend()){
			if(focus != nullptr && focus != *it)
				focus->on_lose_focus(ht);
			focus = *it;
			focus->on_left_press(ap - this->get_position(), ht);
		}
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::on_left_release(const point &ap, hittest &ht)
	{
		pushing = false;
		if(focus != nullptr){
			focus->on_left_release(ap - this->get_position(), ht);
		}
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::on_mouse_move(const point &ap, hittest &ht)
	{
		if(pushing){
			if(focus != nullptr){
				focus->on_mouse_move(ap - this->get_position(), ht);
			}
		} else{
			auto it = find_object(ap);
			if(it != objects.rend()){
				(*it)->on_mouse_move(ap - this->get_position(), ht);
				if(hover != nullptr && hover != *it){
					hover->on_mouse_leave(ht);
				}
				hover = *it;
			} else if(hover != nullptr){
				hover->on_mouse_leave(ht);
				hover = nullptr;
			}
		}
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::on_mouse_leave(const hittest &ht)
	{
		if(!pushing && hover != nullptr){
			hover->on_mouse_leave(ht);
			hover = nullptr;
		}
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::on_lose_focus(const hittest &ht)
	{
		if(focus != nullptr){
			focus->on_lose_focus(ht);
			focus = nullptr;
		}
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	bool userdefined_object<Derived, CheckDuplicate, MultiThread>::create_resource(const creation_params &cp)
	{
		created = true;
		creation = cp;
		return std::all_of(
			resources.begin(), resources.end(),
			std::bind(
				&resource::create_resource,
				std::placeholders::_1,
				std::ref(cp)))
			&& std::all_of(
			objects.begin(), objects.end(),
			std::bind(
				&object::create_resource,
				std::placeholders::_1,
				std::ref(cs)));
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::destroy_resource()
	{
		created = false;
		std::for_each(resources.begin(), resources.end(), std::mem_fn(&resource::destroy_resource));
		std::for_each(objects.begin(), objects.end(), std::mem_fn(&object::destroy_resource));
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::draw(const paint_params &pp)
	{
		auto p = this->get_position();
		D2D1::Matrix3x2F transform;
		pp.target->GetTransform(&transform);
		pp.target->SetTransform(transform * D2D1::Matrix3x2F::Translation(p.x, p.y));

		pp.target->PushAxisAlignedClip(
			rect(point(), this->get_size()),
			D2D1_ANTIALIAS_MODE_ALIASED);

		std::for_each(
			objects.begin(), objects.end(),
			std::bind(
				&object::draw,
				std::placeholders::_1,
				std::ref(ps)));

		pp.target->PopAxisAlignedClip();

		pp.target->SetTransform(transform);
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::reorder_object(object *move, object *base, bool behind /* =false */)
	{
		auto it = std::find(objects.begin(), objects.end(), move);
		if(it != objects.end()){
			if(base == nullptr){
				if(it != objects.begin()){
					std::copy_backward(objects.begin(), it - 1, it + 1);
					objects[0] = *it;
				}
			} else{
				auto base_it = std::find(objects.begin(), objects.end(), base);
				if(base_it == objects.end())
					return;
				if(it < base_it){
					std::rotate(it, it + 1, base_it);
					if(behind)
						std::iter_swap(base_it, base_it - 1);
				} else if(it > base_it){
					std::rotate(base_it + 1, it, it + 1);
					if(!behind)
						std::iter_swap(base_it, base_it + 1);
				}
			}
		}
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::register_object(object *o)
	{
		if(o == nullptr)
			return;
		if(CheckDuplicate && std::find(objects.begin(), objects.end(), o) != objects.end())
			return;

		if(created)
			o->create_resource(creation);
		objects.push_back(o);
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::unregister_object(object *o)
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

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::delete_object(object *o)
	{
		if(o == nullptr)
			return;

		unregister_object(o);
		o->destroy_resource();
		delete o;
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::register_resource(resource *r)
	{
		if(r == nullptr)
			return;
		if(CheckDuplicate && resources.find(r) != resources.end()){
			// throw std::runtime_error("resource is already registered");
			return;
		}

		if(created)
			r->create_resource(creation);
		resources.insert(r);
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::unregister_resource(resource *r)
	{
		if(r == nullptr)
			return;

		auto it = resources.find(r);
		if(it != resources.end()){
			r->destroy_resource();
			resources.erase(it);
			return;
		}

		// throw std::runtime_error("non-registered resource");
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::delete_resource(resource *r)
	{
		if(r == nullptr)
			return;

		unregister_resource(r);
		r->destroy_resource();
		delete r;
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	template <class Object, class... Args>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::init_object(Object *&o, Args&&... args)
	{
		o = new Object(std::forward(args)...);
		register_object(o);
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	template <class Object, class... Args>
	Object *userdefined_object<Derived, CheckDuplicate, MultiThread>::new_object(Args&&... args)
	{
		Object *o;
		init_object(o, std::forward(args)...);
		return o;
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	template <class Resource, class... Args>
	void userdefined_object<Derived, CheckDuplicate, MultiThread>::init_resource(Resource *&r, Args&&... args)
	{
		r = new Resource(std::forward(args)...);
		register_resource(r);
	}

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false>
	template <class Resource, class... Args>
	Resource *userdefined_object<Derived, CheckDuplicate, MultiThread>::new_resource(Args&&... args)
	{
		Resource *r;
		init_resource(r, std::forward(args)...);
		return r;
	}

} }
