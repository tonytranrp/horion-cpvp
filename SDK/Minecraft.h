#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

class Minecraft {
private:
	char pad_0x0000[0xD8];  //0x0000
public:
	float* timer;  // 0x00D8

	Level* getLevel() {
		__int64 gamesess = *reinterpret_cast<__int64*>(reinterpret_cast<__int64>(this) + 0xC0);
		if (gamesess) {
			return **reinterpret_cast<Level***>(gamesess + 32);
		} else
			return nullptr;
	}
};