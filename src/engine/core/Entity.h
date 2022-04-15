#pragma once

#include <vector>
#include <array>
#include <bitset>
#include <memory>

#include <ILog.h>

#include "Ecs.h"

#include "DynamicExport.h"

#include "Defines.h"

namespace Run {
	// Client can change this number if needed. Note though changing this to a very high number could cause major preformance drops!
	static constexpr size_t MAX_COMPONENTS = 32;

	class RUN_API Entity
	{
	public:
		Entity() = default;

		void onCreate();
		void onUpdate(double dt);
		void onDestroy();

		template<typename T, typename... TArgs>
		inline T& addComponent(TArgs&&... args) {
			T* compPtr = new T(args...);

			m_compArr[getComponentTypeID<T>()] = (Component*)compPtr;

			compPtr->entity = this;

			return *compPtr;
		}

		template<typename T>
		inline T& getComponent() {
			T* ptr = (T*)m_compArr[getComponentTypeID<T>()];
			return *ptr;
		}

		template<typename T>
		inline bool hasComponent() { return m_compArr[getComponentTypeID<T>()] != nullptr; }

	private:
		std::array<Component*, MAX_COMPONENTS> m_compArr;
	};
}

