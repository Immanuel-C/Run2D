#pragma once

#include <glm/glm.hpp>

namespace Run {
	namespace Math {
		typedef glm::vec2 Vector2;
		typedef glm::vec3 Vector3;
		typedef glm::vec4 Vector4;

		typedef glm::ivec2 IVector2;
		typedef glm::ivec3 IVector3;
		typedef glm::ivec4 IVector4;

		typedef glm::i16vec2 I16Vector2;
		typedef glm::i16vec3 I16Vector3;
		typedef glm::i16vec4 I16Vector4;

		typedef glm::i8vec2 I8Vector2;
		typedef glm::i8vec3 I8Vector3;
		typedef glm::i8vec4 I8Vector4;

		typedef glm::uvec2 UIVector2;
		typedef glm::uvec3 UIVector3;
		typedef glm::uvec4 UIVector4;

		typedef glm::u16vec2 UI16Vector2;
		typedef glm::u16vec3 UI16Vector3;
		typedef glm::u16vec4 UI16Vector4;

		typedef glm::u8vec2 UI8Vector2;
		typedef glm::u8vec3 UI8Vector3;
		typedef glm::u8vec4 UI8Vector4;

		typedef glm::dvec2 DVector2;
		typedef glm::dvec3 DVector3;
		typedef glm::dvec4 DVector4;

		typedef glm::bvec2 BVector2;
		typedef glm::bvec3 BVector3;
		typedef glm::bvec4 BVector4;


		template <typename T>
		using TVector2 = glm::tvec2<T>;
		template <typename T>
		using TVector3 = glm::tvec3<T>;
		template <typename T>
		using TVector4 = glm::tvec4<T>;

		template <typename T, size_t length>
		using TVector = glm::vec<length, T, glm::packed_highp>;
	}
}