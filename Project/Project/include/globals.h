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

enum EditState {
	WALL,
	LIGHT,
	HEAVY,
	SUPPORT,
	GOAL,
	DOOR,
	END
};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;