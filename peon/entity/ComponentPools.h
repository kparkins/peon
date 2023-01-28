#ifndef PEON_COMPONENT_HUB_H
#define PEON_COMPONENT_HUB_H

#include "EcsTypes.h"
#include "Pool.h"

namespace Peon {

	class ComponentPools {
	protected:
		Pool* pools[MAX_COMPONENTS];
	};

}

#endif