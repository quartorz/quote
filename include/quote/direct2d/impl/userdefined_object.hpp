#pragma once

#include "../userdefined_object.hpp"

#include <utility>

namespace quote{ namespace direct2d{

	template <class Derived>
	std::vector<object*>::reverse_iterator userdefined_object<Derived>::find_object(const point &ap)
	{
		point p = ap - this->get_position();
		return std::find_if(
			objects.rbegin(), objects.rend(),
			[&p](object *o){return o->is_colliding(p);});
	}

	template <class Derived>
	userdefined_object<Derived>::userdefined_object():
		hover(nullptr),
		focus(nullptr),
		pushing(false),
		creation(),
		created(false)
	{
	}

	template <class Derived>
	bool userdefined_object<Derived>::is_colliding(const point &ap)
	{
		if(!ap.is_inside(get_rect()))
			return false;
		return find_object(ap) != objects.rend();
	}

	template <class Derived>
	void userdefined_object<Derived>::on_left_press(const point &ap, hittest &ht)
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

	template <class Derived>
	void userdefined_object<Derived>::on_left_release(const point &ap, hittest &ht)
	{
		pushing = false;
		if(focus != nullptr){
			focus->on_left_release(ap - this->get_position(), ht);
		}
	}

	template <class Derived>
	void userdefined_object<Derived>::on_mouse_move(const point &ap, hittest &ht)
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

	template <class Derived>
	void userdefined_object<Derived>::on_mouse_leave(const hittest &ht)
	{
		if(!pushing && hover != nullptr){
			hover->on_mouse_leave(ht);
			hover = nullptr;
		}
	}

	template <class Derived>
	void userdefined_object<Derived>::on_lose_focus(const hittest &ht)
	{
		if(focus != nullptr){
			focus->on_lose_focus(ht);
			focus = nullptr;
		}
	}

	template <class Derived>
	bool userdefined_object<Derived>::create_resource(const creation_params &cp)
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
				std::ref(cp)));
	}

	template <class Derived>
	void userdefined_object<Derived>::destroy_resource()
	{
		created = false;
		std::for_each(resources.begin(), resources.end(), std::mem_fn(&resource::destroy_resource));
		std::for_each(objects.begin(), objects.end(), std::mem_fn(&object::destroy_resource));
	}

	template <class Derived>
	void userdefined_object<Derived>::draw(const paint_params &pp)
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
				std::ref(pp)));

		pp.target->PopAxisAlignedClip();

		pp.target->SetTransform(transform);
	}

} }
