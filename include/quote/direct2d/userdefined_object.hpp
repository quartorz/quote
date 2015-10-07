#pragma once

#include "base_types.hpp"
#include "object.hpp"
#include "resource_creator.hpp"
#include "object_creator.hpp"
#include "resource_manager.hpp"
#include "object_manager.hpp"

#include <unordered_set>
#include <vector>
#include <algorithm>
#include <functional>

namespace quote{ namespace direct2d{

	template <class Derived>
	class userdefined_object:
		public object,
		public resource_manager<userdefined_object<Derived>>,
		public object_manager<userdefined_object<Derived>>
	{
		object *hover, *focus;
		bool pushing;
		creation_params creation;
		bool created;

		std::vector<object*>::reverse_iterator find_object(const point &);

	public:
		userdefined_object();

		virtual bool is_colliding(const point &) override;
		virtual void on_left_press(const point &, hittest &) override;
		virtual void on_left_release(const point &, hittest &) override;
		virtual void on_mouse_move(const point &, hittest &) override;
		virtual void on_mouse_leave(const hittest &) override;
		virtual void on_lose_focus(const hittest &) override;

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const paint_params &);
	};

} }

#include "impl/userdefined_object.hpp"