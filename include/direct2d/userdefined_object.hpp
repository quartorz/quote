#pragma once

#include "base_types.hpp"
#include "object.hpp"

#include <unordered_set>
#include <vector>
#include <algorithm>
#include <functional>

namespace quote{ namespace direct2d{

	template <class Derived, bool CheckDuplicate=false, bool MultiThread=false /* –¢ŽÀ‘• */>
	class userdefined_object: public object{
		std::unordered_set<resource*> resources;
		std::vector<object*> objects;
		object *hover, *focus;
		bool pushing;
		creation_params creation;
		bool created;

		std::vector<object*>::reverse_iterator find_object(const point &);

	public:
		userdefined_object();
		~userdefined_object();

		virtual bool is_colliding(const point &) override;
		virtual void on_left_press(const point &, hittest &) override;
		virtual void on_left_release(const point &, hittest &) override;
		virtual void on_mouse_move(const point &, hittest &) override;
		virtual void on_mouse_leave(const hittest &) override;
		virtual void on_lose_focus(const hittest &) override;

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const paint_params &);

		void reorder_object(object *, object *base, bool behind=false);

		void register_object(object *);
		void unregister_object(object *);
		void delete_object(object *);

		void register_resource(resource *);
		void unregister_resource(resource *);
		void delete_resource(resource *);

		template <class Object, class... Args>
		void init_object(Object *&, Args&&...);
		template <class Object, class... Args>
		Object *new_object(Args&&...);

		template <class Resource, class... Args>
		void init_resource(Resource *&, Args&&...);
		template <class Resource, class... Args>
		Resource *new_resource(Args&&...);
	};

} }

#include "details\userdefined_object.hpp"
