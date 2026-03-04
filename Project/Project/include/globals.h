#pragma once
#include "raylib.h"

struct Time {
	Vector2 position;
	Vector2 velocity;
	Rectangle spriteRect;
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
	LIGHTENEMY,
	HEAVYENEMY,
	SUPPORTENEMY,
	GOAL,
	DOOR,
	END
};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;