#pragma once

#include "Component.h"
#include "Vector.h"

namespace Run {
	class TransformComponent : public Component {
	public:
		void onCreate() override { }
		void onUpdate(double dt) override { }
		void onDestroy() override { }

		TransformComponent() = default;

		Math::Vector2 position{0.0f}, scale{0.0f};
		float rotation = 0.0f;
	};


	class SpriteComponent : public Component {
	public:
		void onCreate() override { }
		void onUpdate(double dt) override { }
		void onDestroy() override { }

		SpriteComponent() = default;
	};
}