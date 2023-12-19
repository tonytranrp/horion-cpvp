#pragma once
#include "../SDK/ClientInstance.h"
#include "../SDK/GameMode.h"
#include "../Memory/Hooks.h"

class MoveUtils {
public:
	static void Tp(float speed, bool lerp, float Y);
	static void SetMotion(float speed, bool lerp, bool onTick);
	static bool isOnground(float height);
	static bool isOverVoid();
	static Vec2 GetMove(float speed, float direction, MoveInputHandler *move = nullptr);
};
