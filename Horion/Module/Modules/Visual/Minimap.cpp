#include "Minimap.h"

#include "../../../../SDK/BlockLegacy.h"
#include "../../../DrawUtils.h"

Minimap::Minimap() : IModule(0, Category::VISUAL, "Shows a map of the area around you.") {
	registerIntSetting("Delay", &this->delay, 60, 0, 200);
	registerIntSetting("Y Level", &this->yLevel, 320, 0, 320);
	registerBoolSetting("Chunk Borders", &this->chunkBorders, this->chunkBorders);
	//registerBoolSetting("Players", &this->players, this->players);
}

void Minimap::onTick(GameMode* gm) {
	static int Odelay = 0;
	if (++Odelay > this->delay) {
		doMinimap();
		Odelay = 0;
	}
}

void Minimap::doMinimap() {
	static uintptr_t getMapColorF = FindSignature("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B E9 4D 8B E0 4C 8B F2 4C 8B F9");
	using get_map_color_t = MC_Color*(__fastcall*)(Block*, MC_Color*, BlockSource*, Vec3i);
	static get_map_color_t getMapColor = reinterpret_cast<get_map_color_t>(getMapColorF);

	LocalPlayer* plr = Game.getLocalPlayer();
	Vec3 pos = *plr->getPos();

	// Clear the blocks list
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i] = {};  // Empty
	}

	Vec3i p = {(int)pos.x, (int)pos.y, (int)pos.z};

	static constexpr int map_div = map_size / 2;

	int idx = 0;
	for (int a = 0; a < map_size; a++) {
		for (int b = 0; b < map_size; b++) {
			int off[2] = {(map_size - a) - map_div, (map_size - b) - map_div};
			Vec3i bp = {p.x - *off, 0,
						  p.z - off[1]};
			bp.y = Game.getRegion()->getHeightMap(bp.x, bp.z) - 1;
			auto block = Game.getRegion()->getBlock(bp);
			size_t at = static_cast<size_t>(idx);
			MC_Color holder;
			getMapColor(block, &holder, Game.getRegion(), bp);
			this->blocks[at] = holder.arr;
			this->blocks[at].worldPos = bp;
			idx++;
		}
	}
}

void Minimap::onPreRender(MinecraftUIRenderContext* ctx) {
	auto plr = Game.getClientInstance()->getLocalPlayer();
	if (plr)
		DrawUtils::drawMinimap(this);
}