#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

class BlockLegacy;

class Block {
public:
	CLASS_FIELD(data, 0x8, uint16_t);
	CLASS_FIELD(blockLegacy, 0x10, BlockLegacy*);
	CLASS_FIELD(runtimeId, 0xB8, uint32_t);

	inline BlockLegacy* toLegacy() const { return this->blockLegacy; }

	virtual ~Block();
	virtual int getRenderLayer();
};