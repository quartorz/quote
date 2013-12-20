#pragma once

#include "base_types.hpp"

namespace quote{ namespace direct2d{

	class resource{
	public:
		virtual ~resource()
		{
		}
		
		virtual bool create_resource(const creation_params&) = 0;
		virtual void destroy_resource() = 0;
//		virtual resource *duplicate(){}
	};

} }