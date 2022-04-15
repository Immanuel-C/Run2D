#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vector.h"

namespace Run {
	namespace Math {
		typedef glm::mat4 Matrix4;
		typedef glm::mat3 Matrix3;
		typedef glm::mat2 Matrix2;

		typedef glm::imat4x4 IMatrix4;
		typedef glm::imat3x3 IMatrix3;
		typedef glm::imat2x2 IMatrix2;

		typedef glm::i16mat4x4 I16Matrix4;
		typedef glm::i16mat3x3 I16Matrix3;
		typedef glm::i16mat2x2 I16Matrix2;

		typedef glm::i8mat4x4 I8Matrix4;
		typedef glm::i8mat3x3 I8Matrix3;
		typedef glm::i8mat2x2 I8Matrix2;

		typedef glm::umat4x4 UIMatrix4;
		typedef glm::umat3x3 UIMatrix3;
		typedef glm::umat2x2 UIMatrix2;

		typedef glm::u16mat4x4 UI16Matrix4;
		typedef glm::u16mat3x3 UI16Matrix3;
		typedef glm::u16mat2x2 UI16Matrix2;

		typedef glm::u8mat4x4 UI8Matrix4;
		typedef glm::u8mat3x3 UI8Matrix3;
		typedef glm::u8mat2x2 UI8Matrix2;

		typedef glm::dmat4 DMatrix4;
		typedef glm::dmat3 DMatrix3;
		typedef glm::dmat2 DMatrix2;

		template <typename T>
		using TMatrix2 = glm::tmat2x2<T>;
		template <typename T>
		using TMatrix3 = glm::tmat3x3<T>;
		template <typename T>
		using TMatrix4 = glm::tmat4x4<T>;


		template <typename T, glm::length_t rowSize, glm::length_t colomnSize>
		using TMatrix = glm::mat<rowSize, colomnSize, T, glm::defaultp>;

		template <typename T>
		TMatrix4<T> rotateMatrix4(const TMatrix4<T>& matrix, T angle, const TVector3<T> rotationVector) 
		{ return glm::rotate(matrix, angle, rotationVector); }

		template <typename T>
		TMatrix4<T> translateMatrix4(const TMatrix4<T>& matrix, const TVector3<T> translationVector) 
		{ return glm::translate(matrix, translationVector); }

		template <typename T>
		TMatrix4<T> scaleMatrix4(const TMatrix4<T>& matrix, TVector3<T> scaleVector) 
		{ return glm::scale(matrix, scaleVector); }
	}
}