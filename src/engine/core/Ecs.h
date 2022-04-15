#pragma once

#include "Component.h"

#include <ILog.h>
#include <utility>

namespace Run {
	using ComponentTypeID = int64_t;

	inline ComponentTypeID getUniqueComponentID() {
		static ComponentTypeID lastID = 0ull;
		return lastID++;
	}

	template <typename T>
	inline ComponentTypeID getComponentTypeID() {
		// A new version of this function will be created for each component
		// (because of template)
		static const ComponentTypeID typeID = getUniqueComponentID();
		return typeID;
	}

}