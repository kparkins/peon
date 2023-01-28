#ifndef PEON_COMPONENT_ITERATOR_H
#define PEON_COMPONENT_ITERATOR_H

#include <tuple>
#include <vector>

#include "Pool.h"

using std::tuple;
using std::vector;

namespace Peon {

	template <typename... ComponentTypes>
	class ComponentIterator {
	public:
		ComponentIterator(vector<Pool*> entities);

		tuple<ComponentTypes...> operator*();
		const tuple<ComponentTypes...> operator*() const;

		tuple<ComponentTypes...> operator->();
		const tuple<ComponentTypes...> operator->() const;

		ComponentIterator& operator++();
		ComponentIterator& operator--();
		ComponentIterator operator++(int);
		ComponentIterator operator--(int);
		ComponentIterator& operator+=(int n);
		ComponentIterator& operator-=(int n);
		ComponentIterator operator+(int n) const;
		ComponentIterator operator-(int n) const;

		size_t operator-(ComponentIterator const& r) const;
		bool operator<(ComponentIterator const& r) const;
		bool operator<=(ComponentIterator const& r) const;
		bool operator>(ComponentIterator const& r) const;
		bool operator>=(ComponentIterator const& r) const;
		bool operator!=(const ComponentIterator& r) const;
		bool operator==(const ComponentIterator& r) const;

	private:
		size_t index;
		vector<Pool*> pools;
	};

}

#endif