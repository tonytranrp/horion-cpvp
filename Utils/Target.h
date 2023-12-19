#pragma once

#include "../SDK/Entity.h"

class Target {
private:
	static bool containsOnlyASCII(const std::string& string);

public:
	static void init(LocalPlayer** lc);
	static bool isValidTarget(Entity* ent, bool mobs, bool visual);
};
