#include "ChestBlockActor.h"

#include "../Utils/Utils.h"

bool ChestBlockActor::isBarrelBlock() {
	std::string alloc;
	Utils::CallVFunc<23, void, std::string*, __int64>(this, &alloc, 0);
	auto result = strcmp(alloc.c_str(), "container.barrel");
	return result == 0;
}
bool ChestBlockActor::isShulkerBlock() {
	std::string alloc;
	Utils::CallVFunc<23, void, std::string*, __int64>(this, &alloc, 0);
	auto result = strcmp(alloc.c_str(), "container.shulkerbox");
	return result == 0;
}
AABB ChestBlockActor::getObstructionAABB() {
	void* coolPtr = malloc(sizeof(AABB) + 4);
	Utils::CallVFunc<40, void, void*>(this, coolPtr);
	AABB ret = *reinterpret_cast<AABB*>(coolPtr);
	free(coolPtr);
	return ret;
}
