#pragma once

#include "../scene_manager.hpp"

namespace quote{

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::add_scene(int key, scene<Traits, MultiThread> *scene)
	{
		if(created)
			scene->create_resource(static_cast<Derived*>(this)->creation_params());
		scenes.insert({key, scene});
		if(current_scene == nullptr){
			current_idx = key;
			current_scene = scene;
		}
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::remove_scene(int key)
	{
		if(created)
			scenes[key]->destroy_resource();
		scenes.erase(key);
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::select_scene(int key)
	{
		scenes[current_idx]->on_hide();
		current_scene = scenes[key];
		current_scene->on_show();
		current_idx = key;
		static_cast<Derived*>(this)->repaint();
	}

	template <class Derived, class Traits, bool MultiThread>
	scene<Traits, MultiThread> *scene_manager<Derived, Traits, MultiThread>::get_current_scene() const
	{
		return current_scene;
	}

	template <class Derived, class Traits, bool MultiThread>
	int scene_manager<Derived, Traits, MultiThread>::get_scene_count() const
	{
		return scenes.size();
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::on_size(int w, int h)	
	{
		typedef typename Traits::size size;
		for(auto &s: scenes){
			s.second->set_size(size(w, h));
		}
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::on_key_down(unsigned keycode)
	{
		current_scene->on_key_down(keycode);
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::on_key_up(unsigned keycode)
	{
		current_scene->on_key_up(keycode);
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::on_timer(unsigned id)
	{
		current_scene->on_timer(id);
	}

	template <class Derived, class Traits, bool MultiThread>
	bool scene_manager<Derived, Traits, MultiThread>::create_resource(const typename Traits::creation_params &cp)
	{
		created = true;
		return std::all_of(
			scenes.begin(), scenes.end(),
			[&cp](const std::pair<int, scene<Traits, MultiThread>*> &p){
				return p.second->create_resource(cp);
		});
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::destroy_resource()
	{
		created = false;
		for(auto &s: scenes){
			s.second->destroy_resource();
		}
	}

	template <class Derived, class Traits, bool MultiThread>
	void scene_manager<Derived, Traits, MultiThread>::draw(const typename Traits::paint_params &pp)
	{
		pp.clear(typename Traits::color(255, 255, 255));
		current_scene->draw(pp);
	}

	template <class Derived, class Traits, bool MultiThread>
	template <class Functor>
	void scene_manager<Derived, Traits, MultiThread>::iterate_objects(Functor functor)
	{
		functor(*current_scene);
	}

}
