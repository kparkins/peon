
#ifndef PEON_COMPONENT_VIEW_H
#define PEON_COMPONENT_VIEW_H

#include <vector>

#include "Component.h"
#include "ComponentIterator.h"
#include "Pool.h"

using std::vector;

namespace Peon {

    template <typename... IncludedTypes>
    class ComponentView {
    public:
        ComponentView(vector<Pool*> pools);

        ComponentIterator<IncludedTypes...> begin();
        ComponentIterator<IncludedTypes...> end();

        const ComponentIterator<IncludedTypes...> begin() const;
        const ComponentIterator<IncludedTypes...> end() const;

    private:
        vector<Pool*> pools;
    };

    template <typename... IncludedTypes>
    ComponentView<IncludedTypes...>::ComponentView(vector<Pool*> pools)
        : pools(pools) {
        ComponentId componentIds[] = { 0, Component<IncludedTypes>::Id()... };
    }

    template <typename... IncludedTypes>
    const ComponentIterator<IncludedTypes...>
        ComponentView<IncludedTypes...>::begin() const {
        return ComponentIterator<IncludedTypes...>(pools);
    }

    template <typename... IncludedTypes>
    const ComponentIterator<IncludedTypes...> ComponentView<IncludedTypes...>::end()
        const {
        return ComponentIterator<IncludedTypes...>(pools);
    }

    template <typename... IncludedTypes>
    ComponentIterator<IncludedTypes...> ComponentView<IncludedTypes...>::begin() {
        return ComponentIterator<IncludedTypes...>(pools);
    }

    template <typename... IncludedTypes>
    ComponentIterator<IncludedTypes...> ComponentView<IncludedTypes...>::end() {
        return ComponentIterator<IncludedTypes...>(pools);
    }

}

#endif