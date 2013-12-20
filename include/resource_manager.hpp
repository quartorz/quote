#pragma once

#include <unordered_set>

namespace quote{

	template <class Derived, class Traits, bool CheckDuplicate = false, bool MultiThread = false /* –¢ŽÀ‘• */>
	class resource_manager{
		using resource = typename Traits::resource;

		std::unordered_set<resource*> resources;
		bool created = false;

	public:
		~resource_manager();

		bool create_resource(const typename Traits::creation_params&);
		void destroy_resource();

		void register_resource(typename Traits::resource*);
		void unregister_resource(typename Traits::resource*);
		void delete_resource(typename Traits::resource*);

		template <class Resource, class... Args>
		void init_resource(Resource *&, Args&&... args);
		template <class Resource, class... Args>
		Resource *new_resource(Args&&... args);
	};

}

#include "details/resource_manager.hpp"
