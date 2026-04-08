#pragma once
#include "raylib.h"

struct Time {
	Vector2 position;
	Vector2 velocity;
	Rectangle spriteRect;
	int health;
};

struct SaveDetails {
	int level;
	int progress;
	float healthPercentage;
	float momentumPercentage;
	bool rewind;
	bool skip;
	bool stop;
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
	SELECT,
	WALL,
	LIGHTENEMY,
	HEAVYENEMY,
	SUPPORTENEMY,
	GOAL,
	DOOR,
	END
};


// Function sourced from the Raylib website:
// https://www.raylib.com/examples/audio/loader.html?name=audio_stream_effects
static void AudioProcessEffectLPF(void* buffer, unsigned int frames)
{
	static float low[2] = { 0.0f, 0.0f };
	static const float cutoff = 70.0f / 44100.0f; // 70 Hz lowpass filter
	const float k = cutoff / (cutoff + 0.1591549431f); // RC filter formula

	// Converts the buffer data before using it
	float* bufferData = (float*)buffer;
	for (unsigned int i = 0; i < frames * 2; i += 2)
	{
		const float l = bufferData[i];
		const float r = bufferData[i + 1];

		low[0] += k * (l - low[0]);
		low[1] += k * (r - low[1]);
		bufferData[i] = low[0];
		bufferData[i + 1] = low[1];
	}
}

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;