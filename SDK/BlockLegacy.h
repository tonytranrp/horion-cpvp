#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

#include "Block.h"
#include "BlockActor.h"
#include "BlockSource.h"
#include "HashedString.h"

class BlockMaterial { // actually called class Material
public:
	int32_t type;  // 5 for water, 6 for lava
	bool isNeverBuildable;
	bool isAlwaysDestroyable;
	bool isLiquid;
	float translucency;
	bool isBlockingMotion;
	bool isBlockingPrecipitation;
	bool isSolid;
	bool isSuperHot;
};
static_assert(sizeof(BlockMaterial) == 0x10);

class BlockSource;
class Block;
class Entity;

class BlockLegacy {
public:
	uintptr_t** Vtable;

	CLASS_FIELD(tileName, 0x8, std::string);
	CLASS_FIELD(rawName, 0x28, HashedString);  // name, xref: BlockLegacy::extractBlockNameInfo, BlockLegacy ctor assigns result of this struct
	CLASS_FIELD(material, 0xD8, class BlockMaterial*); // xref: BlockLegacy ctor, material param
	CLASS_FIELD(blockId, 0x154, uint16_t);  // xref: BlockLegacy ctor, int param

	void liquidGetFlow(Vec3* flowOut, BlockSource* reg, const Vec3i* pos);
	bool getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const Vec3i* pos);
	AABB* getOutline(BlockSource* region, const Vec3i* pos, AABB* input);
	bool hasWater(BlockSource* reg, const Vec3i& pos);
	bool canBeBuiltOver(BlockSource& region, const Vec3i& pos);
	const std::string& getRawNameStr() const;
};