#pragma once

namespace Run {
	namespace Math {
		union Vector2 {
			struct {
				float x, y;
			};

			struct {
				float r, g;
			};

			struct {
				float u, v;
			};
		};


		union Vector3 {
			struct {
				float x, y, z;
			};

			struct {
				float r, g, b;
			};

			struct {
				float u, v, r;
			};
		};

		union Vector4 {
			struct {
				float x, y, z, w;
			};

			struct {
				float r, g, b, a;
			};
		};

		typedef Vector4 Colour;
	}
}