#include "BlockLegacy.h"

bool BlockLegacy::getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const Vec3i* pos) {
	return Utils::CallVFunc<5, bool>(
		this, collShapeOut, block, blockSource, pos, /*garbage optional entity ref*/ nullptr);
}

AABB* BlockLegacy::getOutline(BlockSource* region, const Vec3i* pos, AABB* input) {
	return Utils::CallVFunc<13, AABB*>(this, region, pos, input);
}

// LiquidBlock::_getFlow(IConstBlockSource const &,BlockPos const &,Material const &)
void BlockLegacy::liquidGetFlow(Vec3* flowOut, BlockSource* reg, const Vec3i* pos) {
	using liquid_getFlow_t = Vec3i*(__cdecl*)(Vec3*, BlockSource *, const Vec3i *, const BlockMaterial &);
	static liquid_getFlow_t liquid_getDepth = Utils::FuncFromSigOffset<liquid_getFlow_t>(FindSignature("E8 ? ? ? ? F3 0F 10 07 48 83 C3 18"), 1);
	
	liquid_getDepth(flowOut, reg, pos, *this->material);
}

bool BlockLegacy::hasWater(BlockSource* reg, const Vec3i& pos) {
	if(this->material->isLiquid){
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}

bool BlockLegacy::canBeBuiltOver(BlockSource& region, const Vec3i& pos) { // xref: Block::canBeBuiltOver
	return Utils::CallVFunc<114, bool>(this, &region, pos);
}

const std::string& BlockLegacy::getRawNameStr() const {
	return this->rawName.text;
}