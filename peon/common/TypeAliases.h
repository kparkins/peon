#ifndef PEON_TYPE_ALIASES_H
#define PEON_TYPE_ALIASES_H

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

namespace Peon {
    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T>
    using Unique = std::unique_ptr<T>;

    template<typename T>
    using Weak = std::weak_ptr<T>;
}

#endif