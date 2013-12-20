#pragma once

#include <vector>

namespace quote{

	template <class Derived, class Traits, bool CheckDuplicate=false, bool MultiThread=false /* –¢ŽÀ‘• */>
	class object_manager{
		std::vector<typename Traits::object*> objects;
		bool created = false;

	public:
		~object_manager();
		bool create_resource(const typename Traits::creation_params &);
		void destroy_resource();
		void draw(const typename Traits::paint_params &);

		void reorder_object(typename Traits::object *, typename Traits::object *base, bool behind=false);

		void register_object(typename Traits::object *);
		void unregister_object(typename Traits::object *);
		void delete_object(typename Traits::object *);

		template <class Object, class... Args>
		void init_object(Object *&, Args&&...);
		template <class Object, class... Args>
		Object *new_object(Args&&...);

		template <class Functor>
		void iterate_objects(Functor);
	};

}

#include "details/object_manager.hpp"
