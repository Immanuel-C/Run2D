#pragma once

#include <memory>

namespace Run {
	template<typename T>
	using Ref = std::shared_ptr<T>;

}