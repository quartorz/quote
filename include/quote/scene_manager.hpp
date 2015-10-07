#pragma once

#include "scene_base.hpp"

namespace quote{

	template <class Derived, class Traits>
	class scene_manager{
	protected:
		std::unordered_map<int, scene_base<Traits> *> scenes;
		scene_base<Traits> *current_scene = nullptr;
		int current_idx = 0;
		bool created = false;

	public:
		void add_scene(int key, scene_base<Traits> *scene);
		void remove_scene(int key);
		void select_scene(int key);
		scene_base<Traits> *get_scene(int key);
		scene_base<Traits> *get_current_scene() const;
		int get_scene_count() const;
		void on_size(int, int);
		void on_key_down(unsigned);
		void on_key_up(unsigned);
		void on_timer(unsigned);

		bool create_resource(const typename Traits::creation_params &);
		void destroy_resource();
		void draw(const typename Traits::paint_params &);

		template <class Functor>
		void iterate_objects(Functor);
	};

}

#include "impl/scene_manager.hpp"
