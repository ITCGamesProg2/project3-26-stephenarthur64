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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;