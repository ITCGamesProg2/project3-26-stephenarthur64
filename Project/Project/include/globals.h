#pragma once
#include "raylib.h"

struct Time {
	Vector2 position;
	Vector2 velocity;
	float radius;
};

enum AttackTypes {
	LIGHT,
	HEAVY,
	SPECIAL
};