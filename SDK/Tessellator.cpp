#include "Tessellator.h"

MaterialPtr::MaterialPtr(std::string materialName, bool switchable) {
	using materialPtrConst_t = void(__fastcall *)(MaterialPtr *, __int64, const HashedString *);
	static materialPtrConst_t materialPtrConst = reinterpret_cast<materialPtrConst_t>(FindSignature("48 89 4C 24 ? ? 48 83 EC ? 4C 8B CA 48 8B D9 ? ? 48 89 01 48 89 41 ? 48 8B 02 49 8B D0 49 8B C9 48 8B 40 ? ? ? ? ? ? ? 4C 8B C0"));

	static int64_t renderGroupBaseCommon = []() {
		auto sig = FindSignature("48 8D 0D ? ? ? ? 48 8B 40 ? ? ? ? ? ? ? 48 8B F8 48 8B 50 ? 48 85 D2") + 3;
		auto off = *reinterpret_cast<int *>(sig);
		return sig + 4 + off;
	}();
	static int64_t renderGroupBaseSwitchable = []() {
		auto sig = FindSignature("48 8D 0D ? ? ? ? 48 8B 40 ? ? ? ? ? ? ? 48 8B D8 48 8B 50 ? 48 85 D2 74") + 3;
		auto off = *reinterpret_cast<int *>(sig);
		return sig + 4 + off;
	}();

	HashedString hashedStr(materialName);
	materialPtrConst(this, switchable ? renderGroupBaseSwitchable : renderGroupBaseCommon, &hashedStr);
}

void Tessellator::addPostTransformOffset(float x, float y, float z) {
	this->transformOffset = this->transformOffset.add(x, y, z);
}

Mesh* BlockTessellator::getMeshForBlockInWorld(Tessellator *t, Block *block, Vec3i &pos) {
	using BT_getMeshForBlockInWorld_t = Mesh* (*)(BlockTessellator *, Tessellator *, Block *, Vec3i &, int, __int64);
	static BT_getMeshForBlockInWorld_t getMeshForBlock = reinterpret_cast<BT_getMeshForBlockInWorld_t>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 48 01 00 00 4D 8B E1 49 8B F0"));

	return getMeshForBlock(this, t, block, pos, block->getRenderLayer(), 0);
}

void BlockTessellator::tesselateBlockInWorld(Tessellator *t, Block *block, Vec3i &pos) {
	using BT_tesselateBlockInWorld_t =void(*)(BlockTessellator *, Tessellator *, Block *, Vec3i &, bool);
	static BT_tesselateBlockInWorld_t tesselateBlock = reinterpret_cast<BT_tesselateBlockInWorld_t>(FindSignature("40 53 55 56 57 48 83 EC 58 48 8B D9 49 8B F1 0F B6 49 78 49 8B E8 48 8B FA 84 C9 75 30 83 BB E0 00 00 00 00 74 27"));

	return tesselateBlock(this, t, block, pos, false);
}
