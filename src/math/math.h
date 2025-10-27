#pragma once
#include <numbers>
#include <cmath>
#include <Windows.h>

#include "../../ext/SimpleMath.h"

using namespace DirectX::SimpleMath;

class Vec2 {
public:
	constexpr Vec2(
		const float x = 0.f,
		const float y = 0.f) noexcept :
		x(x), y(y) { }

	float x, y;
};

class Vec3
{
public:
	constexpr Vec3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) {
	}

	constexpr const Vec3& operator-(const Vec3& other) const noexcept;
	constexpr const Vec3& operator+(const Vec3& other) const noexcept;
	constexpr const Vec3& operator/(const float factor) const noexcept;
	constexpr const Vec3& operator*(const float factor) const noexcept;

	// 3d -> 2d, explanations already exist.
	const bool world_to_screen(const DirectX::SimpleMath::Matrix& view_matrix, Vec2& out);

	const bool IsZero();

	float x, y, z;
};