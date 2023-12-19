#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

class Entity;
class Block;
class BlockActor;
class BlockMaterial;
class Bounds;
class BlockSourceListener;
class ActorBlockSyncMessage;

class BlockSource {
	virtual ~BlockSource();
public:
	virtual Block* getBlock(const Vec3i&, uint32_t aux) const;
	virtual Block* getBlock(const Vec3i& pos) const;
	virtual Block* getBlock(int32_t x, int32_t y, int32_t z) const;
	virtual BlockActor* getBlockEntity(const Vec3i& pos);
	virtual Block* getExtraBlock(const Vec3i& pos);
	virtual Block* getLiquidBlock(const Vec3i& pos);
	virtual bool hasBlock(const Vec3i&);
	virtual bool containsAnyLiquid(AABB const&);
	virtual bool containsMaterial(AABB const&, BlockMaterial);
	virtual BlockMaterial* getMaterial(const Vec3i&);
	virtual BlockMaterial* getMaterial(int, int, int);

private:
	virtual bool hasChunksAt(Bounds const&);
	virtual bool hasChunksAt(Vec3i const&, int);
	virtual bool hasChunksAt(AABB const&);
	virtual int getDimensionId(void);
	virtual __int64 fetchAABBs(std::vector<AABB>&, AABB const&, bool);
	virtual __int64 fetchCollisionShapes(std::vector<AABB>&, AABB const&, float*, bool, std::optional<class GetCollisionShapeInterface const>);
	virtual __int64 getTallestCollisionShape(AABB const&, float*, bool, class IActorMovementProxy*);
	virtual __int64 getWeakRef(void);
	virtual __int64 addListener(BlockSourceListener&);
	virtual __int64 removeListener(BlockSourceListener&);
	virtual __int64 fetchEntities(Entity const*, AABB const&, bool);
	virtual __int64 fetchEntities(int actorId, AABB const&, Entity const*);
	virtual __int64 setBlock(Vec3i const&, Block const&, int, ActorBlockSyncMessage const*, Entity*);
	virtual __int64 getMinHeight(void);
	virtual __int64 getDimension(void);
	virtual __int64 getDimensionConst(void);
	virtual __int64 getDimension1(void);
	virtual __int64 getChunkAt(Vec3i const&);
	virtual __int64 getILevel(void);
	virtual __int64 fetchAABBs(AABB const&, bool);
	virtual __int64 fetchCollisionShapes(AABB const&, float*, bool, IActorMovementProxy*);
	virtual __int64 getChunkSource(void);

public:
	int getHeightMap(int x, int z);
};
