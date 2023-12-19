#include "BlockSource.h"

int BlockSource::getHeightMap(int x, int z) {
	using BlockSource_getHeightMap_t = int(__fastcall*)(BlockSource*, int, int);
	static auto BlockSource_getHeightMap = reinterpret_cast<BlockSource_getHeightMap_t>(FindSignature("40 53 48 83 EC ? 48 8B 01 48 8B D9 89 54 24 ? 48 8D 54 24 ? ? ? ? 24 ? ? ? ? 44 89 44 24 ? ? ? ? ? ? ? 4C 8B C0 48 85 C0"));
	return BlockSource_getHeightMap(this, x, z);
}