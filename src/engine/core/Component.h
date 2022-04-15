#pragma once


namespace Run {
	class Entity;
	
	class Component
	{
	public:
		Entity* entity;

		virtual void onCreate() { }
		virtual void onUpdate(double dt) { }
		virtual void onDestroy() { }
	};
}


