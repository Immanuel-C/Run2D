#pragma once

#include "Vector.h"
#include "DynamicExport.h"

namespace Run {
	union Colour {
		struct {
			float r, g, b, a;
		};

		struct {
			uint32_t ur, ug, ub, ua;
		};
	};
}