#pragma once

#include <unordered_set>

namespace quote{

	template <class Derived, class Traits>
	class resource_manager{
		using resource = typename Traits::resource;

		bool created = false;
		typename Traits::creation_params cp;

	protected:
		std::unordered_set<resource*> resources;

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

#include <quote/impl/resource_manager.hpp>
