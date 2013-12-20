#pragma once

#include "scene.hpp"

namespace quote{

	template <class Derived, class Traits, bool MultiThread = false>
	class scene_manager{
	protected:
		std::unordered_map<int, scene<Traits, MultiThread>*> scenes;
		scene<Traits, MultiThread> *current_scene = nullptr;
		int current_idx = 0;
		bool created = false;

	public:
		void add_scene(int key, scene<Traits, MultiThread> *scene);
		void remove_scene(int key);
		void select_scene(int key);
		scene<Traits, MultiThread> *get_current_scene() const;
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

#include "details/scene_manager.hpp"
