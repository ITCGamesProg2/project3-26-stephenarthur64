#pragma once
#include "raylib.h"

struct Time {
	Vector2 position;
	Vector2 velocity;
	float radius;
	int health;
};

enum AttackTypes {
	LIGHT,
	HEAVY,
	SPECIAL,
	SUPPORT
};

enum TimeAbilities {
	REWIND,
	SKIP,
	STOP,
	MAX
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;