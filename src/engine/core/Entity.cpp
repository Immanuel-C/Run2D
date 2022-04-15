#include "Entity.h"


namespace Run {
	void Entity::onCreate() {
		for (uint32_t i = 0; i < MAX_COMPONENTS; i++)
			if (m_compArr[i])
				m_compArr[i]->onCreate();
	}

	void Entity::onUpdate(double dt) {
		for (uint32_t i = 0; i < MAX_COMPONENTS; i++)
			if (m_compArr[i])
				m_compArr[i]->onUpdate(dt);
	}

	void Entity::onDestroy() {
		for (uint32_t i = 0; i < MAX_COMPONENTS; i++) {
			if (m_compArr[i]) {
				m_compArr[i]->onDestroy();
				delete m_compArr[i];
			}
		}
	}
}