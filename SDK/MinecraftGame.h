#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

class MinecraftGame {
	char pad_0000[0xD8];  // 0x0000
public:
	bool canUseKeys;  // 0xD8

	const bool canUseKeybinds() {
		return canUseKeys;
	};

	class Font* getMojangles() {
		return *reinterpret_cast<Font**>((uintptr_t)(this) + 0xF88);
	}

	struct FontRepository* getFontRepo() {
		return **reinterpret_cast<FontRepository***>((uintptr_t)(this) + 0xF78);
	}

	class ClientInstance* getPrimaryClientInstance() {
		int64_t* ptr = 0;
		Utils::CallVFunc<2, int64_t*>((void*)((uintptr_t)this + 0x58), &ptr);
		return *(ClientInstance**)ptr;
	}
};

namespace Bedrock {
struct Platform_UWP {
	char pad_0x0[0x20];
	MinecraftGame* minecraftGame;
};
}

struct MainView {
	char pad_0x0[0xD0];
	Bedrock::Platform_UWP* platform;

public:
	static inline MainView* instance;
};